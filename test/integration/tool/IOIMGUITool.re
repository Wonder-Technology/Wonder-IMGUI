let load = (fetchFunc, record) => FontIMGUIService.load(fetchFunc, record);

let parseFnt = (fntStr, fntFilePath) =>
  ParseFntIMGUIService.parse(fntStr, fntFilePath);

let buildFakeFetchTextResponse = text =>
  {"text": () => text} |> Js.Promise.resolve;

let buildFakeFetchBlobResponse = blob =>
  {"blob": () => blob} |> Js.Promise.resolve;

let buildFakeFetch = (sandbox, fntFilePath, bitmapFilePath) => {
  open Sinon;

  let fetch = createEmptyStubWithJsObjSandbox(sandbox);

  fetch
  |> onCall(1)
  |> returns(buildFakeFetchBlobResponse(bitmapFilePath))
  |> onCall(0)
  |> returns(
       buildFakeFetchTextResponse(Node.Fs.readFileAsUtf8Sync(fntFilePath)),
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
            window.loadImageBlob_wonder = function(objectUrl, resolve, reject){
                resolve(objectUrl)
            }

        |}
];