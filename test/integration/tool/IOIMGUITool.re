let load =
    (customTextureSourceDataArr, (fetchFunc, handleWhenLoadingFunc), record) =>
  record
  |> AssetIMGUIService.load(
       customTextureSourceDataArr,
       (fetchFunc, handleWhenLoadingFunc),
     );

let parseFnt = (fntStr, fntFilePath) =>
  ParseFntIMGUIService.parse(fntStr, fntFilePath);

let buildFakeFetchTextResponse = (sandbox, contentLength, text) =>
  {
    "headers": {
      "get":
        Sinon.createEmptyStubWithJsObjSandbox(sandbox)
        |> Sinon.withOneArg("content-length")
        |> Sinon.returns(contentLength),
    },
    "text": () => text,
  }
  |> Js.Promise.resolve;

let buildFakeFetchBlobResponse = (sandbox, contentLength, blob) =>
  {
    "headers": {
      "get":
        Sinon.createEmptyStubWithJsObjSandbox(sandbox)
        |> Sinon.withOneArg("content-length")
        |> Sinon.returns(contentLength),
    },
    "blob": () => blob,
  }
  |> Js.Promise.resolve;

let buildFakeFetch =
    (
      sandbox,
      fntFilePath,
      bitmapFilePath,
      (
        customTextureSource1ContentLength,
        customTextureSource2ContentLength,
        bitmapFileContentLength,
        fntFileContentLength,
      ),
      customTextureSourceSrc1,
      customTextureSourceSrc2,
    ) => {
  open Sinon;

  let fetch = createEmptyStubWithJsObjSandbox(sandbox);

  fetch
  |> onCall(3)
  |> returns(
       buildFakeFetchBlobResponse(
         sandbox,
         customTextureSource2ContentLength,
         customTextureSourceSrc2,
       ),
     )
  |> onCall(2)
  |> returns(
       buildFakeFetchBlobResponse(
         sandbox,
         customTextureSource1ContentLength,
         customTextureSourceSrc1,
       ),
     )
  |> onCall(1)
  |> returns(
       buildFakeFetchBlobResponse(
         sandbox,
         bitmapFileContentLength,
         bitmapFilePath,
       ),
     )
  |> onCall(0)
  |> returns(
       buildFakeFetchTextResponse(
         sandbox,
         fntFileContentLength,
         Node.Fs.readFileAsUtf8Sync(fntFilePath),
       ),
     );

  fetch;
};

let buildFakeURL = [%raw
  sandbox => {|
            var URL = {
              createObjectURL: (blob) => blob,
              revokeObjectURL: sandbox.stub()
            };

            window.URL = URL;
        |}
];

let buildFakeLoadImage = [%bs.raw
  () => {|
            window.loadImageBlob_wonder_imgui = function(objectUrl, resolve, reject){
                resolve(objectUrl)
            }

        |}
];