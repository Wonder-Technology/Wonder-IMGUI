open IMGUIType;

open FontType;

let _parseStrToObj = str => {
  let item_exp = [%re {|/\w+=[^ \r\n]+/gi|}];
  let int_exp = [%re {|/^[\-]?\d+$/|}];

  let obj = WonderCommonlib.MutableHashMapService.createEmpty();

  let break = ref(false);

  while (! break^) {
    switch (item_exp |> Js.Re.exec(str)) {
    | None => break := true
    | Some(result) =>
      Js.Nullable.iter(
        Js.Re.captures(result)[0],
        (. tempStr) => {
          let index = tempStr |> Js.String.indexOf("=");
          let key = tempStr |> Js.String.substring(~from=0, ~to_=index);
          let value = tempStr |> Js.String.substringToEnd(~from=index + 1);

          let value =
            Js.Re.test(value, int_exp) ?
              value :
              value->(Js.String.get(0)) == "\"" ?
                value
                |> Js.String.substring(
                     ~from=1,
                     ~to_=value |> Js.String.length,
                   ) :
                value;

          obj
          |> WonderCommonlib.MutableHashMapService.set(
               key,
               value |> StringService.toInt,
             )
          |> ignore;
        },
      )
    };
  };

  obj;
};

let _parseChar = (fntStr: string) => {
  let fontDefDictionary =
    WonderCommonlib.MutableSparseMapService.createEmpty();

  let char_exp = [%re {|/char [^\n]*(\n|$)/gi|}];

  let break = ref(false);

  while (! break^) {
    switch (char_exp |> Js.Re.exec(fntStr)) {
    | None => break := true
    | Some(result) =>
      Js.Nullable.iter(
        Js.Re.captures(result)[0],
        (. char) => {
          let charObj = _parseStrToObj(char);
          let charId =
            charObj |> WonderCommonlib.MutableHashMapService.unsafeGet("id");

          fontDefDictionary
          |> WonderCommonlib.MutableSparseMapService.set(
               charId,
               {
                 id: charId,
                 rect: (
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet("x"),
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet("y"),
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet("width"),
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet(
                        "height",
                      ),
                 ),
                 xOffset:
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet(
                        "xoffset",
                      ),
                 yOffset:
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet(
                        "yoffset",
                      ),
                 /* xAdvance equal width of char texture */
                 xAdvance:
                   charObj
                   |> WonderCommonlib.MutableHashMapService.unsafeGet(
                        "xadvance",
                      ),
               },
             )
          |> ignore;
        },
      )
    };
  };

  fontDefDictionary;
};

let buildKerningHashMapKey = (first, second) => first * 1000 + second;

let _changeKerningArrayToHashMap = kerningArray =>
  kerningArray
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. map, {first, second, amount}) =>
         map
         |> WonderCommonlib.MutableSparseMapService.set(
              buildKerningHashMapKey(first, second),
              amount,
            ),
       WonderCommonlib.MutableSparseMapService.createEmpty(),
     );

let _parseKerning = (fntStr: string) => {
  let kerningArray = [||];

  let kerning_exp = [%re {|/kerning [^\n]*(\n|$)/gi|}];

  let break = ref(false);

  while (! break^) {
    switch (kerning_exp |> Js.Re.exec(fntStr)) {
    | None => break := true
    | Some(result) =>
      Js.Nullable.iter(
        Js.Re.captures(result)[0],
        (. kerning) => {
          let kerningObj = _parseStrToObj(kerning);

          kerningArray
          |> ArrayService.push({
               first:
                 kerningObj
                 |> WonderCommonlib.MutableHashMapService.unsafeGet("first"),
               second:
                 kerningObj
                 |> WonderCommonlib.MutableHashMapService.unsafeGet("second"),
               amount:
                 kerningObj
                 |> WonderCommonlib.MutableHashMapService.unsafeGet("amount"),
             })
          |> ignore;
        },
      )
    };
  };

  kerningArray |> _changeKerningArrayToHashMap;
};

let parse = fntStr => {
  let common_exp = [%re {|/common [^\n]*(\n|$)/gi|}];
  let page_exp = [%re {|/page [^\n]*(\n|$)/gi|}];

  /*!
    xAdvance:the number of pixels the cursor move forward to draw the next char after draw current char
    xOffset:pixel x offset of current font when drawing
    yOffset:pixel y offset of current font when drawing
    */

  let commonObj =
    _parseStrToObj(
      switch (common_exp |> Js.Re.exec(fntStr)) {
      | None =>
        WonderLog.Log.fatal(
          WonderLog.Log.buildFatalMessage(
            ~title="parse",
            ~description={j|fntStr should match common_exp|j},
            ~reason="",
            ~solution={j||j},
            ~params={j|fntStr: $fntStr
    common_exp: $common_exp |j},
          ),
        )
      | Some(result) =>
        Js.Nullable.toOption(Js.Re.captures(result)[0])
        |> OptionService.unsafeGet
      },
    );

  let pageObj =
    _parseStrToObj(
      switch (page_exp |> Js.Re.exec(fntStr)) {
      | None =>
        WonderLog.Log.fatal(
          WonderLog.Log.buildFatalMessage(
            ~title="parse",
            ~description={j|fntStr should match page_exp|j},
            ~reason="",
            ~solution={j||j},
            ~params={j|fntStr: $fntStr
    page_exp: $page_exp |j},
          ),
        )
      | Some(result) =>
        Js.Nullable.toOption(Js.Re.captures(result)[0])
        |> OptionService.unsafeGet
      },
    );

  switch (pageObj |> WonderCommonlib.MutableHashMapService.get("id")) {
  | None =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="parse",
        ~description={j|file could not be found|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  | _ => ()
  };

  /* TODO support multi pages */
  {
    commonHeight:
      commonObj
      |> WonderCommonlib.MutableHashMapService.unsafeGet("lineHeight"),
    commonBase:
      commonObj |> WonderCommonlib.MutableHashMapService.unsafeGet("base"),
    scaleW:
      commonObj |> WonderCommonlib.MutableHashMapService.unsafeGet("scaleW"),
    scaleH:
      commonObj |> WonderCommonlib.MutableHashMapService.unsafeGet("scaleH"),
    fontDefDictionary: _parseChar(fntStr),
    kerningMap: _parseKerning(fntStr),
  };
};