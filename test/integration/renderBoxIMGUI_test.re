open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render box", () => {
    open Expect;
    open Expect.Operators;
    open Sinon;

    let sandbox = getSandboxDefaultVal();
    let record = ref(Obj.magic(0));

    beforeEach(() => {
      sandbox := createSandbox();
      record := ManageIMGUIAPI.createRecord() |> AssetTool.prepareFontAsset;
    });
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    describe("test buffer data", () => {
      let _testWithIMGUIFunc =
          (bufferData, (testBufferDataFunc, imguiFunc), record) => {
        let record = RenderIMGUITool.prepareFntData(record);

        testBufferDataFunc(sandbox, record, imguiFunc, bufferData);
      };

      let _test = (bufferData, testBufferDataFunc, record) =>
        _testWithIMGUIFunc(
          bufferData,
          (
            testBufferDataFunc,
            (_, apiJsObj, record) => {
              let ((boxX1, boxY1, boxWidth1, boxHeight1), color) =
                BoxIMGUITool.buildBoxData1();

              let apiJsObj = Obj.magic(apiJsObj);
              let boxFunc = apiJsObj##box;

              let record =
                boxFunc(.
                  (boxX1, boxY1, boxWidth1, boxHeight1),
                  color,
                  record,
                );

              record;
            },
          ),
          record,
        );

      test("test position buffer data", () =>
        _test(
          [|38., 48., 38., 253., 143., 48., 143., 253.|],
          RenderIMGUITool.testPositionBufferData,
          record^,
        )
      );

      describe("test texCoord buffer data", () =>
        test("box texCoord data use setting->fontTexUvForWhite", () => {
          let u = 0.1;
          let v = 0.2;
          let record =
            SettingIMGUITool.setSetting(
              ~record=record^,
              ~fontTexUvForWhite=[|u, v|],
              (),
            );

          _test(
            [|u, v, u, v, u, v, u, v|],
            RenderIMGUITool.testTexCoordBufferData,
            record,
          );
        })
      );
    });
  });