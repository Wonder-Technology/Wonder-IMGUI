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
  let {textColor, buttonSetting, radioButtonSetting, fontTexUvForWhite} =
    RecordIMGUIService.getSetting(record);

  let buttonSettingJsObj = {
    "buttonColor": buttonSetting.buttonColor,
    "hoverButtonColor": buttonSetting.hoverButtonColor,
    "clickButtonColor": buttonSetting.clickButtonColor,
  };
  let radioButtonSettingJsObj = {
    "radioButtonOuterColor": radioButtonSetting.radioButtonOuterColor,
    "radioButtonInnerColor": radioButtonSetting.radioButtonInnerColor,
    "radioButtonOuterColorHover":
      radioButtonSetting.radioButtonOuterColorHover,
    "radioButtonInnerColorHover":
      radioButtonSetting.radioButtonInnerColorHover,
    "radioButtonCircleSegments": radioButtonSetting.radioButtonCircleSegments,
    "radioButtonInnerRadius": radioButtonSetting.radioButtonInnerRadius,
    "radioButtonOuterRadius": radioButtonSetting.radioButtonOuterRadius,
  };

  {
    "textColor": textColor,
    "buttonSetting": buttonSettingJsObj,
    "radioButtonSetting": radioButtonSettingJsObj,
    "fontTexUvForWhite": fontTexUvForWhite,
  };
};

let setSetting = (settingJsObj, record) => {
  let buttonSettingJsObj = settingJsObj##buttonSetting;
  let radioButtonSettingJsObj = settingJsObj##radioButtonSetting;

  let buttonSetting = {
    buttonColor: buttonSettingJsObj##buttonColor,
    hoverButtonColor: buttonSettingJsObj##hoverButtonColor,
    clickButtonColor: buttonSettingJsObj##clickButtonColor,
  };

  let radioButtonSetting = {
    radioButtonOuterColor: radioButtonSettingJsObj##radioButtonOuterColor,
    radioButtonInnerColor: radioButtonSettingJsObj##radioButtonInnerColor,
    radioButtonOuterColorHover: radioButtonSettingJsObj##radioButtonOuterColorHover,
    radioButtonInnerColorHover: radioButtonSettingJsObj##radioButtonInnerColorHover,
    radioButtonCircleSegments: radioButtonSettingJsObj##radioButtonCircleSegments,
    radioButtonInnerRadius: radioButtonSettingJsObj##radioButtonInnerRadius,
    radioButtonOuterRadius: radioButtonSettingJsObj##radioButtonOuterRadius,
  };

  RecordIMGUIService.setSetting(
    {
      textColor: settingJsObj##textColor,
      buttonSetting,
      radioButtonSetting,
      fontTexUvForWhite: settingJsObj##fontTexUvForWhite,
    },
    record,
  );
};

let getCustomData = ManageIMGUIService.getCustomData;

let getIMGUIFunc = ManageIMGUIService.getIMGUIFunc;

let setIMGUIFunc = ManageIMGUIService.setIMGUIFunc;