open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render radioButton", () => {
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

    describe("optimize", () => {
      let _prepare = () => {
        let (
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          str1,
        ) =
          RadioButtonIMGUITool.buildRadioButtonData1();
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ~bufferData, ())
          |> Obj.magic;
        let record =
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(),
               (_, apiJsObj, record) => {
                 let radioButtonFunc = apiJsObj##radioButton;
                 let (record, isRadioButtonClick) =
                   radioButtonFunc(.
                     (
                       radioButtonX1,
                       radioButtonY1,
                       radioButtonWidth1,
                       radioButtonHeight1,
                     ),
                     str1,
                     record,
                   );

                 record;
               },
             );
        let record = RenderIMGUITool.prepareFntData(record);
        let record =
          ManageIMGUIAPI.init(gl, RenderIMGUITool.buildCanvasSize(), record);

        (
          record,
          gl,
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          bufferData,
        );
      };

      test("if current color !== last color, update buffer data", () => {
        let (
          record,
          gl,
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          bufferData,
        ) =
          _prepare();
        let record =
          ManageIMGUIAPI.render(
            gl,
            RenderIMGUITool.buildIOData(
              ~pointUp=false,
              ~pointDown=false,
              ~pointPosition=(radioButtonX1, radioButtonY1),
              (),
            ),
            record,
          );
        let bufferDataCallCountAfterFirstRender = bufferData |> getCallCount;
        let record =
          ManageIMGUIAPI.render(
            gl,
            RenderIMGUITool.buildIOData(
              ~pointUp=false,
              ~pointDown=false,
              ~pointPosition=(
                radioButtonX1 + radioButtonWidth1 / 2,
                radioButtonY1 + radioButtonHeight1 / 2,
              ),
              (),
            ),
            record,
          );

        (bufferData |> getCallCount)
        - bufferDataCallCountAfterFirstRender
        |> expect > 0;
      });
      test("else, not", () => {
        let (
          record,
          gl,
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          bufferData,
        ) =
          _prepare();

        let record =
          ManageIMGUIAPI.render(
            gl,
            RenderIMGUITool.buildIOData(
              ~pointUp=false,
              ~pointDown=false,
              ~pointPosition=(radioButtonX1, radioButtonY1),
              (),
            ),
            record,
          );
        let bufferDataCallCountAfterFirstRender = bufferData |> getCallCount;
        let record =
          ManageIMGUIAPI.render(
            gl,
            RenderIMGUITool.buildIOData(
              ~pointUp=false,
              ~pointDown=false,
              ~pointPosition=(radioButtonX1, radioButtonY1),
              (),
            ),
            record,
          );

        (bufferData |> getCallCount)
        - bufferDataCallCountAfterFirstRender
        |> expect == 0;
      });
    });

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
            (_, apiJsObj, record) => {
              let (
                (
                  radioButtonX1,
                  radioButtonY1,
                  radioButtonWidth1,
                  radioButtonHeight1,
                ),
                str1,
              ) =
                RadioButtonIMGUITool.buildRadioButtonData1();

              let radioButtonFunc = apiJsObj##radioButton;
              let (record, isRadioButtonClick) =
                radioButtonFunc(.
                  (
                    radioButtonX1,
                    radioButtonY1,
                    radioButtonWidth1,
                    radioButtonHeight1,
                  ),
                  str1,
                  record,
                );

              record;
            },
          ),
          record,
        );

      let _testRadioButtonClick = (ioData, result) => {
        let (
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          str1,
        ) =
          RadioButtonIMGUITool.buildRadioButtonData1();
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
               (_, apiJsObj, record) => {
                 let radioButtonFunc = apiJsObj##radioButton;
                 let (record, isRadioButtonClick) =
                   radioButtonFunc(.
                     (
                       radioButtonX1,
                       radioButtonY1,
                       radioButtonWidth1,
                       radioButtonHeight1,
                     ),
                     str1,
                     record,
                   );

                 isClick := isRadioButtonClick;

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
        describe("test draw circle and draw text", () => {
          test("test position buffer data", () =>
            _test(
              [|
                137.5,
                147.5,
                240.,
                147.5,
                216.01955541969525,
                213.38572999287027,
                155.29893821086037,
                248.44279468375132,
                86.25000000000003,
                236.26760388790495,
                41.18150636944439,
                182.55706469088108,
                41.18150636944439,
                112.44293530911897,
                86.24999999999996,
                58.73239611209506,
                155.2989382108603,
                46.55720531624867,
                216.01955541969522,
                81.61427000712969,
                240.,
                147.49999999999997,
                244.,
                62.,
                244.,
                117.,
                289.,
                62.,
                289.,
                117.,
                287.,
                75.,
                287.,
                117.,
                331.,
                75.,
                331.,
                117.,
              |],
              RenderIMGUITool.testPositionBufferData,
              record^,
            )
          );

          describe("test texCoord buffer data", () =>
            test("texCoord data use setting->fontTexUvForWhite", () => {
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
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
                  0.1,
                  0.2,
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
            test("test one radioButton", () =>
              _test(
                [|
                  2,
                  1,
                  0,
                  3,
                  2,
                  0,
                  4,
                  3,
                  0,
                  5,
                  4,
                  0,
                  6,
                  5,
                  0,
                  7,
                  6,
                  0,
                  8,
                  7,
                  0,
                  9,
                  8,
                  0,
                  10,
                  9,
                  0,
                  11,
                  12,
                  13,
                  14,
                  13,
                  12,
                  15,
                  16,
                  17,
                  18,
                  17,
                  16,
                |],
                RenderIMGUITool.testIndexBufferData,
                record^,
              )
            );
            test("test two radioButtons", () =>
              _testWithIMGUIFunc(
                [|
                  2,
                  1,
                  0,
                  3,
                  2,
                  0,
                  4,
                  3,
                  0,
                  5,
                  4,
                  0,
                  6,
                  5,
                  0,
                  7,
                  6,
                  0,
                  8,
                  7,
                  0,
                  9,
                  8,
                  0,
                  10,
                  9,
                  0,
                  11,
                  12,
                  13,
                  14,
                  13,
                  12,
                  15,
                  16,
                  17,
                  18,
                  17,
                  16,
                  21,
                  20,
                  19,
                  22,
                  21,
                  19,
                  23,
                  22,
                  19,
                  24,
                  23,
                  19,
                  25,
                  24,
                  19,
                  26,
                  25,
                  19,
                  27,
                  26,
                  19,
                  28,
                  27,
                  19,
                  29,
                  28,
                  19,
                  30,
                  31,
                  32,
                  33,
                  32,
                  31,
                  34,
                  35,
                  36,
                  37,
                  36,
                  35,
                |],
                (
                  RenderIMGUITool.testIndexBufferData,
                  (_, apiJsObj, record) => {
                    let (
                      (
                        radioButtonX1,
                        radioButtonY1,
                        radioButtonWidth1,
                        radioButtonHeight1,
                      ),
                      str1,
                    ) =
                      RadioButtonIMGUITool.buildRadioButtonData1();
                    let (
                      (
                        radioButtonX2,
                        radioButtonY2,
                        radioButtonWidth2,
                        radioButtonHeight2,
                      ),
                      str2,
                    ) =
                      RadioButtonIMGUITool.buildRadioButtonData1();

                    let radioButtonFunc = apiJsObj##radioButton;
                    let (record, isRadioButtonClick) =
                      radioButtonFunc(.
                        (
                          radioButtonX1,
                          radioButtonY1,
                          radioButtonWidth1,
                          radioButtonHeight1,
                        ),
                        str1,
                        record,
                      );
                    let (record, isRadioButtonClick) =
                      radioButtonFunc(.
                        (
                          radioButtonX2,
                          radioButtonY2,
                          radioButtonWidth2,
                          radioButtonHeight2,
                        ),
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

      describe("if mouse hit radioButton", () => {
        let _buildHitPosition =
            (
              radioButtonX1,
              radioButtonY1,
              radioButtonWidth1,
              radioButtonHeight1,
            ) => (
          radioButtonX1 + radioButtonWidth1 / 2,
          radioButtonY1 + radioButtonHeight1 / 2,
        );

        describe("if mouse not click", () => {
          describe("test color buffer data", () =>
            test(
              "outterColor should be hover color and not draw inner circle", () => {
              let (
                (
                  radioButtonX1,
                  radioButtonY1,
                  radioButtonWidth1,
                  radioButtonHeight1,
                ),
                str1,
              ) =
                RadioButtonIMGUITool.buildRadioButtonData1();

              let record =
                SettingIMGUITool.setSetting(
                  ~record=record^,
                  ~radioButtonOuterColorHover=[|0., 0.2, 0.3|],
                  ~radioButtonInnerColorHover=[|0.5, 0.5, 0.3|],
                  (),
                );

              _test(
                [|
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
                  0.,
                  0.2,
                  0.3,
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
                    ~pointPosition=
                      _buildHitPosition(
                        radioButtonX1,
                        radioButtonY1,
                        radioButtonWidth1,
                        radioButtonHeight1,
                      ),
                    (),
                  ),
                ),
                record,
              );
            })
          );

          test("test radioButton is not click", () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData1();

            _testRadioButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=false,
                ~pointDown=false,
                ~pointPosition=
                  _buildHitPosition(
                    radioButtonX1,
                    radioButtonY1,
                    radioButtonWidth1,
                    radioButtonHeight1,
                  ),
                (),
              ),
              false,
            );
          });
        });

        describe("if mouse click", () => {
          describe("test buffer data", () =>
            describe("test color buffer data", () =>
              test(
                "innerColor and outterColor should be hover color and should draw inner circle",
                () => {
                  let record =
                    SettingIMGUITool.setSetting(
                      ~record=record^,
                      ~radioButtonOuterColorHover=[|0.1, 0.2, 0.3|],
                      ~radioButtonInnerColorHover=[|0.5, 0.5, 0.3|],
                      (),
                    );
                  let (
                    (
                      radioButtonX1,
                      radioButtonY1,
                      radioButtonWidth1,
                      radioButtonHeight1,
                    ),
                    str1,
                  ) =
                    RadioButtonIMGUITool.buildRadioButtonData1();

                  _test(
                    [|
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.1,
                      0.2,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
                      0.5,
                      0.5,
                      0.3,
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
                        ~pointPosition=
                          _buildHitPosition(
                            radioButtonX1,
                            radioButtonY1,
                            radioButtonWidth1,
                            radioButtonHeight1,
                          ),
                        (),
                      ),
                    ),
                    record,
                  );
                },
              )
            )
          );

          test("test radioButton is click", () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData1();

            _testRadioButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=true,
                ~pointDown=true,
                ~pointPosition=
                  _buildHitPosition(
                    radioButtonX1,
                    radioButtonY1,
                    radioButtonWidth1,
                    radioButtonHeight1,
                  ),
                (),
              ),
              true,
            );
          });
        });
      });

      describe("else", () => {
        let _buildNotHitPosition =
            (
              radioButtonX1,
              radioButtonY1,
              radioButtonWidth1,
              radioButtonHeight1,
            ) => (
          radioButtonX1,
          radioButtonY1,
        );

        describe("test color buffer data", () =>
          test(
            "outterColor should be radioButtonOuterColor and not draw inner circle",
            () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData1();

            let record =
              SettingIMGUITool.setSetting(
                ~record=record^,
                ~radioButtonOuterColor=[|0.1, 0.2, 0.3|],
                (),
              );

            _test(
              [|
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
                0.1,
                0.2,
                0.3,
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
                  ~pointPosition=(radioButtonX1 - 1, radioButtonY1),
                  (),
                ),
              ),
              record,
            );
          })
        );

        describe("test radioButton is not click", () =>
          test("test1", () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData1();

            _testRadioButtonClick(
              RenderIMGUITool.buildIOData(
                ~pointUp=true,
                ~pointDown=true,
                ~pointPosition=
                  _buildNotHitPosition(
                    radioButtonX1,
                    radioButtonY1,
                    radioButtonWidth1,
                    radioButtonHeight1,
                  ),
                (),
              ),
              false,
            );
          })
        );
      });
    });
  });