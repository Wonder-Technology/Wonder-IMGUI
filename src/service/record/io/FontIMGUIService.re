open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath}),
};

let initBitmap = (blob, {assetData} as record) => {
  let {bitmapId} = assetData;
  let {bitmapMap} = assetData;

  ImageService.loadImageByBlobPromise(blob |> Blob.createObjectURL)
  |> tap(image => Blob.revokeObjectURL(blob))
  |> map(image =>
       bitmapMap
       |> WonderCommonlib.MutableHashMapService.set(bitmapId, image)
       |> ignore
     );
};

let initFnt = (fntStr, {assetData} as record) => {
  let {fntId} = assetData;
  let {fntDataMap} = assetData;

  just(fntStr)
  |> map(fntStr => ParseFntIMGUIService.parse(fntStr))
  |> map(fntData =>
       fntDataMap
       |> WonderCommonlib.MutableHashMapService.set(fntId, fntData)
       |> ignore
     );
};

let load = (fetchFunc, handleWhenLoadingFunc, {assetData} as record) => {
  let {fntId, bitmapId} = assetData;
  let {fntFilePath, bitmapFilePath} =
    RecordIMGUIService.unsafeGetFontData(record);
  let {fntDataMap, bitmapMap} = assetData;

  fromPromise(
    fetchFunc(. bitmapFilePath)
    |> then_(response => {
         handleWhenLoadingFunc(
           FetchService.getContentLength(response),
           bitmapFilePath,
         );

         response |> resolve;
       })
    |> then_(Fetch.Response.blob),
  )
  |> flatMap(blob => initBitmap(blob, record))
  |> merge(
       fromPromise(
         fetchFunc(. fntFilePath)
         |> then_(response => {
              handleWhenLoadingFunc(
                FetchService.getContentLength(response),
                fntFilePath,
              );

              response |> resolve;
            })
         |> then_(Fetch.Response.text),
       )
       |> flatMap(fntStr => initFnt(fntStr, record)),
     )
  |> drain
  |> then_(() => record |> resolve);
};

module SetAsset = {
  let getBitmapData = record =>
    RecordAssetIMGUIService.getSettedAssetData(record).bitmapArrayBuffer;

  let unsafeGetBitmapData = record =>
    record |> getBitmapData |> OptionService.unsafeGet;

  let setBitmapData = (bitmapArrayBuffer, record) => {
    ...record,
    assetData: {
      ...record.assetData,
      settedAssetData: {
        ...RecordAssetIMGUIService.getSettedAssetData(record),
        bitmapArrayBuffer: Some(bitmapArrayBuffer),
      },
    },
  };

  let getFntData = record =>
    RecordAssetIMGUIService.getSettedAssetData(record).fntContent;

  let unsafeGetFntData = record =>
    record |> getFntData |> OptionService.unsafeGet;

  let setFntData = (content, record) => {
    ...record,
    assetData: {
      ...record.assetData,
      settedAssetData: {
        ...RecordAssetIMGUIService.getSettedAssetData(record),
        fntContent: Some(content),
      },
    },
  };
};