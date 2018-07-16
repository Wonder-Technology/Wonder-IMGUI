open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), (fntId, bitmapId), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath, fntId, bitmapId}),
};

/* let _loadImageByBlobPromise:
     Fetch.blob => Js.Promise.t(DomExtendType.imageElement) = [%raw
     blob => {|
       return new Promise((resolve, reject) => {
       var image = new Image();


       image.src = URL.createObjectURL(blob);


       image.onload = (function (image) {
         throw new Error("aaa")
           return resolve(image);
         });

       image.onerror = (function (e) {
         throw new Error("bbb")
         console.trace();
                 return reject(new Error(e));
               });
       });

       URL.revokeObjectURL( blob );
       |}
   ];







   let _loadImageByBlobStream = blob =>
     fromPromise(_loadImageByBlobPromise(blob)); */

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
  let {fntFilePath, bitmapFilePath, fntId, bitmapId} =
    RecordIMGUIService.getFontData(record);
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
  |> map(() => record);
};