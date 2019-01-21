open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath}),
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
  |> flatMap(blob =>
       ImageService.loadImageByBlobPromise(blob |> Blob.createObjectURL)
       |> tap(image => Blob.revokeObjectURL(blob))
     )
  |> map(image => {
       bitmapMap
       |> WonderCommonlib.MutableHashMapService.set(bitmapId, image)
       |> ignore;
       ();
     })
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
       |> map(fntStr => ParseFntIMGUIService.parse(fntStr, fntFilePath))
       |> map(fntData => {
            fntDataMap
            |> WonderCommonlib.MutableHashMapService.set(fntId, fntData)
            |> ignore;
            ();
          }),
     )
  |> drain
  |> then_(() => record |> resolve);
};