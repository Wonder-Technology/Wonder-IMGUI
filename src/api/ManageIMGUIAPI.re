open IMGUIType;

let createRecord = ManageIMGUIService.createRecord;

let init = ManageIMGUIService.init;

let render = (gl, ioDataJsObj, record) =>
  ManageIMGUIService.render(
    gl,
    {
      pointUp: ioDataJsObj##pointUp,
      pointDown: ioDataJsObj##pointDown,
      pointPosition: ioDataJsObj##pointPosition,
      pointMovementDelta: ioDataJsObj##pointMovementDelta,
    },
    record,
  );

let getSetting = record => {
  let {
    textColor,
    buttonColor,
    hoverButtonColor,
    clickButtonColor,
    fontTexUvForWhite,
  } =
    RecordIMGUIService.getSetting(record);

  {
    "textColor": textColor,
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