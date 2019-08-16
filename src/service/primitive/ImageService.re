open ImageType;

open WonderBsMost.Most;

open Js.Promise;

let getType = src =>
  switch (PathService.extname(src)) {
  | None =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="getType",
        ~description={j|image extname should exist|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  | Some(".jpg") => Jpg
  | Some(".png") => Png
  | _ => Unknown
  };

let convertMimeTypeToImageType = mimeType =>
  switch (mimeType) {
  | "image/png" => Png
  | "image/jpeg" => Jpg
  | _ => Unknown
  };

let _loadBlobImage = [%raw
  (objectUrl, resolve, reject) => {|
          if (typeof window.loadImageBlob_wonder_imgui === "undefined") {
  window.loadImageBlob_wonder_imgui = function(objectUrl, resolve, reject){
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

  window.loadImageBlob_wonder_imgui(objectUrl, resolve, reject)
    |}
];

let loadImageByBlobPromise = objectUrl =>
  make((~resolve, ~reject) => _loadBlobImage(objectUrl, resolve, reject))
  |> fromPromise;