open IMGUIType;

open FontType;

let _getExn = optionVal => optionVal;

let _isNone = [%raw
  optionVal => {|
    return optionVal === undefined;
    |}
];

/* let _computeMetrics = [%raw
     (
       fntData,
       text,
       letterSpacing,
       start,
       end_,
       width,
       hasFontDefDictionaryDataFunc,
       getGlyphFunc,
       getKerningFunc,
     ) => {|
                var curPen = 0,
                   curWidth = 0,
                   count = 0,
                   lastGlyph = null;

               if (!hasFontDefDictionaryDataFunc(fntData)) {
                   return [start, start, 0];
               }

               var end_ = Math.min(text.length, end_);

               for (let i=start; i < end_; i++) {
                   let id = text.charCodeAt(i),
                       glyph = getGlyphFunc(fntData, id);

                   if (!_isNone(glyph)) {
                       glyph = _getExn(glyph);
                       let kern = lastGlyph ? getKerningFunc(fntData, lastGlyph.id, glyph.id) : 0,
                           nextPen = null,
                           nextWidth = null;

                       curPen += kern;

                       nextPen = curPen + glyph.xAdvance + letterSpacing;
                       nextWidth = curPen + glyph.rect.width;

                       //we've hit our limit; we can't move onto the next glyph
                       if (nextWidth > width || nextPen > width){
                           if(count === 0){
                               count = 1;
                               curWidth = nextWidth;
                           }

                           break;
                       }

                       //otherwise continue along our line
                       curPen = nextPen;
                       curWidth = nextWidth;
                       lastGlyph = glyph;
                   }

                   count++
               }

               //make sure rightmost edge lines up with rend_ered glyphs
               if (lastGlyph){
                   curWidth += lastGlyph.xOffset;
               }

               return [
                    start,
                    start + count,
                    curWidth
               ]
               |}
   ]; */

let _computeMetrics =
    (
      fntData,
      text,
      letterSpacing,
      start,
      end_,
      width,
      hasFontDefDictionaryDataFunc,
      getGlyphFunc,
      getKerningFunc,
    ) =>
  ! hasFontDefDictionaryDataFunc(fntData) ?
    (start, start, 0) :
    {
      /* WonderLog.Log.print(
         (
               text,
               letterSpacing,
               start,
               end_,
               width,
             )
         ) |> ignore; */

      let curPen = ref(0);
      let curWidth = ref(0);
      let count = ref(0);
      let lastGlyph = ref(None);

      let break = ref(false);

      for (i in start to Js.Math.min_int(text |> Js.String.length, end_) - 1) {
        break^ ?
          () :
          {
            let id = text |> Js.String.charCodeAt(i);
            let glyph = getGlyphFunc(fntData, id);

            switch (glyph) {
            | Some(glyph) =>
              let (
                charXInImage,
                charYInImage,
                charWidthInImage,
                charHeightInImage,
              ) =
                glyph.rect;

              let kern =
                switch (lastGlyph^) {
                | Some(lastGlyph) =>
                  getKerningFunc(fntData, lastGlyph.id, glyph.id)
                | None => 0
                };

              curPen := curPen^ + kern;

              let nextPen = curPen^ + glyph.xAdvance + letterSpacing;
              let nextWidth = curPen^ + charWidthInImage;

              /* we've hit our limit; we can't move onto the next glyph */
              nextWidth > width || nextPen > width ?
                {
                  count^ === 0 ?
                    {
                      count := 1;
                      curWidth := nextWidth;
                    } :
                    ();

                  break := true;
                } :
                ();

              /* otherwise continue along our line */
              curPen := nextPen;
              curWidth := nextWidth;
              lastGlyph := Some(glyph);
            | None => ()
            };

            count := count^ + 1;
          };
      };

      /* make sure rightmost edge lines up with rendered glyphs */
      switch (lastGlyph^) {
      | Some(lastGlyph) => curWidth := curWidth^ + lastGlyph.xOffset
      | None => ()
      };

      (start, start + count^, curWidth^);
    };

let _findNewLineIndex = (text, char, start, end_) => {
  let idx = text |> Js.String.indexOfFrom(char, start);

  idx === (-1) || idx > end_ ? end_ : idx;
};

let _isWhitespace = char => [%re {|/\s/|}] |> Js.Re.test(char);

let _greedy = [%raw
  (
    fntData,
    text,
    letterSpacing,
    start,
    end_,
    width,
    hasFontDefDictionaryDataFunc,
    getGlyphFunc,
    getKerningFunc,
  ) => {|
             /* A greedy word wrapper based on LibGDX algorithm
            https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/graphics/g2d/BitmapFontCache.java */
    const NEWLINE_CHAR = '\n';

            var lines = [],
                textWidth = width;

            while (start < end_ && start < text.length) {
                /* get next newline position */
                let newLine = _findNewLineIndex(text, NEWLINE_CHAR, start, end_);

                /* eat whitespace at start of line */
                while (start < newLine) {
                    if (!_isWhitespace( text.charAt(start) )){
                        break;
                    }

                    start++;
                }

                /* determine visible # of glyphs for the available width */
                let measured = _computeMetrics(fntData, text, letterSpacing, start, newLine, textWidth, hasFontDefDictionaryDataFunc, getGlyphFunc, getKerningFunc),
                    lineEnd = start + (measured[1]-measured[0]),
                    nextStart = lineEnd + NEWLINE_CHAR.length;


                /* if we had to cut the line before the next newline... */
                if (lineEnd < newLine) {
                    /* find char to break on */
                    while (lineEnd > start) {
                        if (_isWhitespace(text.charAt(lineEnd))){
                            break;
                        }

                        lineEnd--
                    }

                    if (lineEnd === start) {
                        if (nextStart > start + NEWLINE_CHAR.length){
                            nextStart--;
                        }

 /* If no characters to break, show all. */
                        lineEnd = nextStart ;
                    }
                    else {
                        nextStart = lineEnd;

                        /* eat whitespace at end_ of line */
                        while (lineEnd > start) {
                            if (!_isWhitespace(text.charAt(lineEnd - NEWLINE_CHAR.length))){
                                break;
                            }

                            lineEnd--;
                        }
                    }
                }

                if (lineEnd >= start) {
                    lines.push(_computeMetrics(fntData, text, letterSpacing, start, lineEnd, textWidth, hasFontDefDictionaryDataFunc, getGlyphFunc, getKerningFunc));
                }

                start = nextStart
            }
            return lines


            |}
];

let getLines =
    (fntData, text, (letterSpacing, width, start, end_), fallbackGlyphTuple) =>
  _greedy(
    fntData,
    text,
    letterSpacing,
    start,
    end_,
    width,
    FontService.hasFontDefDictionaryData,
    BitmapFontSearchGlyphIMGUIService.getGlyph(fallbackGlyphTuple),
    BitmapFontParserIMGUIService.getKerning,
  );