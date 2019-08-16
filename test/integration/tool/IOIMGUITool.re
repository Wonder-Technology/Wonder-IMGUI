let load =
    (customTextureSourceDataArr, (fetchFunc, handleWhenLoadingFunc), record) =>
  record
  |> AssetIMGUIService.LoadAsset.load(
       customTextureSourceDataArr,
       (fetchFunc, handleWhenLoadingFunc),
     );

let parseFnt = fntStr => ParseFntIMGUIService.parse(fntStr);

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
              createObjectURL: sandbox.stub(),
              revokeObjectURL: sandbox.stub()
            };

            window.URL = URL;
        |}
];

let setFakeCreateObjectURLReturnBlob = [%raw
  param => {|
            window.URL.createObjectURL = blob => blob;
  |}
];

let getFakeCreateObjectURL = [%raw
  param => {|
  return window.URL.createObjectURL
  |}
];

let buildFakeLoadImage = [%bs.raw
  param => {|
            window.loadImageBlob_wonder_imgui = function(objectUrl, resolve, reject){
                resolve(objectUrl)
            }

        |}
];

let buildFakeBlob = [%raw
  param => {|
var Blob = function(arrayBufferArr, param){
if( typeof window.blobData_wonder_forTest === "undefined"){
window.blobData_wonder_forTest = [
[arrayBufferArr[0], param]
];
} else{
window.blobData_wonder_forTest.push(
[arrayBufferArr[0], param]
);
}
};

window.Blob = Blob;
|}
];

let getBlobData = [%raw param => {|
return window.blobData_wonder_forTest;
|}];