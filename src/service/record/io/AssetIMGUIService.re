open IMGUIType;

let getFntData = ({assetData} as record) => {
  let {fntDataMap} = assetData;
  let fontData = RecordIMGUIService.unsafeGetFontData(record);

  fntDataMap |> WonderCommonlib.HashMapService.get(fontData.fntId);
};

let unsafeGetFntData = record =>
  getFntData(record) |> OptionService.unsafeGet;

let getBitmap = ({assetData} as record) => {
  let {bitmapMap} = assetData;
  let fontData = RecordIMGUIService.unsafeGetFontData(record);

  bitmapMap |> WonderCommonlib.HashMapService.get(fontData.bitmapId);
};

let unsafeGetBitmap = record => getBitmap(record) |> OptionService.unsafeGet;

let isLoadAsset = record =>
  RecordIMGUIService.getFontData(record)
  |> Js.Option.isSome
  && getBitmap(record)
  |> Js.Option.isSome;