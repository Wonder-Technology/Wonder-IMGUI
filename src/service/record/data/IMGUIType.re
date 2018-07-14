open GlType;

type rect = (int, int, int, int);

type uv = (float, float);

type colorArr = array(float);

type setting = {
  textScale: float,
  textColorArr: colorArr,
};

/* type ioData = {
     mouseLeftDownCur: bool,
     mouseLeftDownPrev: bool,
     mousePositionCur: (int, int),
     mousePositionPrev: (int, int),
   }; */

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
  uProjectionMatLocation: uniformLocation,
  uSampler2DLocation: uniformLocation,
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

type fftData = WonderCommonlib.HashMapService.t(array(fftCharData));

type imguiFunc = (. imguiRecord) => imguiRecord
and imguiRecord = {
  setting,
  fftData,
  webglData: option(webglData),
  drawDataArr: DrawDataArrType.drawDataArr,
  /* ioData, */
  /* activeWidgetId: option(int), */
  imguiFunc: option(imguiFunc),
};