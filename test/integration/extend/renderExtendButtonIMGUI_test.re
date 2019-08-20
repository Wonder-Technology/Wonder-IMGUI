open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render extend->button", () => {
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
      let _setDefaultSkinData =
          (
            ~record,
            ~buttonColor=[|0.35, 0.1, 0.1|],
            ~hoverButtonColor=[|0.35, 0.1, 0.1|],
            ~clickButtonColor=[|0.35, 0.1, 0.1|],
            ~buttonImage=Js.Nullable.null,
            ~hoverButtonImage=Js.Nullable.null,
            ~clickButtonImage=Js.Nullable.null,
            ~fontAlign=FontType.Center,
            ~fontColor=[|1., 1., 1.|],
            (),
          ) =>
        record
        |> ExtendIMGUIAPI.setSkinData(
             ExtendIMGUIAPI.getDefaultSkinName(),
             ExtendIMGUIAPI.setButtonSkinData(
               ExtendIMGUIAPI.createButtonSkinData(
                 buttonColor,
                 hoverButtonColor,
                 clickButtonColor,
                 buttonImage,
                 hoverButtonImage,
                 clickButtonImage,
                 fontAlign,
                 fontColor,
               ),
               ExtendIMGUIAPI.unsafeGetDefaultSkinData(record),
             ),
           );

      let _testWithExecFuncDataAndSkinData =
          (
            bufferData,
            (testBufferDataFunc, execFuncData),
            (buttonColor, hoverButtonColor, clickButtonColor),
            record,
          ) => {
        let record = RenderIMGUITool.prepareFntData(record);

        testBufferDataFunc(
          sandbox,
          record,
          (
            execFuncData,
            record =>
              record
              |> _setDefaultSkinData(
                   ~record=_,
                   ~buttonColor,
                   ~hoverButtonColor,
                   ~clickButtonColor,
                   (),
                 ),
          ),
          bufferData,
        );
      };

      let _testWithExecFuncData =
          (bufferData, (testBufferDataFunc, execFuncData), record) => {
        let record = RenderIMGUITool.prepareFntData(record);

        testBufferDataFunc(sandbox, record, execFuncData, bufferData);
      };

      let _buildExecFuncData = () =>
        (. _, apiJsObj, record) => {
          let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
            ButtonIMGUITool.buildButtonData1();

          let apiJsObj = Obj.magic(apiJsObj);

          let button = apiJsObj##button;

          let (record, isButtonClick) =
            button(.
              ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1),
              Js.Nullable.null,
              record,
            );

          record;
        };

      let _test = (bufferData, testBufferDataFunc, record) =>
        _testWithExecFuncData(
          bufferData,
          (testBufferDataFunc, _buildExecFuncData()),
          record,
        );

      let _testWithSkinData =
          (
            ~bufferData,
            ~testBufferDataFunc,
            ~record,
            ~buttonColor=[|0.35, 0.1, 0.1|],
            ~hoverButtonColor=[|0.35, 0.1, 0.1|],
            ~clickButtonColor=[|0.35, 0.1, 0.1|],
            (),
          ) =>
        _testWithExecFuncDataAndSkinData(
          bufferData,
          (testBufferDataFunc, _buildExecFuncData()),
          (buttonColor, hoverButtonColor, clickButtonColor),
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
          ExecIMGUITool.addExecFuncData(
            ~record=record^,
            ~func=
              (. _, apiJsObj, record) => {
                let apiJsObj = Obj.magic(apiJsObj);

                let button = apiJsObj##button;

                let (record, isButtonClick) =
                  button(.
                    (
                      (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                      str1,
                    ),
                    Js.Nullable.null,
                    record,
                  );

                isClick := isButtonClick;

                record;
              },
            (),
          );
        let record = RenderIMGUITool.prepareFntData(record);
        let record =
          ManageIMGUIAPI.init(gl, RenderIMGUITool.buildCanvasSize(), record);

        let record = ManageIMGUIAPI.render(gl, ioData, record);

        isClick^ |> expect == result;
      };

      /* let _testPositionBufferDataWithIOData = (ioData) => {

                   RenderIMGUITool.testPositionBufferDataWithIOData(
                     /* RenderIMGUITool.buildIOData(
                       ~pointUp=false,
                       ~pointDown=false,
                       ~pointPosition=(- buttonX1, buttonY1),
                       (),
                     ), */
                     ioData,

                   )
         }; */

      let _testImage =
          (
            ~bufferData,
            ~testBufferDataFunc,
            ~buttonImage=Js.Nullable.null,
            ~hoverButtonImage=Js.Nullable.null,
            ~clickButtonImage=Js.Nullable.null,
            ~record,
            (),
          ) => {
        let record = RenderIMGUITool.prepareFntData(record);

        let (
          (
            (imageX1, imageY1, imageWidth1, imageHeight1),
            (imageS01, imageT01, imageS11, imageT11),
            textureId1,
          ),
          (
            (imageX2, imageY2, imageWidth2, imageHeight2),
            (imageS02, imageT02, imageS12, imageT12),
            textureId2,
          ),
          (
            (imageX3, imageY3, imageWidth3, imageHeight3),
            (imageS03, imageT03, imageS13, imageT13),
            textureId3,
          ),
        ) =
          RenderIMGUITool.buildImageData();

        let record =
          AssetIMGUIService.setCustomImageArr(
            AssetTool.buildCustomImageDataArr(),
            record,
          );
        /*
         let record =
           record
           |> _setDefaultSkinData(
                ~record=_,
                ~buttonImage,
                ~hoverButtonImage,
                ~clickButtonImage,
                (),
              ); */

        let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
          ButtonIMGUITool.buildButtonData1();

        testBufferDataFunc(
          sandbox,
          record,
          (
            _buildExecFuncData(),
            record =>
              record
              |> _setDefaultSkinData(
                   ~record=_,
                   ~buttonImage,
                   ~hoverButtonImage,
                   ~clickButtonImage,
                   (),
                 ),
          ),
          bufferData,
        );
      };

      describe("test buffer data", () => {
        describe("test draw box and draw text", () => {
          test("test fontTexture program->position buffer data", () =>
            _test(
              [|
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
                174.,
              |],
              RenderIMGUITool.testFontTextureProgramPositionBufferData,
              record^,
            )
          );
          test("test no texture program->position buffer data", () =>
            _test(
              [|35., 45., 35., 250., 140., 45., 140., 250.|],
              RenderIMGUITool.testNoTextureProgramPositionBufferData,
              record^,
            )
          );

          test("test fontTexture program->texCoord buffer data", () =>
            _test(
              [|
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
              RenderIMGUITool.testFontTextureProgramTexCoordBufferData,
              record^,
            )
          );

          describe("test index buffer", () => {
            describe("test one button", () => {
              test("test fontTexture program->index buffer", () =>
                _test(
                  [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
                  RenderIMGUITool.testFontTextureProgramIndexBufferData,
                  record^,
                )
              );
              test("test no texture program->index buffer", () =>
                _test(
                  [|0, 1, 2, 3, 2, 1|],
                  RenderIMGUITool.testNoTextureProgramIndexBufferData,
                  record^,
                )
              );
            });

            describe("test two buttons", () => {
              let _buildExecFuncData = () =>
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

                  let button = apiJsObj##button;

                  let (record, isButtonClick) =
                    button(.
                      (
                        (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                        str1,
                      ),
                      Js.Nullable.null,
                      record,
                    );

                  let (record, isButtonClick) =
                    button(.
                      (
                        (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                        str2,
                      ),
                      Js.Nullable.null,
                      record,
                    );

                  record;
                };

              test("test fontTexture program->index buffer", () =>
                _testWithExecFuncData(
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
                  |],
                  (
                    RenderIMGUITool.testFontTextureProgramIndexBufferData,
                    _buildExecFuncData(),
                  ),
                  record^,
                )
              );
              test("test no texture program->index buffer", () =>
                _testWithExecFuncData(
                  [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
                  (
                    RenderIMGUITool.testNoTextureProgramIndexBufferData,
                    _buildExecFuncData(),
                  ),
                  record^,
                )
              );
            });
          });
        });

        describe("test draw image and draw text", () => {
          describe("test position buffer data", () => {
            test("test customTexture program->position buffer data", () => {
              let (
                (_, _, textureId1),
                (_, _, textureId2),
                (_, _, textureId3),
              ) =
                RenderIMGUITool.buildImageData();
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              _testImage(
                ~buttonImage=Js.Nullable.return(textureId3),
                ~bufferData=[|35., 45., 35., 250., 140., 45., 140., 250.|],
                ~record=record^,
                ~testBufferDataFunc=
                  RenderIMGUITool.testCustomTextureProgramPositionBufferDataWithIODataAndAfterInitFunc(
                    RenderIMGUITool.buildIOData(
                      ~pointUp=false,
                      ~pointDown=false,
                      ~pointPosition=(- buttonX1, buttonY1),
                      (),
                    ),
                  ),
                (),
              );
            });
            test("test font texture program->position buffer data", () => {
              let (
                (_, _, textureId1),
                (_, _, textureId2),
                (_, _, textureId3),
              ) =
                RenderIMGUITool.buildImageData();
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              _testImage(
                ~buttonImage=Js.Nullable.return(textureId3),
                ~bufferData=[|
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
                  174.,
                |],
                ~record=record^,
                ~testBufferDataFunc=
                  RenderIMGUITool.testFontTextureProgramPositionBufferDataWithIODataAndAfterInitFunc(
                    RenderIMGUITool.buildIOData(
                      ~pointUp=false,
                      ~pointDown=false,
                      ~pointPosition=(- buttonX1, buttonY1),
                      (),
                    ),
                  ),
                (),
              );
            });
          });

          describe("test texCoord buffer data", () =>
            describe("test customTexture program->texCoord buffer data", () =>
              test("should use whole image", () => {
                let (
                  (_, _, textureId1),
                  (_, _, textureId2),
                  (_, _, textureId3),
                ) =
                  RenderIMGUITool.buildImageData();
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                  ButtonIMGUITool.buildButtonData1();

                _testImage(
                  ~buttonImage=Js.Nullable.return(textureId3),
                  ~hoverButtonImage=Js.Nullable.return(textureId3),
                  ~clickButtonImage=Js.Nullable.return(textureId3),
                  ~bufferData=[|0., 0., 0., 1., 1., 0., 1., 1.|],
                  ~record=record^,
                  ~testBufferDataFunc=RenderIMGUITool.testCustomTextureProgramTexCoordBufferDataAndAfterInitFunc,
                  (),
                );
              })
            )
          );

          describe("test index buffer", () =>
            describe("test one button", () =>
              test("test customTexture program->index buffer data", () => {
                let (
                  (_, _, textureId1),
                  (_, _, textureId2),
                  (_, _, textureId3),
                ) =
                  RenderIMGUITool.buildImageData();

                _testImage(
                  ~buttonImage=Js.Nullable.return(textureId3),
                  ~hoverButtonImage=Js.Nullable.return(textureId3),
                  ~clickButtonImage=Js.Nullable.return(textureId3),
                  ~bufferData=[|0, 1, 2, 3, 2, 1|],
                  ~record=record^,
                  ~testBufferDataFunc=RenderIMGUITool.testCustomTextureProgramIndexBufferDataAndAfterInitFunc,
                  (),
                );
              })
            )
          );
        });
      });

      describe("if mouse hit button", () => {
        describe("if mouse not click or down", () => {
          describe("test color buffer data", () =>
            describe("test no texture program", () =>
              test("box color should be custom style->hoverButtonColor", () => {
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                  ButtonIMGUITool.buildButtonData1();

                _testWithSkinData(
                  ~bufferData=[|
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
                  |],
                  ~hoverButtonColor=[|0.4, 0.2, 0.|],
                  ~testBufferDataFunc=
                    RenderIMGUITool.testNoTextureProgramColorBufferDataWithIODataAndAfterInitFunc(
                      RenderIMGUITool.buildIOData(
                        ~pointUp=false,
                        ~pointDown=false,
                        ~pointPosition=(buttonX1, buttonY1),
                        (),
                      ),
                    ),
                  ~record=record^,
                  (),
                );
              })
            )
          );

          describe("test image", () =>
            test("image should be custom style->hoverButtonImage", () => {
              open DrawDataType;

              let (
                (_, _, textureId1),
                (_, _, textureId2),
                (_, _, textureId3),
              ) =
                RenderIMGUITool.buildImageData();
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              let record = RenderIMGUITool.prepareFntData(record^);

              let record =
                AssetIMGUIService.setCustomImageArr(
                  AssetTool.buildCustomImageDataArr(),
                  record,
                );

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);

              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let canvasWidth = 1000;
              let canvasHeight = 500;

              let record =
                ExecIMGUITool.addExecFuncData(
                  ~record,
                  ~func=_buildExecFuncData(),
                  (),
                );
              let record =
                ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

              let record =
                record
                |> _setDefaultSkinData(
                     ~record=_,
                     ~buttonImage=Js.Nullable.return(textureId1),
                     ~hoverButtonImage=Js.Nullable.return(textureId2),
                     ~clickButtonImage=Js.Nullable.return(textureId3),
                     (),
                   );

              let record =
                ManageIMGUIAPI.render(
                  gl,
                  RenderIMGUITool.buildIOData(
                    ~pointUp=false,
                    ~pointDown=false,
                    ~pointPosition=(buttonX1, buttonY1),
                    (),
                  ),
                  record,
                );

              let {customTextureDrawDataMap} =
                RenderIMGUITool.getDrawData(record);
              customTextureDrawDataMap
              |> WonderCommonlib.MutableHashMapService.has(textureId2)
              |> expect == true;
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
              describe("test no texture program", () =>
                test("box color should be custom style->clickButtonColor", () => {
                  let (
                    (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                    str1,
                  ) =
                    ButtonIMGUITool.buildButtonData1();

                  _testWithSkinData(
                    ~bufferData=[|
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
                    |],
                    ~clickButtonColor=[|0.1, 0.2, 0.5|],
                    ~testBufferDataFunc=
                      RenderIMGUITool.testNoTextureProgramColorBufferDataWithIODataAndAfterInitFunc(
                        RenderIMGUITool.buildIOData(
                          ~pointUp=false,
                          ~pointDown=true,
                          ~pointPosition=(buttonX1, buttonY1),
                          (),
                        ),
                      ),
                    ~record=record^,
                    (),
                  );
                })
              )
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
              describe("test no texture program", () =>
                test("box color should be custom style->clickButtonColor", () => {
                  let (
                    (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                    str1,
                  ) =
                    ButtonIMGUITool.buildButtonData1();

                  _testWithSkinData(
                    ~bufferData=[|
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
                    |],
                    ~clickButtonColor=[|0.1, 0.2, 0.5|],
                    ~testBufferDataFunc=
                      RenderIMGUITool.testNoTextureProgramColorBufferDataWithIODataAndAfterInitFunc(
                        RenderIMGUITool.buildIOData(
                          ~pointUp=true,
                          ~pointDown=true,
                          ~pointPosition=(buttonX1, buttonY1),
                          (),
                        ),
                      ),
                    ~record=record^,
                    (),
                  );
                })
              )
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
          describe("test no texture program", () =>
            test("box color should be setting->buttonColor", () => {
              let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                ButtonIMGUITool.buildButtonData1();

              _testWithSkinData(
                ~bufferData=[|
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
                |],
                ~buttonColor=[|0.4, 0.2, 0.|],
                ~testBufferDataFunc=
                  RenderIMGUITool.testNoTextureProgramColorBufferDataWithIODataAndAfterInitFunc(
                    RenderIMGUITool.buildIOData(
                      ~pointUp=false,
                      ~pointDown=false,
                      ~pointPosition=(buttonX1 - 1, buttonY1),
                      (),
                    ),
                  ),
                ~record=record^,
                (),
              );
            })
          )
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