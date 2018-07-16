open IMGUIType;

let getFontData = record => record.fontData |> OptionService.unsafeGet;

let getWebglData = record => record.webglData |> OptionService.unsafeGet;