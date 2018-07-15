open IMGUIType;

/* TODO add tabSize, letterSpacing to setting */

let getLayoutData =
    (text: string, (width, tabSize, letterSpacing, align), record) =>
  switch (AssetIMGUIService.getFntData(record)) {
  | None =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="getLayoutData",
        ~description={j|impossible to create font: not find fnt file|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  | Some(fntData) =>
    let (fallbackSpaceGlyph, fallbackTabGlyph) =
      BitmapFontSearchGlyphIMGUIService.setupSpaceGlyphs(fntData, tabSize);

    let lines =
      BitmapFontWordWrapperIMGUIService.getLines(
        fntData,
        text,
        (letterSpacing, width, 0, text |> Js.String.length),
      );

    let lineHeight = fntData.commonHeight;

    let minWidth = width;

    let maxLineWidth =
      lines
      |> WonderCommonlib.ArrayService.reduceOneParam(
           (. width, (start, end_, curWidth)) =>
             Js.Math.maxMany_int([|width, curWidth, minWidth|]),
           0,
         );

    let (layoutDataArr, x, y, lastGlyph) =
      lines
      |> WonderCommonlib.ArrayService.reduceOneParami(
           (.
             (layoutDataArr, x, y, lastGlyph),
             (start, end_, lineWidth),
             lineIndex,
           ) => {
             let lastGlyph = ref(None);
             let x = ref(x);
             let y = ref(y);

             for (i in start to end_ - 1) {
               let id =
                 text |> Js.String.charCodeAt(i) |> NumberType.floatToInt;

               switch (
                 BitmapFontSearchGlyphIMGUIService.getGlyph(
                   fntData,
                   id,
                   fallbackSpaceGlyph,
                   fallbackTabGlyph,
                 )
               ) {
               | Some(glyph) =>
                 let tx =
                   switch (lastGlyph^) {
                   | Some(lastGlyph) =>
                     x :=
                       x^
                       + BitmapFontParserIMGUIService.getKerning(
                           fntData,
                           lastGlyph.id,
                           glyph.id,
                         );
                     x^;
                   | None => x^
                   };

                 let tx =
                   switch (align) {
                   | Center => tx + (maxLineWidth - lineWidth) / 2
                   | Right => tx + (maxLineWidth - lineWidth)
                   };

                 layoutDataArr
                 |> ArrayService.push({
                      position: (tx + glyph.xOffset, y^ + glyph.yOffset),
                      data: glyph,
                      index: i,
                      line: lineIndex,
                    })
                 |> ignore;

                 x := x^ + glyph.xAdvance + letterSpacing;
                 lastGlyph := Some(glyph);
               | None => ()
               };
             };

             (layoutDataArr, 0, y^ + lineHeight, lastGlyph^);
           },
           ([||], 0, 0, None),
         );

    layoutDataArr;
  };