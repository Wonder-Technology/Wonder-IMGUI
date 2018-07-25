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

    describe("test with io data", () => {
      let _buildNotHitPosition =
          (radioButtonX, radioButtonY, radioButtonWidth, radioButtonHeight) => (
        radioButtonX,
        radioButtonY,
      );

      let _buildHitPosition =
          (radioButtonX, radioButtonY, radioButtonWidth, radioButtonHeight) => (
        radioButtonX + radioButtonWidth / 2,
        radioButtonY + radioButtonHeight / 2,
      );

      let _buildIMGUIFuncOfTwoRadioButtons =
          (apiJsObj, defaultSelectIndex, record) => {
        let (
          (
            radioButtonX1,
            radioButtonY1,
            radioButtonWidth1,
            radioButtonHeight1,
          ),
          str1,
          group1,
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
          group2,
        ) =
          RadioButtonIMGUITool.buildRadioButtonData2();

        let apiJsObj = Obj.magic(apiJsObj);
        let radioButtonFunc = apiJsObj##radioButton;

        let (record, clickRadioButtonIndex) =
          radioButtonFunc(.
            [|
              (
                (
                  radioButtonX1,
                  radioButtonY1,
                  radioButtonWidth1,
                  radioButtonHeight1,
                ),
                str1,
              ),
              (
                (
                  radioButtonX2,
                  radioButtonY2,
                  radioButtonWidth2,
                  radioButtonHeight2,
                ),
                str2,
              ),
            |],
            defaultSelectIndex,
            group1,
            record,
          );

        (record, clickRadioButtonIndex);
      };

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
              let (
                (
                  radioButtonX1,
                  radioButtonY1,
                  radioButtonWidth1,
                  radioButtonHeight1,
                ),
                str1,
                group1,
              ) =
                RadioButtonIMGUITool.buildRadioButtonData1();

              let apiJsObj = Obj.magic(apiJsObj);
              let radioButtonFunc = apiJsObj##radioButton;
              let (record, clickRadioButtonIndex) =
                radioButtonFunc(.
                  [|
                    (
                      (
                        radioButtonX1,
                        radioButtonY1,
                        radioButtonWidth1,
                        radioButtonHeight1,
                      ),
                      str1,
                    ),
                  |],
                  0,
                  group1,
                  record,
                );

              record;
            },
          ),
          record,
        );

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
                137.5,
                147.5,
                199.,
                147.5,
                184.61173325181716,
                187.03143799572217,
                148.17936292651623,
                208.0656768102508,
                106.75000000000001,
                200.76056233274298,
                79.70890382166664,
                168.53423881452863,
                79.70890382166664,
                126.46576118547138,
                106.74999999999997,
                94.23943766725704,
                148.1793629265162,
                86.9343231897492,
                184.61173325181713,
                107.96856200427781,
                199.,
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
                  13,
                  12,
                  11,
                  14,
                  13,
                  11,
                  15,
                  14,
                  11,
                  16,
                  15,
                  11,
                  17,
                  16,
                  11,
                  18,
                  17,
                  11,
                  19,
                  18,
                  11,
                  20,
                  19,
                  11,
                  21,
                  20,
                  11,
                  22,
                  23,
                  24,
                  25,
                  24,
                  23,
                  26,
                  27,
                  28,
                  29,
                  28,
                  27,
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
                  13,
                  12,
                  11,
                  14,
                  13,
                  11,
                  15,
                  14,
                  11,
                  16,
                  15,
                  11,
                  17,
                  16,
                  11,
                  18,
                  17,
                  11,
                  19,
                  18,
                  11,
                  20,
                  19,
                  11,
                  21,
                  20,
                  11,
                  22,
                  23,
                  24,
                  25,
                  24,
                  23,
                  26,
                  27,
                  28,
                  29,
                  28,
                  27,
                  32,
                  31,
                  30,
                  33,
                  32,
                  30,
                  34,
                  33,
                  30,
                  35,
                  34,
                  30,
                  36,
                  35,
                  30,
                  37,
                  36,
                  30,
                  38,
                  37,
                  30,
                  39,
                  38,
                  30,
                  40,
                  39,
                  30,
                  43,
                  42,
                  41,
                  44,
                  43,
                  41,
                  45,
                  44,
                  41,
                  46,
                  45,
                  41,
                  47,
                  46,
                  41,
                  48,
                  47,
                  41,
                  49,
                  48,
                  41,
                  50,
                  49,
                  41,
                  51,
                  50,
                  41,
                  52,
                  53,
                  54,
                  55,
                  54,
                  53,
                  56,
                  57,
                  58,
                  59,
                  58,
                  57,
                |],
                (
                  RenderIMGUITool.testIndexBufferData,
                  (. _, apiJsObj, record) => {
                    let (
                      (
                        radioButtonX1,
                        radioButtonY1,
                        radioButtonWidth1,
                        radioButtonHeight1,
                      ),
                      str1,
                      group1,
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
                      group2,
                    ) =
                      RadioButtonIMGUITool.buildRadioButtonData1();

                    let apiJsObj = Obj.magic(apiJsObj);
                    let radioButtonFunc = apiJsObj##radioButton;

                    let (record, _) =
                      radioButtonFunc(.
                        [|
                          (
                            (
                              radioButtonX1,
                              radioButtonY1,
                              radioButtonWidth1,
                              radioButtonHeight1,
                            ),
                            str1,
                          ),
                        |],
                        0,
                        group1,
                        record,
                      );
                    let (record, _) =
                      radioButtonFunc(.
                        [|
                          (
                            (
                              radioButtonX2,
                              radioButtonY2,
                              radioButtonWidth2,
                              radioButtonHeight2,
                            ),
                            str2,
                          ),
                        |],
                        0,
                        group2,
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
        describe("if mouse not click", () =>
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
                group1,
              ) =
                RadioButtonIMGUITool.buildRadioButtonData1();

              let record =
                SettingIMGUITool.setSetting(
                  ~record=record^,
                  ~radioButtonOuterColorHover=[|0.1, 0.2, 0.3|],
                  ~radioButtonInnerColorHover=[|0.5, 0.5, 0.3|],
                  (),
                );

              _testWithIMGUIFunc(
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
                |],
                (
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
                  (. _, apiJsObj, record) => {
                    let (record, clickRadioButtonIndex) =
                      _buildIMGUIFuncOfTwoRadioButtons(apiJsObj, 1, record);

                    record;
                  },
                ),
                record,
              );
            })
          )
        );
        /* test("test radioButton is not click", () => {
             let (
               (
                 radioButtonX1,
                 radioButtonY1,
                 radioButtonWidth1,
                 radioButtonHeight1,
               ),
               str1,
               group1,
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
           }); */

        describe("if mouse click", () =>
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
                    group1,
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
          )
        );
        /* test("test radioButton is click", () => {
             let (
               (
                 radioButtonX1,
                 radioButtonY1,
                 radioButtonWidth1,
                 radioButtonHeight1,
               ),
               str1,
               group1,
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
           }); */
      });

      describe("else", () =>
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
              group1,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData1();

            let record =
              SettingIMGUITool.setSetting(
                ~record=record^,
                ~radioButtonOuterColor=[|0.1, 0.2, 0.3|],
                (),
              );

            _testWithIMGUIFunc(
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
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
                0.5,
                0.3,
                1.,
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
              |],
              (
                RenderIMGUITool.testColorBufferDataWithIOData(
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=false,
                    ~pointPosition=(radioButtonX1 - 1, radioButtonY1),
                    (),
                  ),
                ),
                (. _, apiJsObj, record) => {
                  let (
                    (
                      radioButtonX1,
                      radioButtonY1,
                      radioButtonWidth1,
                      radioButtonHeight1,
                    ),
                    str1,
                    group1,
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
                    group2,
                  ) =
                    RadioButtonIMGUITool.buildRadioButtonData2();

                  let apiJsObj = Obj.magic(apiJsObj);
                  let radioButtonFunc = apiJsObj##radioButton;
                  let (record, clickRadioButtonIndex) =
                    radioButtonFunc(.
                      [|
                        (
                          (
                            radioButtonX1,
                            radioButtonY1,
                            radioButtonWidth1,
                            radioButtonHeight1,
                          ),
                          str1,
                        ),
                        (
                          (
                            radioButtonX2,
                            radioButtonY2,
                            radioButtonWidth2,
                            radioButtonHeight2,
                          ),
                          str2,
                        ),
                      |],
                      1,
                      group1,
                      record,
                    );

                  record;
                },
              ),
              record,
            );
          })
        )
      );
      /* describe("test radioButton is not click", () =>
           test("test1", () => {
             let (
               (
                 radioButtonX1,
                 radioButtonY1,
                 radioButtonWidth1,
                 radioButtonHeight1,
               ),
               str1,
               group1,
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
         ); */

      describe("test group", () => {
        describe("test defaultSelectIndex is second one", () => {
          let _buildIMGUIFunc = (apiJsObj, record) =>
            _buildIMGUIFuncOfTwoRadioButtons(apiJsObj, 1, record);

          test("should select second one", () => {
            let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
            let gl =
              FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;
            let clickIndex = ref(None);
            let record =
              record^
              |> ManageIMGUIAPI.setIMGUIFunc(
                   RenderIMGUITool.buildCustomData(),
                   (. _, apiJsObj, record) => {
                     let (record, clickRadioButtonIndex) =
                       _buildIMGUIFunc(apiJsObj, record);

                     clickIndex := clickRadioButtonIndex;

                     record;
                   },
                 );
            let record = RenderIMGUITool.prepareFntData(record);
            let record =
              ManageIMGUIAPI.init(
                gl,
                RenderIMGUITool.buildCanvasSize(),
                record,
              );

            let record =
              ManageIMGUIAPI.render(
                gl,
                RenderIMGUITool.buildIOData(
                  ~pointUp=false,
                  ~pointDown=false,
                  ~pointPosition=(0, 0),
                  (),
                ),
                record,
              );

            clickIndex^ |> expect == Some(1);
          });
          test("test position buffer data", () =>
            _testWithIMGUIFunc(
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
                359.,
                459.,
                412.,
                459.,
                399.60035548530584,
                493.0677433133866,
                368.20335341634734,
                511.194810909647,
                332.5,
                504.89934640057527,
                309.1962910983469,
                477.1270675962605,
                309.1962910983469,
                440.8729324037396,
                332.5,
                413.10065359942473,
                368.2033534163473,
                406.805189090353,
                399.60035548530584,
                424.9322566866134,
                412.,
                459.,
                358.8,
                458.8,
                390.6,
                458.8,
                383.1602132911835,
                479.24064598803193,
                364.3220120498084,
                490.11688654578825,
                342.90000000000003,
                486.33960784034514,
                328.91777465900816,
                469.6762405577563,
                328.9177746590081,
                447.92375944224375,
                342.9,
                431.2603921596549,
                364.3220120498084,
                427.4831134542118,
                383.1602132911835,
                438.35935401196804,
                390.6,
                458.8,
                414.,
                423.,
                414.,
                478.,
                447.,
                423.,
                447.,
                478.,
              |],
              (
                RenderIMGUITool.testPositionBufferData,
                (. _, apiJsObj, record) => {
                  let (record, _) = _buildIMGUIFunc(apiJsObj, record);

                  record;
                },
              ),
              record^,
            )
          );
        });

        test(
          {|test click first radio in first step; then not click any group radios in second step;
        should select first radio|},
          () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
              group1,
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
              group2,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData2();
            let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
            let gl =
              FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;
            let clickIndex = ref(None);
            let record =
              record^
              |> ManageIMGUIAPI.setIMGUIFunc(
                   RenderIMGUITool.buildCustomData(),
                   (. _, apiJsObj, record) => {
                     let apiJsObj = Obj.magic(apiJsObj);
                     let radioButtonFunc = apiJsObj##radioButton;
                     let (record, clickRadioButtonIndex) =
                       radioButtonFunc(.
                         [|
                           (
                             (
                               radioButtonX1,
                               radioButtonY1,
                               radioButtonWidth1,
                               radioButtonHeight1,
                             ),
                             str1,
                           ),
                           (
                             (
                               radioButtonX2,
                               radioButtonY2,
                               radioButtonWidth2,
                               radioButtonHeight2,
                             ),
                             str2,
                           ),
                         |],
                         1,
                         group1,
                         record,
                       );

                     clickIndex := clickRadioButtonIndex;

                     record;
                   },
                 );
            let record = RenderIMGUITool.prepareFntData(record);
            let record =
              ManageIMGUIAPI.init(
                gl,
                RenderIMGUITool.buildCanvasSize(),
                record,
              );

            let record =
              ManageIMGUIAPI.render(
                gl,
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
                record,
              );

            let record =
              ManageIMGUIAPI.render(
                gl,
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
                record,
              );

            clickIndex^ |> expect == Some(0);
          },
        );

        test(
          {|test click first radio in first step; then click second radio in the same group in second step;
        should select second radio|},
          () => {
            let (
              (
                radioButtonX1,
                radioButtonY1,
                radioButtonWidth1,
                radioButtonHeight1,
              ),
              str1,
              group1,
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
              group2,
            ) =
              RadioButtonIMGUITool.buildRadioButtonData2();
            let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
            let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
            let gl =
              FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ~bufferData, ())
              |> Obj.magic;
            let clickIndex = ref(None);
            let record =
              record^
              |> ManageIMGUIAPI.setIMGUIFunc(
                   RenderIMGUITool.buildCustomData(),
                   (. _, apiJsObj, record) => {
                     let apiJsObj = Obj.magic(apiJsObj);
                     let radioButtonFunc = apiJsObj##radioButton;
                     let (record, clickRadioButtonIndex) =
                       radioButtonFunc(.
                         [|
                           (
                             (
                               radioButtonX1,
                               radioButtonY1,
                               radioButtonWidth1,
                               radioButtonHeight1,
                             ),
                             str1,
                           ),
                           (
                             (
                               radioButtonX2,
                               radioButtonY2,
                               radioButtonWidth2,
                               radioButtonHeight2,
                             ),
                             str2,
                           ),
                         |],
                         1,
                         group1,
                         record,
                       );

                     clickIndex := clickRadioButtonIndex;

                     record;
                   },
                 );
            let record = RenderIMGUITool.prepareFntData(record);
            let record =
              ManageIMGUIAPI.init(
                gl,
                RenderIMGUITool.buildCanvasSize(),
                record,
              );

            let record =
              ManageIMGUIAPI.render(
                gl,
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
                record,
              );

            let record =
              ManageIMGUIAPI.render(
                gl,
                RenderIMGUITool.buildIOData(
                  ~pointUp=true,
                  ~pointDown=true,
                  ~pointPosition=
                    _buildHitPosition(
                      radioButtonX2,
                      radioButtonY2,
                      radioButtonWidth2,
                      radioButtonHeight2,
                    ),
                  (),
                ),
                record,
              );

            clickIndex^ |> expect == Some(1);
          },
        );
      });
    });
  });