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
          let customTextureSourceSrc1 = "./test/res/image/1.jpg";
          let customTextureSourceSrc2 = "./test/res/image/2.png";
          let customTextureSourceDataArr = [|
            (customTextureSourceSrc1, "a1"),
            (customTextureSourceSrc2, "a2"),
          |];
          let customImageArr = [|
            (customTextureSourceSrc1, "a1", ImageType.Jpg),
            (customTextureSourceSrc2, "a2", ImageType.Png),
          |];
          IOIMGUITool.buildFakeURL(sandbox^);
          IOIMGUITool.buildFakeLoadImage(.);
          let fetch =
            IOIMGUITool.buildFakeFetch(
              sandbox,
              fntFilePath,
              bitmapFilePath,
              customTextureSourceSrc1,
              customTextureSourceSrc2,
            );

          record^
          |> IOIMGUIAPI.addFont(fntFilePath, bitmapFilePath)
          |> IOIMGUITool.load(customTextureSourceDataArr, fetch)
          |> then_(record => testFunc(bitmap, customImageArr, record));
        };

        testPromise("load bitmap image", () =>
          _test((bitmap, _, record) =>
            AssetTool.unsafeGetBitmap(record)
            |> Obj.magic
            |> expect == bitmap
            |> resolve
          )
        );

        testPromise("load custom texture sources", () =>
          _test((bitmap, customImageArr, record) =>
            AssetIMGUIAPI.getCustomImageArr(record)
            |> Obj.magic
            |> expect == customImageArr
            |> resolve
          )
        );

        describe("load fnt data", () =>
          describe("parse data", () =>
            testPromise("test", () =>
              _test((_bitmap, _, record) => {
                let {
                  commonHeight,
                  commonBase,
                  scaleW,
                  scaleH,
                  fontDefDictionary,
                  kerningMap,
                } =
                  AssetTool.unsafeGetFntData(record);

                (
                  commonHeight,
                  commonBase,
                  scaleW,
                  scaleH,
                  fontDefDictionary |> SparseMapService.length,
                  (fontDefDictionary |> SparseMapService.getValidValues)[0],
                  kerningMap
                  |> SparseMapService.getValidValues
                  |> SparseMapService.length,
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
                          )
                |> resolve;
              })
            )
          )
        );
      })
    );
  });