open FontType;

let hasFontDefDictionaryData = ({fontDefDictionary}) =>
  fontDefDictionary |> WonderCommonlib.MutableSparseMapService.length > 0;