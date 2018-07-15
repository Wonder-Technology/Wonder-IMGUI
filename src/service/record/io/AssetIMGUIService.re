open IMGUIType;

let getFntData = ({assetData} as record) => {
  let {fntDataMap} = assetData;
  let fontData = RecordIMGUIService.getFontData(record);

  fntDataMap |> WonderCommonlib.HashMapService.get(fontData.fntId);
};

let unsafeGetFntData = record =>
  getFntData(record) |> OptionService.unsafeGet;

let unsafeGetBitmap = ({assetData} as record) => {
  let {bitmapMap} = assetData;
  let fontData = RecordIMGUIService.getFontData(record);

  bitmapMap |> WonderCommonlib.HashMapService.unsafeGet(fontData.bitmapId);
};