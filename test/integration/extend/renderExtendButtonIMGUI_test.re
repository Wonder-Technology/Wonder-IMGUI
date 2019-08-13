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
      let _initSkin =
          (
            ~record,
            ~buttonColor=[|0.35, 0.1, 0.1|],
            ~hoverButtonColor=[|0.35, 0.1, 0.1|],
            ~clickButtonColor=[|0.35, 0.1, 0.1|],
            ~buttonImage=Js.Nullable.null,
            ~hoverButtonImage=Js.Nullable.null,
            ~clickButtonImage=Js.Nullable.null,
            (),
          ) => {
        open ManageSkinIMGUIService;

        let allCustomStyleData = createAllCustomStyleData();

        let singleCustomStyleData = createSingleCustomStyleData();

        let singleCustomStyleData =
          singleCustomStyleData
          |> addCustomStyleData("buttonColor", buttonColor |> Obj.magic)
          |> addCustomStyleData(
               "hoverButtonColor",
               hoverButtonColor |> Obj.magic,
             )
          |> addCustomStyleData(
               "clickButtonColor",
               clickButtonColor |> Obj.magic,
             );

        let singleCustomStyleData =
          switch (buttonImage |> Js.Nullable.toOption) {
          | Some(buttonImage) =>
            singleCustomStyleData
            |> addCustomStyleData("buttonImage", buttonImage |> Obj.magic)
          | None => singleCustomStyleData
          };

        let singleCustomStyleData =
          switch (hoverButtonImage |> Js.Nullable.toOption) {
          | Some(hoverButtonImage) =>
            singleCustomStyleData
            |> addCustomStyleData(
                 "hoverButtonImage",
                 hoverButtonImage |> Obj.magic,
               )
          | None => singleCustomStyleData
          };

        let singleCustomStyleData =
          switch (clickButtonImage |> Js.Nullable.toOption) {
          | Some(clickButtonImage) =>
            singleCustomStyleData
            |> addCustomStyleData(
                 "clickButtonImage",
                 clickButtonImage |> Obj.magic,
               )
          | None => singleCustomStyleData
          };

        let allCustomStyleData =
          allCustomStyleData
          |> addSingleCustomStyleData("CustomStyle1", singleCustomStyleData);

        let skinData = createSkinData(allCustomStyleData);

        record
        |> clearAllSkins
        |> DataSkinIMGUIService.addDefaultSkinData
        |> addSkinData("Skin1", skinData);
      };

      let _registerCustomControl = record =>
        ExtendIMGUIAPI.registerCustomControl(
          "Wonder_Button",
          (. customControlFuncData, showData, apiJsObj, record) => {
            let (rect, str) = customControlFuncData |> Obj.magic;
            /* TODO move to apiJsObj? */
            let isInBox =
                ((x, y, width, height), (posX: float, posY: float)) => {
              open Pervasives;

              let minX = x;
              let minY = y;
              let maxX = x +. width;
              let maxY = y +. height;

              minX <= posX && posX <= maxX && minY <= posY && posY <= maxY;
            };

            let convertIntPositionToFloatPosition = ((x, y)) => (
              x |> NumberType.intToFloat,
              y |> NumberType.intToFloat,
            );

            let judge =
                (
                  (clickButtonData: 'a, hoverButtonData: 'b, buttonData: 'c),
                  (pointDown, pointPosition),
                  isClickFunc,
                  record,
                ) =>
              isInBox(rect, convertIntPositionToFloatPosition(pointPosition)) ?
                isClickFunc(. record) ?
                  (true, clickButtonData) :
                  pointDown ?
                    (false, clickButtonData) : (false, hoverButtonData) :
                (false, buttonData);

            let apiJsObj = apiJsObj |> Obj.magic;

            let getIOData = apiJsObj##getIOData;
            let getPointUp = apiJsObj##getPointUp;
            let getPointDown = apiJsObj##getPointDown;
            let getPointPosition = apiJsObj##getPointPosition;
            let isClick = apiJsObj##isClick;

            let drawBox = apiJsObj##drawBox;
            let drawText = apiJsObj##drawText;
            let drawImage = apiJsObj##drawImage;

            let ioData = getIOData(. record);
            let pointUp = getPointUp(. ioData);
            let pointDown = getPointDown(. ioData);
            let pointPosition = getPointPosition(. ioData);

            let parseShowData = apiJsObj##parseShowData;
            let unsafeGetSkinData = apiJsObj##unsafeGetSkinData;
            let unsafeGetSingleCustomStyleDataMap =
              apiJsObj##unsafeGetSingleCustomStyleDataMap;

            let unsafeGetCustomStyleData = apiJsObj##unsafeGetCustomStyleData;

            let hasCustomStyleData = apiJsObj##hasCustomStyleData;

            let (skinName, singleCustomStyleName) =
              parseShowData(. showData);

            let singleCustomStyleDataMap =
              unsafeGetSingleCustomStyleDataMap(.
                singleCustomStyleName,
                unsafeGetSkinData(. skinName, record),
              );

            let buttonImage =
              hasCustomStyleData(. "buttonImage", singleCustomStyleDataMap) ?
                unsafeGetCustomStyleData(.
                  "buttonImage",
                  singleCustomStyleDataMap,
                )
                |> Js.Nullable.return :
                Js.Nullable.null;
            let buttonColor =
              unsafeGetCustomStyleData(.
                "buttonColor",
                singleCustomStyleDataMap,
              );

            let hoverButtonImage =
              hasCustomStyleData(.
                "hoverButtonImage",
                singleCustomStyleDataMap,
              ) ?
                unsafeGetCustomStyleData(.
                  "hoverButtonImage",
                  singleCustomStyleDataMap,
                )
                |> Js.Nullable.return :
                Js.Nullable.null;
            let hoverButtonColor =
              unsafeGetCustomStyleData(.
                "hoverButtonColor",
                singleCustomStyleDataMap,
              );

            let clickButtonImage =
              hasCustomStyleData(.
                "clickButtonImage",
                singleCustomStyleDataMap,
              ) ?
                unsafeGetCustomStyleData(.
                  "clickButtonImage",
                  singleCustomStyleDataMap,
                )
                |> Js.Nullable.return :
                Js.Nullable.null;
            let clickButtonColor =
              unsafeGetCustomStyleData(.
                "clickButtonColor",
                singleCustomStyleDataMap,
              );

            let (isButtonClick, (imageId, color)) =
              judge(
                (
                  (clickButtonImage, clickButtonColor),
                  (hoverButtonImage, hoverButtonColor),
                  (buttonImage, buttonColor),
                ),
                (pointDown, pointPosition),
                isClick,
                record,
              );

            let record =
              switch (Js.Nullable.toOption(imageId)) {
              | None => drawBox(. rect, color, record)
              | Some(imageId) =>
                drawImage(. rect, (0., 0., 1., 1.), imageId, record)
              };

            let record = drawText(. rect, str, FontType.Center, record);

            (record, isButtonClick |> Obj.magic);
          },
          record,
        );

      let _testWithIMGUIFuncAndSkinData =
          (
            bufferData,
            (testBufferDataFunc, imguiFunc),
            (buttonColor, hoverButtonColor, clickButtonColor),
            record,
          ) => {
        let record = RenderIMGUITool.prepareFntData(record);

        let record =
          record
          |> _initSkin(
               ~record=_,
               ~buttonColor,
               ~hoverButtonColor,
               ~clickButtonColor,
               (),
             )
          |> _registerCustomControl;

        testBufferDataFunc(sandbox, record, imguiFunc, bufferData);
      };

      let _testWithIMGUIFunc =
          (bufferData, (testBufferDataFunc, imguiFunc), record) => {
        let record = RenderIMGUITool.prepareFntData(record);

        let record =
          record |> _initSkin(~record=_, ()) |> _registerCustomControl;

        testBufferDataFunc(sandbox, record, imguiFunc, bufferData);
      };

      let _buildIMGUIFunc = () =>
        (. _, apiJsObj, record) => {
          let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
            ButtonIMGUITool.buildButtonData1();

          let apiJsObj = Obj.magic(apiJsObj);

          /* let buttonFunc = apiJsObj##button;
             let (record, isButtonClick) =
               buttonFunc(.
                 (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                 str1,
                 record,
               ); */

          let unsafeGetCustomControl = apiJsObj##unsafeGetCustomControl;

          let buttonFunc = unsafeGetCustomControl(. "Wonder_Button", record);

          let (record, isButtonClick) =
            buttonFunc(.
              ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1),
              Js.Nullable.return(("Skin1", "CustomStyle1")),
              record,
            );

          record;
        };

      let _test = (bufferData, testBufferDataFunc, record) =>
        _testWithIMGUIFunc(
          bufferData,
          (testBufferDataFunc, _buildIMGUIFunc()),
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
        _testWithIMGUIFuncAndSkinData(
          bufferData,
          (testBufferDataFunc, _buildIMGUIFunc()),
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
          record^
          |> _initSkin(~record=_, ())
          |> _registerCustomControl
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(),
               (. _, apiJsObj, record) => {
                 let apiJsObj = Obj.magic(apiJsObj);

                 let unsafeGetCustomControl = apiJsObj##unsafeGetCustomControl;

                 let buttonFunc =
                   unsafeGetCustomControl(. "Wonder_Button", record);

                 let (record, isButtonClick) =
                   buttonFunc(.
                     (
                       (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                       str1,
                     ),
                     Js.Nullable.return(("Skin1", "CustomStyle1")),
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

        let record =
          record
          |> _initSkin(
               ~record=_,
               ~buttonImage,
               ~hoverButtonImage,
               ~clickButtonImage,
               (),
             )
          |> _registerCustomControl;

        let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
          ButtonIMGUITool.buildButtonData1();

        testBufferDataFunc(sandbox, record, _buildIMGUIFunc(), bufferData);
      };

      describe("test buffer data", () => {
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
                174.,
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

                    let unsafeGetCustomControl =
                      apiJsObj##unsafeGetCustomControl;

                    let buttonFunc =
                      unsafeGetCustomControl(. "Wonder_Button", record);

                    let (record, isButtonClick) =
                      buttonFunc(.
                        (
                          (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                          str1,
                        ),
                        Js.Nullable.null,
                        record,
                      );

                    let (record, isButtonClick) =
                      buttonFunc(.
                        (
                          (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                          str2,
                        ),
                        Js.Nullable.null,
                        record,
                      );

                    record;
                  },
                ),
                record^,
              )
            );
          });
        });

        describe("test draw image and draw text", () => {
          test("test position buffer data", () => {
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
                174.,
              |],
              ~record=record^,
              ~testBufferDataFunc=
                RenderIMGUITool.testPositionBufferDataWithIOData(
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

          describe("test texCoord buffer data", () =>
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
                ~bufferData=[|
                  0.,
                  0.,
                  0.,
                  1.,
                  1.,
                  0.,
                  1.,
                  1.,
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
                ~record=record^,
                ~testBufferDataFunc=RenderIMGUITool.testTexCoordBufferData,
                (),
              );
            })
          );

          describe("test index buffer", () =>
            test("test one button", () => {
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
                ~bufferData=[|
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
                |],
                ~record=record^,
                ~testBufferDataFunc=RenderIMGUITool.testIndexBufferData,
                (),
              );
            })
          );
        });
      });

      describe("if mouse hit button", () => {
        describe("if mouse not click or down", () => {
          describe("test color buffer data", () =>
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
                ~hoverButtonColor=[|0.4, 0.2, 0.|],
                ~testBufferDataFunc=
                  RenderIMGUITool.testColorBufferDataWithIOData(
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

              let record =
                record
                |> _initSkin(
                     ~record=_,
                     ~buttonImage=Js.Nullable.return(textureId1),
                     ~hoverButtonImage=Js.Nullable.return(textureId2),
                     ~clickButtonImage=Js.Nullable.return(textureId3),
                     (),
                   )
                |> _registerCustomControl;

              let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);

              let gl =
                FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ())
                |> Obj.magic;
              let canvasWidth = 1000;
              let canvasHeight = 500;
              let record =
                record
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     _buildIMGUIFunc(),
                   );
              let record =
                ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

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
              test("box color should be setting->clickButtonColor", () => {
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
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
                  ~clickButtonColor=[|0.1, 0.2, 0.5|],
                  ~testBufferDataFunc=
                    RenderIMGUITool.testColorBufferDataWithIOData(
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
                let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
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
                  ~clickButtonColor=[|0.1, 0.2, 0.5|],
                  ~testBufferDataFunc=
                    RenderIMGUITool.testColorBufferDataWithIOData(
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
              ~buttonColor=[|0.4, 0.2, 0.|],
              ~testBufferDataFunc=
                RenderIMGUITool.testColorBufferDataWithIOData(
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