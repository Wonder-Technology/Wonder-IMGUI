open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render slider", () => {
    open Expect;
    open Expect.Operators;
    open Sinon;

    let sandbox = getSandboxDefaultVal();
    let record = ref(Obj.magic(0));

    let _buildNotHitPosition = (sliderX, sliderY, sliderWidth, sliderHeight) => (
      sliderX - 1,
      sliderY,
    );

    let _buildHitPosition = (sliderX, sliderY, sliderWidth, sliderHeight) => (
      sliderX,
      sliderY,
    );

    beforeEach(() => {
      sandbox := createSandbox();
      record := ManageIMGUIAPI.createRecord() |> AssetTool.prepareFontAsset;
    });
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    describe("sliderFloat", () =>
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
                  ((x1, y1, width1, height1), textWidth1),
                  (min1, max1, numDecimalDigits1),
                  (defaultValue1, str1),
                ) =
                  SliderIMGUITool.buildSliderFloatData1();

                let sliderFunc = apiJsObj##sliderFloat;
                let (record, isSelected, value) =
                  sliderFunc(.
                    ((x1, y1, width1, height1), textWidth1),
                    (min1, max1, numDecimalDigits1),
                    (defaultValue1, str1),
                    record,
                  );

                record;
              },
            ),
            record,
          );

        let _testSliderClick = (ioData, result) => {
          let (
            ((x1, y1, width1, height1), textWidth1),
            (min1, max1, numDecimalDigits1),
            (defaultValue1, str1),
          ) =
            SliderIMGUITool.buildSliderFloatData1();
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
                   let sliderFunc = apiJsObj##sliderFloat;
                   let (record, isSelected, value) =
                     sliderFunc(.
                       ((x1, y1, width1, height1), textWidth1),
                       (min1, max1, numDecimalDigits1),
                       (defaultValue1, str1),
                       record,
                     );

                   isClick := isSelected;

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

          let record = ManageIMGUIAPI.render(gl, ioData, record);

          isClick^ |> expect == result;
        };

        describe("test buffer data", () =>
          describe("test draw box and draw text", () => {
            test("test position buffer data", () =>
              _test(
                [|
                  41.,
                  51.,
                  41.,
                  351.,
                  241.,
                  51.,
                  241.,
                  351.,
                  41.,
                  51.,
                  41.,
                  351.,
                  61.,
                  51.,
                  61.,
                  351.,
                  66.,
                  67.,
                  66.,
                  123.,
                  112.,
                  67.,
                  112.,
                  123.,
                  110.,
                  101.,
                  110.,
                  122.,
                  132.,
                  101.,
                  132.,
                  122.,
                  129.,
                  67.,
                  129.,
                  122.,
                  176.,
                  67.,
                  176.,
                  122.,
                  173.,
                  67.,
                  173.,
                  123.,
                  219.,
                  67.,
                  219.,
                  123.,
                  243.,
                  80.,
                  243.,
                  123.,
                  287.,
                  80.,
                  287.,
                  123.,
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
                    0.6795918367346939,
                    0.13345521023765997,
                    0.6795918367346939,
                    0.23583180987202926,
                    0.773469387755102,
                    0.13345521023765997,
                    0.773469387755102,
                    0.23583180987202926,
                    0.7428571428571429,
                    0.8391224862888482,
                    0.7428571428571429,
                    0.8775137111517367,
                    0.7877551020408163,
                    0.8391224862888482,
                    0.7877551020408163,
                    0.8775137111517367,
                    0.726530612244898,
                    0.23948811700182815,
                    0.726530612244898,
                    0.340036563071298,
                    0.8224489795918367,
                    0.23948811700182815,
                    0.8224489795918367,
                    0.340036563071298,
                    0.6795918367346939,
                    0.13345521023765997,
                    0.6795918367346939,
                    0.23583180987202926,
                    0.773469387755102,
                    0.13345521023765997,
                    0.773469387755102,
                    0.23583180987202926,
                    0.10408163265306122,
                    0.7568555758683729,
                    0.10408163265306122,
                    0.8354661791590493,
                    0.19387755102040816,
                    0.7568555758683729,
                    0.19387755102040816,
                    0.8354661791590493,
                  |],
                  RenderIMGUITool.testTexCoordBufferData,
                  record,
                );
              })
            );
            describe("test index buffer", () =>
              test("test one slider", () =>
                _test(
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
                    24,
                    25,
                    26,
                    27,
                    26,
                    25,
                  |],
                  RenderIMGUITool.testIndexBufferData,
                  record^,
                )
              )
            );
          })
        );

        describe("if mouse hit slider", () =>
          describe("if mouse not pointDown", () => {
            describe("test color buffer data", () =>
              test("backgroudColor and fillColor should be hover color", () => {
                let (
                  ((x1, y1, width1, height1), textWidth1),
                  (min1, max1, numDecimalDigits1),
                  (defaultValue1, str1),
                ) =
                  SliderIMGUITool.buildSliderFloatData1();

                let record =
                  SettingIMGUITool.setSetting(
                    ~record=record^,
                    ~sliderBackgroundColorHover=[|0.1, 0.2, 0.3|],
                    ~sliderFillColorHover=[|0.5, 0.5, 0.3|],
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
                        _buildHitPosition(x1, y1, width1, height1),
                      (),
                    ),
                  ),
                  record,
                );
              })
            );

            test("test slider is not click", () => {
              let (
                ((x1, y1, width1, height1), textWidth1),
                (min1, max1, numDecimalDigits1),
                (defaultValue1, str1),
              ) =
                SliderIMGUITool.buildSliderFloatData1();

              _testSliderClick(
                RenderIMGUITool.buildIOData(
                  ~pointUp=false,
                  ~pointDown=false,
                  ~pointPosition=_buildHitPosition(x1, y1, width1, height1),
                  (),
                ),
                false,
              );
            });
          })
        );
        /* describe("if mouse pointDown", () => {
             describe("test buffer data", () =>
               describe("test color buffer data", () =>
                 test(
                   "innerColor and outterColor should be hover color and should draw inner box",
                   () => {
                   let record =
                     SettingIMGUITool.setSetting(
                       ~record=record^,
                       ~sliderOuterColorHover=[|0.1, 0.2, 0.3|],
                       ~sliderInnerColorHover=[|0.5, 0.5, 0.3|],
                       (),
                     );
                   let (
                     (sliderX1, sliderY1, sliderWidth1, sliderHeight1),
                     str1,
                   ) =
                     SliderIMGUITool.buildSliderFloatData1();

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
                             sliderX1,
                             sliderY1,
                             sliderWidth1,
                             sliderHeight1,
                           ),
                         (),
                       ),
                     ),
                     record,
                   );
                 })
               )
             );

             test("test slider is click", () => {
               let ((sliderX1, sliderY1, sliderWidth1, sliderHeight1), str1) =
                 SliderIMGUITool.buildSliderFloatData1();

               _testSliderClick(
                 RenderIMGUITool.buildIOData(
                   ~pointUp=true,
                   ~pointDown=true,
                   ~pointPosition=
                     _buildHitPosition(
                       sliderX1,
                       sliderY1,
                       sliderWidth1,
                       sliderHeight1,
                     ),
                   (),
                 ),
                 true,
               );
             });
           }); */
        /* describe("else", () =>
             describe("test color buffer data", () =>
               test(
                 "outterColor should be sliderOuterColor and not draw inner box",
                 () => {
                 let ((sliderX1, sliderY1, sliderWidth1, sliderHeight1), str1) =
                   SliderIMGUITool.buildSliderFloatData1();

                 let record =
                   SettingIMGUITool.setSetting(
                     ~record=record^,
                     ~sliderOuterColor=[|0.1, 0.2, 0.3|],
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
                       ~pointPosition=(sliderX1 - 1, sliderY1),
                       (),
                     ),
                   ),
                   record,
                 );
               })
             )
           );
           describe("test slider is not click", () =>
             test("test1", () => {
               let ((sliderX1, sliderY1, sliderWidth1, sliderHeight1), str1) =
                 SliderIMGUITool.buildSliderFloatData1();

               _testSliderClick(
                 RenderIMGUITool.buildIOData(
                   ~pointUp=true,
                   ~pointDown=true,
                   ~pointPosition=
                     _buildNotHitPosition(
                       sliderX1,
                       sliderY1,
                       sliderWidth1,
                       sliderHeight1,
                     ),
                   (),
                 ),
                 false,
               );
             })
           ); */

        describe("test value", () => {
          test("if render slider in the first frame, use default value", () => {
            let (
              ((x1, y1, width1, height1), textWidth1),
              (min1, max1, numDecimalDigits1),
              (defaultValue1, str1),
            ) =
              SliderIMGUITool.buildSliderFloatData1();

            let ioData =
              RenderIMGUITool.buildIOData(
                ~pointUp=false,
                ~pointDown=false,
                ~pointPosition=_buildHitPosition(x1, y1, width1, height1),
                (),
              );

            let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
            let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
            let gl =
              FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ~bufferData, ())
              |> Obj.magic;
            let result = ref(0.);
            let record =
              record^
              |> ManageIMGUIAPI.setIMGUIFunc(
                   RenderIMGUITool.buildCustomData(),
                   (_, apiJsObj, record) => {
                     let sliderFunc = apiJsObj##sliderFloat;
                     let (record, isSelected, value) =
                       sliderFunc(.
                         ((x1, y1, width1, height1), textWidth1),
                         (min1, max1, numDecimalDigits1),
                         (defaultValue1, str1),
                         record,
                       );

                     result := value;

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

            let record = ManageIMGUIAPI.render(gl, ioData, record);

            result^ |> expect == defaultValue1;
          });

          describe("else, use last value", () => {
            test("test one slider", () => {
              let (
                ((x1, y1, width1, height1), textWidth1),
                (min1, max1, numDecimalDigits1),
                (defaultValue1, str1),
              ) =
                SliderIMGUITool.buildSliderFloatData1();

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let result = ref(0.);
              let record =
                record^
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     (_, apiJsObj, record) => {
                       let sliderFunc = apiJsObj##sliderFloat;
                       let (record, isSelected, value) =
                         sliderFunc(.
                           ((x1, y1, width1, height1), textWidth1),
                           (min1, max1, numDecimalDigits1),
                           (defaultValue1, str1),
                           record,
                         );

                       result := value;

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
                    ~pointDown=true,
                    ~pointPosition=(x1 + width1 / 2, y1),
                    (),
                  ),
                  record,
                );

              let record =
                ManageIMGUIAPI.render(
                  gl,
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=false,
                    ~pointPosition=_buildHitPosition(x1, y1, width1, height1),
                    (),
                  ),
                  record,
                );

              result^ |> expect == min1 +. (max1 -. min1) /. 2.;
            });
            test("test two slider", () => {
              let (
                ((x1, y1, width1, height1), textWidth1),
                (min1, max1, numDecimalDigits1),
                (defaultValue1, str1),
              ) =
                SliderIMGUITool.buildSliderFloatData1();

              let (
                ((x2, y2, width2, height2), textWidth2),
                (min2, max2, numDecimalDigits2),
                (defaultValue2, str2),
              ) =
                SliderIMGUITool.buildSliderFloatData2();

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let result1 = ref(0.);
              let result2 = ref(0.);
              let record =
                record^
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     (_, apiJsObj, record) => {
                       let sliderFunc = apiJsObj##sliderFloat;
                       let (record, _, value1) =
                         sliderFunc(.
                           ((x1, y1, width1, height1), textWidth1),
                           (min1, max1, numDecimalDigits1),
                           (defaultValue1, str1),
                           record,
                         );

                       let (record, _, value2) =
                         sliderFunc(.
                           ((x2, y2, width2, height2), textWidth2),
                           (min2, max2, numDecimalDigits2),
                           (defaultValue2, str2),
                           record,
                         );

                       result1 := value1;
                       result2 := value2;

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
                    ~pointDown=true,
                    ~pointPosition=(x1 + width1 / 2, y1),
                    (),
                  ),
                  record,
                );

              let record =
                ManageIMGUIAPI.render(
                  gl,
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=true,
                    ~pointPosition=(x2 + width2 / 2, y2),
                    (),
                  ),
                  record,
                );

              (result1^, result2^)
              |>
              expect == (
                          min1 +. (max1 -. min1) /. 2.,
                          min2 +. (max2 -. min2) /. 2.,
                        );
            });
          });
        });
      })
    );

    describe("sliderInt", () =>
      describe("test with io data", () =>
        describe("test value", () => {
          test("if render slider in the first frame, use default value", () => {
            let (
              ((x1, y1, width1, height1), textWidth1),
              (min1, max1),
              (defaultValue1, str1),
            ) =
              SliderIMGUITool.buildSliderIntData1();

            let ioData =
              RenderIMGUITool.buildIOData(
                ~pointUp=false,
                ~pointDown=false,
                ~pointPosition=_buildHitPosition(x1, y1, width1, height1),
                (),
              );

            let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
            let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
            let gl =
              FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ~bufferData, ())
              |> Obj.magic;
            let result = ref(0);
            let record =
              record^
              |> ManageIMGUIAPI.setIMGUIFunc(
                   RenderIMGUITool.buildCustomData(),
                   (_, apiJsObj, record) => {
                     let sliderFunc = apiJsObj##sliderInt;
                     let (record, isSelected, value) =
                       sliderFunc(.
                         ((x1, y1, width1, height1), textWidth1),
                         (min1, max1),
                         (defaultValue1, str1),
                         record,
                       );

                     result := value;

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

            let record = ManageIMGUIAPI.render(gl, ioData, record);

            result^ |> expect == defaultValue1;
          });

          describe("else, use last value", () => {
            test("test one slider", () => {
              let (
                ((x1, y1, width1, height1), textWidth1),
                (min1, max1),
                (defaultValue1, str1),
              ) =
                SliderIMGUITool.buildSliderIntData1();

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let result = ref(0);
              let record =
                record^
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     (_, apiJsObj, record) => {
                       let sliderFunc = apiJsObj##sliderInt;
                       let (record, isSelected, value) =
                         sliderFunc(.
                           ((x1, y1, width1, height1), textWidth1),
                           (min1, max1),
                           (defaultValue1, str1),
                           record,
                         );

                       result := value;

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
                    ~pointDown=true,
                    ~pointPosition=(x1 + width1 / 2, y1),
                    (),
                  ),
                  record,
                );

              let record =
                ManageIMGUIAPI.render(
                  gl,
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=false,
                    ~pointPosition=_buildHitPosition(x1, y1, width1, height1),
                    (),
                  ),
                  record,
                );

              let min1 = min1 |> NumberType.intToFloat;
              let max1 = max1 |> NumberType.intToFloat;
              result^
              |>
              expect == (
                          min1
                          +. (max1 -. min1)
                          /. 2.
                          |> Js.Math.round
                          |> NumberType.floatToInt
                        );
            });
            test("test two slider", () => {
              let (
                ((x1, y1, width1, height1), textWidth1),
                (min1, max1),
                (defaultValue1, str1),
              ) =
                SliderIMGUITool.buildSliderIntData1();

              let (
                ((x2, y2, width2, height2), textWidth2),
                (min2, max2),
                (defaultValue2, str2),
              ) =
                SliderIMGUITool.buildSliderIntData2();

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let result1 = ref(0);
              let result2 = ref(0);
              let record =
                record^
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     (_, apiJsObj, record) => {
                       let sliderFunc = apiJsObj##sliderInt;
                       let (record, _, value1) =
                         sliderFunc(.
                           ((x1, y1, width1, height1), textWidth1),
                           (min1, max1),
                           (defaultValue1, str1),
                           record,
                         );

                       let (record, _, value2) =
                         sliderFunc(.
                           ((x2, y2, width2, height2), textWidth2),
                           (min2, max2),
                           (defaultValue2, str2),
                           record,
                         );

                       result1 := value1;
                       result2 := value2;

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
                    ~pointDown=true,
                    ~pointPosition=(x1 + width1 / 2, y1),
                    (),
                  ),
                  record,
                );

              let record =
                ManageIMGUIAPI.render(
                  gl,
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=true,
                    ~pointPosition=(x2 + width2 / 2, y2),
                    (),
                  ),
                  record,
                );

              let min1 = min1 |> NumberType.intToFloat;
              let max1 = max1 |> NumberType.intToFloat;
              let min2 = min2 |> NumberType.intToFloat;
              let max2 = max2 |> NumberType.intToFloat;
              (result1^, result2^)
              |>
              expect == (
                          min1
                          +. (max1 -. min1)
                          /. 2.
                          |> Js.Math.round
                          |> NumberType.floatToInt,
                          min2
                          +. (max2 -. min2)
                          /. 2.
                          |> Js.Math.round
                          |> NumberType.floatToInt,
                        );
            });
          });
        })
      )
    );
  });