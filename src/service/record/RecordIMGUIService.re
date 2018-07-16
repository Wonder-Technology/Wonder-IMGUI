open IMGUIType;

let getFontData = record => record.fontData;

let unsafeGetFontData = record =>
  getFontData(record) |> OptionService.unsafeGet;

let unsafeGetWebglData = record => record.webglData |> OptionService.unsafeGet;