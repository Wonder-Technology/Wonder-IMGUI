open IMGUIType;

let createRecord = ManageIMGUIService.createRecord;

let init = ManageIMGUIService.init;

let render = ManageIMGUIService.render;

let getSetting = record => {
  let {
    textColor,
    buttonSpacing,
    buttonColor,
    hoverButtonColor,
    clickButtonColor,
    fontTexUvForWhite,
  } =
    RecordIMGUIService.getSetting(record);

  {
    "textColor": textColor,
    "buttonSpacing": buttonSpacing,
    "buttonColor": buttonColor,
    "hoverButtonColor": hoverButtonColor,
    "clickButtonColor": clickButtonColor,
    "fontTexUvForWhite": fontTexUvForWhite,
  };
};

let setSetting = (settingJsObj, record) =>
  RecordIMGUIService.setSetting(
    {
      textColor: settingJsObj##textColor,
      buttonSpacing: settingJsObj##buttonSpacing,
      buttonColor: settingJsObj##buttonColor,
      hoverButtonColor: settingJsObj##hoverButtonColor,
      clickButtonColor: settingJsObj##clickButtonColor,
      fontTexUvForWhite: settingJsObj##fontTexUvForWhite,
    },
    record,
  );

let getCustomData = ManageIMGUIService.getCustomData;

let getIMGUIFunc = ManageIMGUIService.getIMGUIFunc;

let setIMGUIFunc = ManageIMGUIService.setIMGUIFunc;