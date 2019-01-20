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
  fontDefDictionary: WonderCommonlib.MutableSparseMapService.t(fntCharData),
  kerningMap: WonderCommonlib.MutableSparseMapService.t(int),
  /* isMultiPages: bool, */
};

type layoutData = {
  position: (int, int),
  data: fntCharData,
  index: int,
  line: int,
};