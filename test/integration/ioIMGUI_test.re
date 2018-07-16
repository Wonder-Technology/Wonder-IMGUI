open Wonder_jest;

open Js.Typed_array;

open WonderBsMost;

open Js.Promise;

open IMGUIType;

open FontType;

let _ =
  describe("ioIMGUI", () => {
    open Expect;
    open Expect.Operators;
    open Sinon;

    let sandbox = getSandboxDefaultVal();

    let record = ref(Obj.magic(0));

    beforeEach(() => {
      sandbox := createSandbox();
      record := ManageIMGUIAPI.createRecord();
    });
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    describe("load", () =>
      describe("load font asset", () => {
        let _test = testFunc => {
          let fntFilePath =
            Node.Path.join([|
              Node.Process.cwd(),
              "./test/res/font/myFont.fnt",
            |]);
          let bitmapFilePath =
            Node.Path.join([|
              Node.Process.cwd(),
              "./test/res/font/myFont.png",
            |]);
          let bitmap = bitmapFilePath;
          IOIMGUITool.buildFakeURL(sandbox^);
          IOIMGUITool.buildFakeLoadImage(.);
          let fetch =
            IOIMGUITool.buildFakeFetch(sandbox, fntFilePath, bitmapFilePath);

          record^
          |> IOIMGUIAPI.addFont(fntFilePath, bitmapFilePath)
          |> IOIMGUITool.load(fetch)
          |> then_(record => testFunc(bitmap, record));
        };

        testPromise("load bitmap image", () =>
          _test((bitmap, record) =>
            AssetTool.unsafeGetBitmap(record)
            |> Obj.magic
            |> expect == bitmap
            |> resolve
          )
        );

        describe("load fnt data", () =>
          describe("parse data", () =>
            testPromise("test", () =>
              _test((_bitmap, record) => {
                let {
                  commonHeight,
                  commonBase,
                  scaleW,
                  scaleH,
                  fontDefDictionary,
                  kerningArray,
                } =
                  AssetTool.unsafeGetFntData(record);

                (
                  commonHeight,
                  commonBase,
                  scaleW,
                  scaleH,
                  fontDefDictionary |> SparseMapService.length,
                  (fontDefDictionary |> SparseMapService.getValidValues)[0],
                  kerningArray |> Js.Array.length,
                  kerningArray[0],
                )
                |>
                expect == (
                            90,
                            70,
                            490,
                            547,
                            127,
                            {
                              id: 32,
                              rect: (486, 459, 0, 0),
                              xOffset: 0,
                              yOffset: 71,
                              xAdvance: 21,
                            },
                            241,
                            {first: 89, second: 112, amount: (-3)},
                          )
                |> resolve;
              })
            )
          )
        );
      })
    );
  });