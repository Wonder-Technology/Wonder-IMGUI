open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath}),
};

let _loadBlobImage = [%raw
  (objectUrl, resolve, reject) => {|
          if (typeof window.loadImageBlob_wonder === "undefined") {
  window.loadImageBlob_wonder = function(objectUrl, resolve, reject){
                      var image = new Image();

                      image.src = objectUrl;

                      image.onload = (function () {
                          return resolve(image);
                        });

                      image.onerror = (function (e) {
                        console.trace();
                                return reject(new Error(e));
                              });
  };
          }

  window.loadImageBlob_wonder(objectUrl, resolve, reject)
    |}
];

let _loadImageByBlobPromise = objectUrl =>
  make((~resolve, ~reject) => _loadBlobImage(objectUrl, resolve, reject))
  |> fromPromise;

let load = (fetchFunc, {assetData} as record) => {
  let {fntId, bitmapId} = assetData;
  let {fntFilePath, bitmapFilePath} =
    RecordIMGUIService.unsafeGetFontData(record);
  let {fntDataMap, bitmapMap} = assetData;

  FetchService.createFetchBlobStream(bitmapFilePath, fetchFunc)
  |> flatMap(blob =>
       _loadImageByBlobPromise(blob |> Blob.createObjectURL)
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