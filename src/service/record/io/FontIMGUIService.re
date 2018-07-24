open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath}),
};

let load = (fetchFunc, {assetData} as record) => {
  let {fntId, bitmapId} = assetData;
  let {fntFilePath, bitmapFilePath} =
    RecordIMGUIService.unsafeGetFontData(record);
  let {fntDataMap, bitmapMap} = assetData;

  FetchService.createFetchBlobStream(bitmapFilePath, fetchFunc)
  |> flatMap(blob =>
       ImageService.loadImageByBlobPromise(blob |> Blob.createObjectURL)
       |> tap(image => Blob.revokeObjectURL(blob))
     )
  |> map(image => {
       bitmapMap
       |> WonderCommonlib.HashMapService.set(bitmapId, image)
       |> ignore;
       ();
     })
  |> merge(
       FetchService.createFetchTextStream(fntFilePath, fetchFunc)
       |> map(fntStr => ParseFntIMGUIService.parse(fntStr, fntFilePath))
       |> map(fntData => {
            fntDataMap
            |> WonderCommonlib.HashMapService.set(fntId, fntData)
            |> ignore;
            ();
          }),
     )
  |> drain
  |> then_(() => record |> resolve);
};