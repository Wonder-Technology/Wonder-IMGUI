open WonderWebgl.GlType;

open StructureType;

open FontType;

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

type customTextureShaderData = {
  program,
  aPositonLocation: attributeLocation,
  aColorLocation: attributeLocation,
  aTexCoordLocation: attributeLocation,
  positionBuffer: buffer,
  colorBuffer: buffer,
  texCoordBuffer: buffer,
  indexBuffer: buffer,
};

type fontTextureShaderData = {
  program,
  aPositonLocation: attributeLocation,
  aColorLocation: attributeLocation,
  aTexCoordLocation: attributeLocation,
  positionBuffer: buffer,
  colorBuffer: buffer,
  texCoordBuffer: buffer,
  indexBuffer: buffer,
};

type noTextureShaderData = {
  program,
  aPositonLocation: attributeLocation,
  aColorLocation: attributeLocation,
  positionBuffer: buffer,
  colorBuffer: buffer,
  indexBuffer: buffer,
};

type webglData = {
  customTextureShaderData,
  fontTextureShaderData,
  noTextureShaderData,
  fontTexture: texture,
  lastWebglData: option(lastWebglData),
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
  fntDataMap: WonderCommonlib.MutableHashMapService.t(fntData),
  bitmapMap:
    WonderCommonlib.MutableHashMapService.t(
      WonderWebgl.DomExtendType.imageElement,
    ),
  customImageArr:
    array(
      (WonderWebgl.DomExtendType.imageElement, string, ImageType.imageType),
    ),
  customTextureMap:
    WonderCommonlib.MutableHashMapService.t(WonderWebgl.GlType.texture),
};

/* type radioButtonData = {
     isSelectedMap: WonderCommonlib.MutableHashMapService.t(int),
   };

   type checkboxData = {
     index: int,
     isSelectedMap: WonderCommonlib.MutableSparseMapService.t(bool),
   };

   type sliderData = {
     index: int,
     valueMap: WonderCommonlib.MutableSparseMapService.t(float),
   }; */

type groupData = {
  positionArr: array(position),
  index: int,
};

type layoutData = {groupData};

type drawData = {
  noTextureDrawData: DrawDataType.noTextureDrawData,
  customTextureDrawData: DrawDataType.customTextureDrawData,
  fontTextureDrawData: DrawDataType.fontTextureDrawData,
  customTextureDrawDataMap:
    WonderCommonlib.MutableHashMapService.t(
      DrawDataType.customTextureDrawData,
    ),
};

/* type controlData = {
     radioButtonData,
     checkboxData,
     sliderData,
   }; */

type customControlAPIJsObj;

type customControlFuncInputData;

type customControlFuncOutputData;

type skinName = string;

type customStyleName = string;

type customControlFunctionShowData =
  Js.Nullable.t((skinName, Js.Nullable.t(customStyleName)));

type customDataForIMGUIFunc;

type apiJsObj;

type imguiFunc =
  (. customDataForIMGUIFunc, apiJsObj, imguiRecord) => imguiRecord
and imguiFuncData = {
  imguiFunc: option(imguiFunc),
  customDataForIMGUIFunc: option(customDataForIMGUIFunc),
  apiJsObj,
}
and customControlFunc =
  (. customControlFuncInputData, customControlFunctionShowData, imguiRecord) =>
  (imguiRecord, customControlFuncOutputData)
and customControlData = {
  apiJsObj: customControlAPIJsObj,
  funcMap: WonderCommonlib.ImmutableHashMapService.t(customControlFunc),
}
and extendData = {
  customControlData,
  skinData: SkinType.skinData,
}
and imguiRecord = {
  assetData,
  fontData: option(fontData),
  webglData: option(webglData),
  drawData,
  imguiFuncData,
  /* controlData, */
  ioData,
  layoutData,
  extendData,
};