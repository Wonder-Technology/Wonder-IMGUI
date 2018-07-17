open IMGUIType;

let getFntData = ({assetData} as record) => {
  let {fntDataMap} = assetData;

  fntDataMap |> WonderCommonlib.HashMapService.get(assetData.fntId);
};

let unsafeGetFntData = record =>
  getFntData(record) |> OptionService.unsafeGet;

let setFntData = (fntData, {assetData} as record) => {
  let {fntDataMap} = assetData;

  fntDataMap
  |> WonderCommonlib.HashMapService.set(assetData.fntId, fntData)
  |> ignore;

  record;
};

let getBitmap = ({assetData} as record) => {
  let {bitmapMap} = assetData;

  bitmapMap |> WonderCommonlib.HashMapService.get(assetData.bitmapId);
};

let unsafeGetBitmap = record => getBitmap(record) |> OptionService.unsafeGet;

let setBitmap = (bitmap, {assetData} as record) => {
  let {bitmapMap} = assetData;

  bitmapMap
  |> WonderCommonlib.HashMapService.set(assetData.bitmapId, bitmap)
  |> ignore;

  record;
};

let isLoadAsset = record =>
  /* RecordIMGUIService.getFontData(record)
     |> Js.Option.isSome */
  getBitmap(record) |> Js.Option.isSome;