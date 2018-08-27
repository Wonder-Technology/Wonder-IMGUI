open Js.Promise;

open WonderBsMost.Most;

let createFetchBlobStream = (filePath, fetchFunc) =>
  fromPromise(fetchFunc(filePath) |> then_(Fetch.Response.blob));

let createFetchTextStream = (filePath, fetchFunc) =>
  fromPromise(fetchFunc(filePath) |> then_(Fetch.Response.text));

let fetch = (. filePath) => Fetch.fetch(filePath);

let _getContentLength: Fetch.Response.t => Js.Nullable.t(string) = [%raw
  response => {|
  return response.headers.get("content-length");
  |}
];

let getContentLength = response =>
  switch (_getContentLength(response) |> Js.toOption) {
  | None =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildErrorMessage(
        ~title="load",
        ~description={j|Content-Length response header unavailable|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  | Some(contentLength) => contentLength |> NumberService.convertStringToInt
  };