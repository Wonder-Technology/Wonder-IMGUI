open WonderWebgl.GlType;

open StructureType;

open FontType;

type radioButtonSetting = {
  radioButtonOuterColor: colorArr,
  radioButtonInnerColor: colorArr,
  radioButtonOuterColorHover: colorArr,
  radioButtonInnerColorHover: colorArr,
  radioButtonCircleSegments: int,
  radioButtonInnerRadius: float,
  radioButtonOuterRadius: float,
};

type buttonSetting = {
  buttonColor: colorArr,
  hoverButtonColor: colorArr,
  clickButtonColor: colorArr,
};

type setting = {
  textColor: colorArr,
  fontTexUvForWhite: array(float),
  buttonSetting,
  radioButtonSetting,
};

type point('a) = ('a, 'a);

type ioData = {
  pointUp: bool,
  pointDown: bool,
  pointPosition: point(int),
  pointMovementDelta: point(int),
};

type lastWebglData = {
  lastProgram: option(program),
  lastElementArrayBuffer: buffer,
  lastArrayBuffer: buffer,
  lastTexture: option(texture),
  lastIsEnableDepthTest: bool,
  lastIsEnableBlend: bool,
};

type webglData = {
  program,
  positionBuffer: buffer,
  colorBuffer: buffer,
  texCoordBuffer: buffer,
  indexBuffer: buffer,
  fontTexture: texture,
  aPositonLocation: attributeLocation,
  aColorLocation: attributeLocation,
  aTexCoordLocation: attributeLocation,
  lastWebglData: option(lastWebglData),
  currentFontTextureDrawDataBaseIndex: int,
};

type fftCharData = {
  .
  "x0": int,
  "y0": int,
  "x1": int,
  "y1": int,
  "xoff": float,
  "yoff": float,
  "xoff2": float,
  "yoff2": float,
  "xadvance": float,
};

type fontData = {
  fntFilePath: string,
  bitmapFilePath: string,
};

type assetData = {
  fntId: string,
  bitmapId: string,
  fntDataMap: WonderCommonlib.HashMapService.t(fntData),
  bitmapMap:
    WonderCommonlib.HashMapService.t(WonderWebgl.DomExtendType.imageElement),
  customImageArr:
    array(
      (WonderWebgl.DomExtendType.imageElement, string, ImageType.imageType),
    ),
  customTextureMap:
    WonderCommonlib.HashMapService.t(WonderWebgl.GlType.texture),
};

type radioButtonData = {
  isSelectedMap: WonderCommonlib.HashMapService.t(int),
};

type groupData = {
  positionArr: array(position),
  index: int,
};

type layoutData = {groupData};

type customDataForIMGUIFunc;

type apiJsObj = {
  .
  "label":
    (. (int, int, int, int), string, FontType.align, imguiRecord) =>
    imguiRecord,
  "image":
    (
      . (int, int, int, int),
      (float, float, float, float),
      string,
      imguiRecord
    ) =>
    imguiRecord,
  "button":
    (. (int, int, int, int), string, imguiRecord) => (imguiRecord, bool),
  "box": (. (int, int, int, int), colorArr, imguiRecord) => imguiRecord,
  "radioButton":
    (. Js.Array.t(((int, int, int, int), string)), string, imguiRecord) =>
    (imguiRecord, option(int)),
  "beginGroup": (. (int, int), imguiRecord) => imguiRecord,
  "endGroup": (. imguiRecord) => imguiRecord,
}
and imguiFunc = (customDataForIMGUIFunc, apiJsObj, imguiRecord) => imguiRecord
and imguiFuncData = {
  imguiFunc: option(imguiFunc),
  customDataForIMGUIFunc: option(customDataForIMGUIFunc),
  apiJsObj,
}
and imguiRecord = {
  setting,
  assetData,
  fontData: option(fontData),
  webglData: option(webglData),
  drawDataArr: DrawDataArrType.drawDataArr,
  imguiFuncData,
  radioButtonData,
  ioData,
  layoutData,
  /* activeWidgetId: option(int), */
};