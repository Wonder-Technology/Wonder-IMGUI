open IMGUIType;

let getFontData = record => record.fontData;

let unsafeGetFontData = record =>
  getFontData(record) |> OptionService.unsafeGet;

let unsafeGetWebglData = record => record.webglData |> OptionService.unsafeGet;

let getSetting = record => record.setting;

let setSetting = (setting, record) => {...record, setting};

let getIOData = record => record.ioData;

/* let getRadioButtonData = record => record.controlData.radioButtonData; */

let getFontTextureDrawData = record => record.drawData.fontTextureDrawData;

let getCustomTextureDrawDataMap = record =>
  record.drawData.customTextureDrawDataMap;

/* let getControlData = record => record.controlData;

   let setControlData = (controlData, record) => {...record, controlData}; */