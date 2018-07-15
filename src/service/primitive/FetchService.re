open Js.Promise;

open WonderBsMost.Most;

let createFetchBlobStream = (filePath, fetchFunc) =>
  fromPromise(fetchFunc(filePath) |> then_(Fetch.Response.blob));

let createFetchTextStream = (filePath, fetchFunc) =>
  fromPromise(fetchFunc(filePath) |> then_(Fetch.Response.text));