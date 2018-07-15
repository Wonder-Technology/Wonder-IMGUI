open IMGUIType;

open WonderBsMost.Most;

open Js.Promise;

let addFont = ((fntFilePath, bitmapFilePath), (fntId, bitmapId), record) => {
  ...record,
  fontData: Some({fntFilePath, bitmapFilePath, fntId, bitmapId}),
};

let _fetch = filePath => Fetch.fetch(filePath);

let _loadImageByBlobPromise:
  Fetch.blob => Js.Promise.t(DomExtendType.imageElement) = [%raw
  blob => {|
    return new Promise((resolve, reject) => {
    var image = new Image();

    image.src = URL.createObjectURL(blob);

    URL.revokeObjectURL( blob );

    image.onload = (function () {
        return resolve(image);
      });

    image.onerror = (function (e) {
      console.trace();
              return reject(new Error(e));
            });
    })
    |}
];

let _loadImageByBlobStream = blob =>
  fromPromise(_loadImageByBlobPromise(blob));

let load = ({assetData} as record) => {
  let {fntFilePath, bitmapFilePath, fntId, bitmapId} =
    RecordIMGUIService.getFontData(record);
  let {fntDataMap, bitmapMap} = assetData;

  FetchService.createFetchBlobStream(bitmapFilePath, _fetch)
  |> flatMap(blob => _loadImageByBlobStream(blob))
  |> map(image => {
       bitmapMap
       |> WonderCommonlib.HashMapService.set(bitmapId, image)
       |> ignore;
       ();
     })
  |> merge(
       FetchService.createFetchTextStream(fntFilePath, _fetch)
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