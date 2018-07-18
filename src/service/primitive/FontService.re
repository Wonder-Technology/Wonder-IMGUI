open FontType;

let hasFontDefDictionaryData = ({fontDefDictionary}) =>
  fontDefDictionary |> SparseMapService.length > 0;