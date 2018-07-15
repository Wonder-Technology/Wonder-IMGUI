open GlType;

type align =
  | Left
  | Center
  | Right;

type rect = (int, int, int, int);

type uv = (float, float, float, float);

type colorArr = array(float);

type setting = {
  /* textScale: float, */
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

type fontData = {
  fntFilePath: string,
  bitmapFilePath: string,
  fntId: string,
  bitmapId: string,
};

type fntCharData = {
  id: int,
  rect,
  xOffset: int,
  yOffset: int,
  xAdvance: int,
  /* page:number; */
};

type kerningData = {
  first: int,
  second: int,
  amount: int,
};

type fntData = {
  commonHeight: int,
  commonBase: int,
  scaleW: int,
  scaleH: int,
  /* atlasName: string, */
  fontDefDictionary: WonderCommonlib.SparseMapService.t(fntCharData),
  kerningArray: array(kerningData),
  /* isMultiPages: bool, */
};

type layoutData = {
  position: (int, int),
  data: fntCharData,
  index: int,
  line: int,
};

type assetData = {
  fntDataMap: WonderCommonlib.HashMapService.t(fntData),
  bitmapMap: WonderCommonlib.HashMapService.t(DomExtendType.imageElement),
};

type imguiFunc = (. imguiRecord) => imguiRecord
and imguiRecord = {
  setting,
  assetData,
  fontData: option(fontData),
  webglData: option(webglData),
  drawDataArr: DrawDataArrType.drawDataArr,
  /* ioData, */
  /* activeWidgetId: option(int), */
  imguiFunc: option(imguiFunc),
};