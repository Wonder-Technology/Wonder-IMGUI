open StructureType;

type align =
  | Left
  | Center
  | Right;

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