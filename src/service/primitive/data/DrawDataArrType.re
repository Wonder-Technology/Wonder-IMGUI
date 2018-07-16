type drawType =
  | FontTexture
  | CustomTexture;

type drawData = {
  drawType,
  customTexture: option(GlType.texture),
  verticeArr: array(float),
  colorArr: array(float),
  texCoordArr: array(float),
  indexArr: array(int),
};

type drawDataArr = array(drawData);

type drawElementsData = {
  drawType,
  customTexture: option(GlType.texture),
  count: int,
  countOffset: int,
};

type drawElementsDataArr = array(drawElementsData);

external drawTypeToInt : drawType => int = "%identity";