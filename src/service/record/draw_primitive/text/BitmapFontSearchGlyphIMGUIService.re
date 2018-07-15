open IMGUIType;

let _getTabId = () =>
  "\t" |> Js.String.charCodeAt(0) |> NumberType.floatToInt;

let _getSpaceId = () =>
  " " |> Js.String.charCodeAt(0) |> NumberType.floatToInt;

let _getFontDefDictionary = fntData => fntData.fontDefDictionary;

let _getGlyphById = (fntData, id) => {
  let dict = _getFontDefDictionary(fntData);

  dict |> WonderCommonlib.SparseMapService.get(id);
};

let getGlyph = (fntData, id, fallbackSpaceGlyph, fallbackTabGlyph) => {
  let glyph = _getGlyphById(fntData, id);

  switch (_getGlyphById(fntData, id)) {
  | Some(glyph) => glyph |. Some
  | None =>
    id === _getTabId() ?
      fallbackTabGlyph |. Some :
      id === _getSpaceId() ? fallbackSpaceGlyph |. Some : None
  };
};

let _getMGlyph = fntData => {
  let m_widthArr = [|"m", "w"|];

  m_widthArr
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. glyph, m_width) =>
         switch (glyph) {
         | Some(glyph) => Some(glyph)
         | None =>
           _getGlyphById(
             fntData,
             m_width |> Js.String.charCodeAt(0) |> NumberType.floatToInt,
           )
         },
       None,
     );
};

let _getFirstGlyph = fntData =>
  Array.unsafe_get(
    _getFontDefDictionary(fntData) |> SparseMapService.getValidValues,
    0,
  );

let setupSpaceGlyphs = (fntData, tabSize) => {
  /* try to get space glyph
     then fall back to the 'm' or 'w' glyphs
     then fall back to the first glyph available */
  let space =
    switch (_getGlyphById(fntData, _getSpaceId())) {
    | Some(space) => space
    | None =>
      switch (_getMGlyph(fntData)) {
      | Some(space) => space
      | None => _getFirstGlyph(fntData)
      }
    };

  (
    space,
    {
      id: _getTabId(),
      rect: (0, 0, 0, 0),
      xOffset: 0,
      yOffset: 0,
      xAdvance: tabSize * space.xAdvance,
    },
  );
};