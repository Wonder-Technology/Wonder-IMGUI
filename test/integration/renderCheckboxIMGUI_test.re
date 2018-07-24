open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render checkbox", () => {
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
          (checkboxX, checkboxY, checkboxWidth, checkboxHeight) => (
        checkboxX - 1,
        checkboxY,
      );

      let _buildHitPosition =
          (checkboxX, checkboxY, checkboxWidth, checkboxHeight) => (
        checkboxX,
        checkboxY,
      );

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
                (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
                str1,
              ) =
                CheckboxIMGUITool.buildCheckboxData1();

              let apiJsObj = Obj.magic(apiJsObj);
              let checkboxFunc = apiJsObj##checkbox;
              let (record, isCheckboxClick) =
                checkboxFunc(.
                  (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
                  str1,
                  record,
                );

              record;
            },
          ),
          record,
        );

      let _testCheckboxClick = (ioData, result) => {
        let ((checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1), str1) =
          CheckboxIMGUITool.buildCheckboxData1();
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
                 let checkboxFunc = apiJsObj##checkbox;
                 let (record, isCheckboxClick) =
                   checkboxFunc(.
                     (
                       checkboxX1,
                       checkboxY1,
                       checkboxWidth1,
                       checkboxHeight1,
                     ),
                     str1,
                     record,
                   );

                 isClick := isCheckboxClick;

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
                455.,
                445.,
                45.,
                445.,
                455.,
                449.,
                62.,
                449.,
                117.,
                494.,
                62.,
                494.,
                117.,
                492.,
                75.,
                492.,
                117.,
                536.,
                75.,
                536.,
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
            test("test one checkbox", () =>
              _test(
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5, 8, 9, 10, 11, 10, 9|],
                RenderIMGUITool.testIndexBufferData,
                record^,
              )
            );
            test("test two checkboxs", () =>
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
                      (
                        checkboxX1,
                        checkboxY1,
                        checkboxWidth1,
                        checkboxHeight1,
                      ),
                      str1,
                    ) =
                      CheckboxIMGUITool.buildCheckboxData1();
                    let (
                      (
                        checkboxX2,
                        checkboxY2,
                        checkboxWidth2,
                        checkboxHeight2,
                      ),
                      str2,
                    ) =
                      CheckboxIMGUITool.buildCheckboxData1();

                    let apiJsObj = Obj.magic(apiJsObj);
                    let checkboxFunc = apiJsObj##checkbox;

                    let (record, _) =
                      checkboxFunc(.
                        (
                          checkboxX1,
                          checkboxY1,
                          checkboxWidth1,
                          checkboxHeight1,
                        ),
                        str1,
                        record,
                      );
                    let (record, _) =
                      checkboxFunc(.
                        (
                          checkboxX2,
                          checkboxY2,
                          checkboxWidth2,
                          checkboxHeight2,
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

      describe("if mouse hit checkbox", () => {
        describe("if mouse not click", () =>
          describe("test color buffer data", () =>
            test(
              "outterColor should be hover color and not draw inner box", () => {
              let (
                (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
                str1,
              ) =
                CheckboxIMGUITool.buildCheckboxData1();

              let record =
                SettingIMGUITool.setSetting(
                  ~record=record^,
                  ~checkboxOuterColorHover=[|0., 0.2, 0.3|],
                  ~checkboxInnerColorHover=[|0.5, 0.5, 0.3|],
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
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
                  1.,
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
                        checkboxX1,
                        checkboxY1,
                        checkboxWidth1,
                        checkboxHeight1,
                      ),
                    (),
                  ),
                ),
                record,
              );
            })
          )
        );
        test("test checkbox is not click", () => {
          let (
            (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
            str1,
          ) =
            CheckboxIMGUITool.buildCheckboxData1();

          _testCheckboxClick(
            RenderIMGUITool.buildIOData(
              ~pointUp=false,
              ~pointDown=false,
              ~pointPosition=
                _buildHitPosition(
                  checkboxX1,
                  checkboxY1,
                  checkboxWidth1,
                  checkboxHeight1,
                ),
              (),
            ),
            false,
          );
        });

        describe("if mouse click", () =>
          describe("test buffer data", () =>
            describe("test color buffer data", () =>
              test(
                "innerColor and outterColor should be hover color and should draw inner box",
                () => {
                let record =
                  SettingIMGUITool.setSetting(
                    ~record=record^,
                    ~checkboxOuterColorHover=[|0.1, 0.2, 0.3|],
                    ~checkboxInnerColorHover=[|0.5, 0.5, 0.3|],
                    (),
                  );
                let (
                  (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
                  str1,
                ) =
                  CheckboxIMGUITool.buildCheckboxData1();

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
                          checkboxX1,
                          checkboxY1,
                          checkboxWidth1,
                          checkboxHeight1,
                        ),
                      (),
                    ),
                  ),
                  record,
                );
              })
            )
          )
        );
        test("test checkbox is click", () => {
          let (
            (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
            str1,
          ) =
            CheckboxIMGUITool.buildCheckboxData1();

          _testCheckboxClick(
            RenderIMGUITool.buildIOData(
              ~pointUp=true,
              ~pointDown=true,
              ~pointPosition=
                _buildHitPosition(
                  checkboxX1,
                  checkboxY1,
                  checkboxWidth1,
                  checkboxHeight1,
                ),
              (),
            ),
            true,
          );
        });
      });

      describe("else", () =>
        describe("test color buffer data", () =>
          test(
            "outterColor should be checkboxOuterColor and not draw inner box",
            () => {
            let (
              (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
              str1,
            ) =
              CheckboxIMGUITool.buildCheckboxData1();

            let record =
              SettingIMGUITool.setSetting(
                ~record=record^,
                ~checkboxOuterColor=[|0.1, 0.2, 0.3|],
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
                  ~pointPosition=(checkboxX1 - 1, checkboxY1),
                  (),
                ),
              ),
              record,
            );
          })
        )
      );
      describe("test checkbox is not click", () =>
        test("test1", () => {
          let (
            (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
            str1,
          ) =
            CheckboxIMGUITool.buildCheckboxData1();

          _testCheckboxClick(
            RenderIMGUITool.buildIOData(
              ~pointUp=true,
              ~pointDown=true,
              ~pointPosition=
                _buildNotHitPosition(
                  checkboxX1,
                  checkboxY1,
                  checkboxWidth1,
                  checkboxHeight1,
                ),
              (),
            ),
            false,
          );
        })
      );

      describe("test select", () => {
        test(
          {|test first frame select it;
   second frame not select it;

   it should be still selected
  |},
          () => {
            let (
              (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
              str1,
            ) =
              CheckboxIMGUITool.buildCheckboxData1();
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
                     let checkboxFunc = apiJsObj##checkbox;
                     let (record, isCheckboxClick) =
                       checkboxFunc(.
                         (
                           checkboxX1,
                           checkboxY1,
                           checkboxWidth1,
                           checkboxHeight1,
                         ),
                         str1,
                         record,
                       );

                     isClick := isCheckboxClick;

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
                      checkboxX1,
                      checkboxY1,
                      checkboxWidth1,
                      checkboxHeight1,
                    ),
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
                  ~pointPosition=
                    _buildNotHitPosition(
                      checkboxX1,
                      checkboxY1,
                      checkboxWidth1,
                      checkboxHeight1,
                    ),
                  (),
                ),
                record,
              );

            isClick^ |> expect == true;
          },
        );

        test(
          {|test first frame select it;
   second frame select it;

   it should be not selected
  |},
          () => {
            let (
              (checkboxX1, checkboxY1, checkboxWidth1, checkboxHeight1),
              str1,
            ) =
              CheckboxIMGUITool.buildCheckboxData1();
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
                     let checkboxFunc = apiJsObj##checkbox;
                     let (record, isCheckboxClick) =
                       checkboxFunc(.
                         (
                           checkboxX1,
                           checkboxY1,
                           checkboxWidth1,
                           checkboxHeight1,
                         ),
                         str1,
                         record,
                       );

                     isClick := isCheckboxClick;

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
                      checkboxX1,
                      checkboxY1,
                      checkboxWidth1,
                      checkboxHeight1,
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
                      checkboxX1,
                      checkboxY1,
                      checkboxWidth1,
                      checkboxHeight1,
                    ),
                  (),
                ),
                record,
              );

            isClick^ |> expect == false;
          },
        );
      });
    });
  });