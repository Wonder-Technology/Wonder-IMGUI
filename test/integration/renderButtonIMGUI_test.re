open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render button", () => {
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

    describe("test with io data", () => {
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
            (. _, apiJsObj, record) => {
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              let apiJsObj = Obj.magic(apiJsObj);
              let buttonFunc = apiJsObj##button;
              let (record, isButtonClick) =
                buttonFunc(.
                  (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                  str1,
                  record,
                );

              record;
            },
          ),
          record,
        );

      let _testButtonClick = (ioData, result) => {
        let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
          ButtonIMGUITool.buildButtonData1();
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ~bufferData, ())
          |> Obj.magic;
        let isClick = ref(false);
        let record =
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(),
               (. _, apiJsObj, record) => {
                 let apiJsObj = Obj.magic(apiJsObj);
                 let buttonFunc = apiJsObj##button;
                 let (record, isButtonClick) =
                   buttonFunc(.
                     (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                     str1,
                     record,
                   );

                 isClick := isButtonClick;

                 record;
               },
             );
        let record = RenderIMGUITool.prepareFntData(record);
        let record =
          ManageIMGUIAPI.init(gl, RenderIMGUITool.buildCanvasSize(), record);

        let record = ManageIMGUIAPI.render(gl, ioData, record);

        isClick^ |> expect == result;
      };

      describe("test buffer data", () =>
        describe("test draw box and draw text", () => {
          test("test position buffer data", () =>
            _test(
              [|
              35.,
              45.,
              35.,
              250.,
              140.,
              45.,
              140.,
              250.,
              46.,
              119.,
              46.,
              174.,
              91.,
              119.,
              91.,
              174.,
              89.,
              132.,
              89.,
              174.,
              133.,
              132.,
              133.,
              174.
              |],
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
                [|
                  u,
                  v,
                  u,
                  v,
                  u,
                  v,
                  u,
                  v,
                  0.1,
                  0.34552102376599636,
                  0.1,
                  0.4460694698354662,
                  0.19183673469387755,
                  0.34552102376599636,
                  0.19183673469387755,
                  0.4460694698354662,
                  0.8734693877551021,
                  0.7568555758683729,
                  0.8734693877551021,
                  0.8336380255941499,
                  0.963265306122449,
                  0.7568555758683729,
                  0.963265306122449,
                  0.8336380255941499,
                |],
                RenderIMGUITool.testTexCoordBufferData,
                record,
              );
            })
          );

          describe("test index buffer", () => {
            test("test one button", () =>
              _test(
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5, 8, 9, 10, 11, 10, 9|],
                RenderIMGUITool.testIndexBufferData,
                record^,
              )
            );
            test("test two buttons", () =>
              _testWithIMGUIFunc(
                [|
                  0,
                  1,
                  2,
                  3,
                  2,
                  1,
                  4,
                  5,
                  6,
                  7,
                  6,
                  5,
                  8,
                  9,
                  10,
                  11,
                  10,
                  9,
                  12,
                  13,
                  14,
                  15,
                  14,
                  13,
                  16,
                  17,
                  18,
                  19,
                  18,
                  17,
                  20,
                  21,
                  22,
                  23,
                  22,
                  21,
                |],
                (
                  RenderIMGUITool.testIndexBufferData,
                  (. _, apiJsObj, record) => {
                    let (
                      (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                      str1,
                    ) =
                      ButtonIMGUITool.buildButtonData1();
                    let (
                      (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                      str2,
                    ) =
                      ButtonIMGUITool.buildButtonData1();

                    let apiJsObj = Obj.magic(apiJsObj);
                    let buttonFunc = apiJsObj##button;
                    let (record, isButtonClick) =
                      buttonFunc(.
                        (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                        str1,
                        record,
                      );
                    let (record, isButtonClick) =
                      buttonFunc(.
                        (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                        str2,
                        record,
                      );

                    record;
                  },
                ),
                record^,
              )
            );
          });
        })
      );

      describe("if mouse hit button", () => {
        describe("if mouse not click or down", () => {
          describe("test color buffer data", () =>
            test("box color should be setting->hoverButtonColor", () => {
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              let record =
                SettingIMGUITool.setSetting(
                  ~record=record^,
                  ~hoverButtonColor=[|0.4, 0.2, 0.|],
                  (),
                );

              _test(
                [|
                  0.4,
                  0.2,
                  0.,
                  0.4,
                  0.2,
                  0.,
                  0.4,
                  0.2,
                  0.,
                  0.4,
                  0.2,
                  0.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                |],
                RenderIMGUITool.testColorBufferDataWithIOData(
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=false,
                    ~pointPosition=(buttonX1, buttonY1),
                    (),
                  ),
                ),
                record,
              );
            })
          );

          test("test button is not click", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            _testButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=false,
                ~pointDown=false,
                ~pointPosition=(buttonX1, buttonY1),
                (),
              ),
              false,
            );
          });
        });

        describe("if mouse down", () => {
          describe("test buffer data", () =>
            describe("test color buffer data", () =>
              test("box color should be setting->clickButtonColor", () => {
                let record =
                  SettingIMGUITool.setSetting(
                    ~record=record^,
                    ~clickButtonColor=[|0.1, 0.2, 0.5|],
                    (),
                  );
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                  ButtonIMGUITool.buildButtonData1();

                _test(
                  [|
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                  |],
                  RenderIMGUITool.testColorBufferDataWithIOData(
                    RenderIMGUITool.buildIOData(
                      ~pointUp=false,
                      ~pointDown=true,
                      ~pointPosition=(buttonX1, buttonY1),
                      (),
                    ),
                  ),
                  record,
                );
              })
            )
          );

          test("test button is not click", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            _testButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=false,
                ~pointDown=true,
                ~pointPosition=(buttonX1, buttonY1),
                (),
              ),
              false,
            );
          });
        });

        describe("if mouse click", () => {
          describe("test buffer data", () =>
            describe("test color buffer data", () =>
              test("box color should be setting->clickButtonColor", () => {
                let record =
                  SettingIMGUITool.setSetting(
                    ~record=record^,
                    ~clickButtonColor=[|0.1, 0.2, 0.5|],
                    (),
                  );
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                  ButtonIMGUITool.buildButtonData1();

                _test(
                  [|
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    0.1,
                    0.2,
                    0.5,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                    1.,
                  |],
                  RenderIMGUITool.testColorBufferDataWithIOData(
                    RenderIMGUITool.buildIOData(
                      ~pointUp=true,
                      ~pointDown=true,
                      ~pointPosition=(buttonX1, buttonY1),
                      (),
                    ),
                  ),
                  record,
                );
              })
            )
          );

          test("test button is click", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            _testButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=true,
                ~pointDown=true,
                ~pointPosition=(buttonX1, buttonY1),
                (),
              ),
              true,
            );
          });
        });
      });

      describe("else", () => {
        describe("test color buffer data", () =>
          test("box color should be setting->buttonColor", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            let record =
              SettingIMGUITool.setSetting(
                ~record=record^,
                ~buttonColor=[|0.4, 0.2, 0.|],
                (),
              );

            _test(
              [|
                0.4,
                0.2,
                0.,
                0.4,
                0.2,
                0.,
                0.4,
                0.2,
                0.,
                0.4,
                0.2,
                0.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
                1.,
              |],
              RenderIMGUITool.testColorBufferDataWithIOData(
                RenderIMGUITool.buildIOData(
                  ~pointUp=false,
                  ~pointDown=false,
                  ~pointPosition=(buttonX1 - 1, buttonY1),
                  (),
                ),
              ),
              record,
            );
          })
        );

        describe("test button is not click", () => {
          test("test1", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            _testButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=true,
                ~pointDown=false,
                ~pointPosition=(buttonX1 - 1, buttonY1),
                (),
              ),
              false,
            );
          });
          test("test2", () => {
            let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
              ButtonIMGUITool.buildButtonData1();

            _testButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=true,
                ~pointDown=true,
                ~pointPosition=(buttonX1, buttonY1 + buttonHeight1 + 1),
                (),
              ),
              false,
            );
          });
        });
      });
    });
  });