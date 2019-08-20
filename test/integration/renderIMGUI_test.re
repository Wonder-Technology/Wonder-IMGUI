open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render", () => {
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

    describe("test render", () => {
      let _buildNoVAOExtension = sandbox =>
        RenderIMGUITool.buildNoVAOExtension(sandbox);

      let _drawTwoLabelsAndThreeImages = () => {
        let (
          (
            (labelX1, labelY1, labelWidth1, labelHeight1),
            labelStr1,
            labelColor1,
            labelAlign1,
          ),
          (
            (labelX2, labelY2, labelWidth2, labelHeight2),
            labelStr2,
            labelColor2,
            labelAlign2,
          ),
        ) =
          RenderIMGUITool.buildLabelData();
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
            record^,
          );

        let record =
          ExecIMGUITool.addExecFuncData(
            ~record,
            ~func=
              (. _, apiJsObj, record) => {
                let apiJsObj = Obj.magic(apiJsObj);
                let label = apiJsObj##label;
                let image = apiJsObj##image;
                let record =
                  label(.
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    (labelColor1, labelAlign1),
                    record,
                  );
                let record =
                  image(.
                    (imageX1, imageY1, imageWidth1, imageHeight1),
                    (imageS01, imageT01, imageS11, imageT11),
                    textureId1,
                    record,
                  );
                let record =
                  image(.
                    (imageX2, imageY2, imageWidth2, imageHeight2),
                    (imageS02, imageT02, imageS12, imageT12),
                    textureId2,
                    record,
                  );
                let record =
                  label(.
                    (labelX2, labelY2, labelWidth2, labelHeight2),
                    labelStr2,
                    (labelColor2, labelAlign2),
                    record,
                  );
                let record =
                  image(.
                    (imageX3, imageY3, imageWidth3, imageHeight3),
                    (imageS03, imageT03, imageS13, imageT13),
                    textureId3,
                    record,
                  );

                record;
              },
            (),
          );

        (
          record,
          (
            (
              ((labelX1, labelY1, labelWidth1, labelHeight1), labelStr1),
              ((labelX2, labelY2, labelWidth2, labelHeight2), labelStr2),
            ),
            (
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
            ),
          ),
        );
      };

      let _prepareAndExecEmptyExecFuncData = gl => {
        let canvasWidth = 1000;
        let canvasHeight = 500;

        let record = ExecIMGUITool.addExecFuncData(~record=record^, ());

        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        (record, canvasWidth, canvasHeight);
      };

      test("clear drawDataArr", () => {
        open DrawDataType;

        let (
          (
            (labelX1, labelY1, labelWidth1, labelHeight1),
            labelStr1,
            labelColor1,
            labelAlign1,
          ),
          _,
        ) =
          RenderIMGUITool.buildLabelData();
        let (
          (
            (imageX1, imageY1, imageWidth1, imageHeight1),
            (imageS01, imageT01, imageS11, imageT11),
            textureId1,
          ),
          _,
          _,
        ) =
          RenderIMGUITool.buildImageData();
        let record = RenderIMGUITool.prepareFntData(record^);
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          ExecIMGUITool.addExecFuncData(
            ~record,
            ~func=
              (. _, apiJsObj, record) => {
                let apiJsObj = Obj.magic(apiJsObj);
                let label = apiJsObj##label;
                let image = apiJsObj##image;

                let record =
                  label(.
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    (labelColor1, labelAlign1),
                    record,
                  );
                let record =
                  image(.
                    (imageX1, imageY1, imageWidth1, imageHeight1),
                    (imageS01, imageT01, imageS11, imageT11),
                    textureId1,
                    record,
                  );

                record;
              },
            (),
          );
        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        let {fontTextureDrawData, customTextureDrawDataMap} =
          RenderIMGUITool.getDrawData(record);

        (
          fontTextureDrawData.verticeArr |> Js.Array.length,
          customTextureDrawDataMap
          |> WonderCommonlib.MutableHashMapService.length,
        )
        |> expect == (8, 1);
      });

      test("reset currentFontTextureDrawDataBaseIndex", () => {
        let (
          (
            (labelX1, labelY1, labelWidth1, labelHeight1),
            labelStr1,
            labelColor1,
            labelAlign1,
          ),
          (
            (labelX2, labelY2, labelWidth2, labelHeight2),
            labelStr2,
            labelColor2,
            labelAlign2,
          ),
        ) =
          RenderIMGUITool.buildLabelData();
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let element_array_buffer = 1;
        let dynamic_draw = 2;
        let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(
            ~sandbox,
            ~getExtension,
            ~element_array_buffer,
            ~bufferData,
            ~dynamic_draw,
            (),
          )
          |> Obj.magic;
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          ExecIMGUITool.addExecFuncData(
            ~record=record^,
            ~func=
              (. _, apiJsObj, record) => {
                let apiJsObj = Obj.magic(apiJsObj);
                let label = apiJsObj##label;
                let record =
                  label(.
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    (labelColor1, labelAlign1),
                    record,
                  );

                record;
              },
            (),
          );
        let record = RenderIMGUITool.prepareFntData(record);
        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);
        let bufferDataCallCountAfterFirstRender = bufferData |> getCallCount;
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        bufferData
        |> getCall(bufferDataCallCountAfterFirstRender + 7)
        |> expect
        |> toCalledWith([|
             element_array_buffer,
             Uint16Array.make([|0, 1, 2, 3, 2, 1|]) |> Obj.magic,
             dynamic_draw,
           |]);
      });

      test("if has no execFuncData, not error", () => {
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record^);

        expect(() => {
          let record =
            ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);
          ();
        })
        |> not_
        |> toThrow;
      });

      test("unbind vao", () => {
        let getExtension = createEmptyStubWithJsObjSandbox(sandbox);

        let bindVertexArrayOES = createEmptyStubWithJsObjSandbox(sandbox);

        let vaoExt = Obj.magic({"bindVertexArrayOES": bindVertexArrayOES});
        getExtension
        |> withOneArg("OES_vertex_array_object")
        |> returns(Js.Nullable.return(vaoExt));

        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;

        let record = ManageIMGUIAPI.init(gl, (0, 0), record^);

        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record = ExecIMGUITool.addExecFuncData(~record, ());
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        bindVertexArrayOES |> expect |> toCalledWith([|Js.Nullable.null|]);
      });

      describe("buffer all data", () => {
        let _testEnableVertexAttribArrayAndVertexAttribPointer =
            (name, size, count) => {
          let getExtension = _buildNoVAOExtension(sandbox);
          let float = 1;
          let location = 10;
          let program = Obj.magic(1);
          let getAttribLocation = createEmptyStubWithJsObjSandbox(sandbox);
          getAttribLocation
          |> withTwoArgs(program, name)
          |> returns(location);
          let createProgram = createEmptyStubWithJsObjSandbox(sandbox);
          createProgram |> returns(program);
          let enableVertexAttribArray =
            createEmptyStubWithJsObjSandbox(sandbox);
          let vertexAttribPointer = createEmptyStubWithJsObjSandbox(sandbox);
          let gl =
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~float,
              ~getExtension,
              ~createProgram,
              ~getAttribLocation,
              ~enableVertexAttribArray,
              ~vertexAttribPointer,
              (),
            )
            |> Obj.magic;
          let (
            (
              (labelX1, labelY1, labelWidth1, labelHeight1),
              labelStr1,
              labelColor1,
              labelAlign1,
            ),
            (
              (labelX2, labelY2, labelWidth2, labelHeight2),
              labelStr2,
              labelColor2,
              labelAlign2,
            ),
          ) =
            RenderIMGUITool.buildLabelData();
          let canvasWidth = 1000;
          let canvasHeight = 500;
          let record =
            ExecIMGUITool.addExecFuncData(
              ~record=record^,
              ~func=
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      (labelColor1, labelAlign1),
                      record,
                    );

                  record;
                },
              (),
            );
          let record = RenderIMGUITool.prepareFntData(record);

          let record =
            ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);
          let record =
            ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

          (
            enableVertexAttribArray |> withOneArg(location) |> getCallCount,
            vertexAttribPointer
            |> withFourArgs(location, size, float, false)
            |> getCallCount,
          )
          |> expect == (count, count);
        };

        let _getPositionBufferData = () => (
          (
            [|12., 104., 12., 147., 59., 104., 59., 147.|],
            [|22., 104., 22., 147., 69., 104., 69., 147.|],
          ),
          (
            [|50., 60., 50., 310., 200., 60., 200., 310.|],
            [|51., 61., 51., 312., 302., 61., 302., 312.|],
            [|52., 62., 52., 414., 404., 62., 404., 414.|],
          ),
        );

        describe("test send position buffer data", () =>
          describe("test buffer data", () => {
            test("test draw one image", () => {
              let (
                (
                  (imageX1, imageY1, imageWidth1, imageHeight1),
                  (imageS01, imageT01, imageS11, imageT11),
                  textureId1,
                ),
                _,
                _,
              ) =
                RenderIMGUITool.buildImageData();
              let (_, (image1Data, _, _)) = _getPositionBufferData();
              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testCustomTextureProgramPositionBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );

                  record;
                },
                image1Data,
              );
            });

            test("test draw two images", () => {
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
                _,
              ) =
                RenderIMGUITool.buildImageData();
              let (_, (image1Data, image2Data, _)) =
                _getPositionBufferData();
              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testCustomTextureProgramPositionBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );
                  let record =
                    image(.
                      (imageX2, imageY2, imageWidth2, imageHeight2),
                      (imageS02, imageT02, imageS12, imageT12),
                      textureId1,
                      record,
                    );

                  record;
                },
                image1Data |> Js.Array.concat(image2Data),
              );
            });

            test("test draw three images", () => {
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
                  record^,
                );

              let (_, (image1Data, image2Data, image3Data)) =
                _getPositionBufferData();
              let record = RenderIMGUITool.prepareFntData(record);

              RenderIMGUITool.testCustomTextureProgramPositionBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId2,
                      record,
                    );
                  let record =
                    image(.
                      (imageX2, imageY2, imageWidth2, imageHeight2),
                      (imageS02, imageT02, imageS12, imageT12),
                      textureId1,
                      record,
                    );
                  let record =
                    image(.
                      (imageX3, imageY3, imageWidth3, imageHeight3),
                      (imageS03, imageT03, imageS13, imageT13),
                      textureId2,
                      record,
                    );

                  record;
                },
                image1Data
                |> Js.Array.concat(image3Data)
                |> Js.Array.concat(image2Data),
              );
            });

            describe("test draw two labels and draw three images", () => {
              let _buildExecFuncData = () => {
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

                let (
                  (
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    labelColor1,
                    labelAlign1,
                  ),
                  (
                    (labelX2, labelY2, labelWidth2, labelHeight2),
                    labelStr2,
                    labelColor2,
                    labelAlign2,
                  ),
                ) =
                  RenderIMGUITool.buildLabelData();

                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let image = apiJsObj##image;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      (labelColor1, labelAlign1),
                      record,
                    );
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId2,
                      record,
                    );
                  let record =
                    image(.
                      (imageX2, imageY2, imageWidth2, imageHeight2),
                      (imageS02, imageT02, imageS12, imageT12),
                      textureId1,
                      record,
                    );
                  let record =
                    label(.
                      (labelX2, labelY2, labelWidth2, labelHeight2),
                      labelStr2,
                      (labelColor2, labelAlign2),
                      record,
                    );
                  let record =
                    image(.
                      (imageX3, imageY3, imageWidth3, imageHeight3),
                      (imageS03, imageT03, imageS13, imageT13),
                      textureId2,
                      record,
                    );

                  record;
                };
              };

              beforeEach(() => {
                let newRecord =
                  AssetIMGUIService.setCustomImageArr(
                    AssetTool.buildCustomImageDataArr(),
                    record^,
                  );

                record := RenderIMGUITool.prepareFntData(newRecord);
              });

              test("test customTexture program->position buffer data", () => {
                let (
                  (label1Data, label2Data),
                  (image1Data, image2Data, image3Data),
                ) =
                  _getPositionBufferData();

                RenderIMGUITool.testCustomTextureProgramPositionBufferData(
                  sandbox,
                  record^,
                  _buildExecFuncData(),
                  image1Data
                  |> Js.Array.concat(image3Data)
                  |> Js.Array.concat(image2Data),
                );
              });
              test("test fontTexture program->position buffer data", () => {
                let (
                  (label1Data, label2Data),
                  (image1Data, image2Data, image3Data),
                ) =
                  _getPositionBufferData();

                RenderIMGUITool.testFontTextureProgramPositionBufferData(
                  sandbox,
                  record^,
                  _buildExecFuncData(),
                  label1Data |> Js.Array.concat(label2Data),
                );
              });
            });
          })
        );

        describe("enableVertexAttribArray and vertexAttribPointer", () =>
          test("test", () =>
            _testEnableVertexAttribArrayAndVertexAttribPointer(
              "a_position",
              2,
              3,
            )
          )
        );

        describe("test send color buffer data", () => {
          describe("test buffer data", () =>
            test("test draw one image", () => {
              let (
                (
                  (imageX1, imageY1, imageWidth1, imageHeight1),
                  (imageS01, imageT01, imageS11, imageT11),
                  textureId1,
                ),
                _,
                _,
              ) =
                RenderIMGUITool.buildImageData();

              RenderIMGUITool.testCustomTextureProgramColorBufferData(
                sandbox,
                record^,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );

                  record;
                },
                [|1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.|],
              );
            })
          );

          describe("enableVertexAttribArray and vertexAttribPointer", () =>
            test("test", () =>
              _testEnableVertexAttribArrayAndVertexAttribPointer(
                "a_color",
                3,
                3,
              )
            )
          );
        });

        describe("test send texCoord buffer data", () => {
          describe("test buffer data", () =>
            test("test draw one image", () => {
              let (
                (
                  (imageX1, imageY1, imageWidth1, imageHeight1),
                  (imageS01, imageT01, imageS11, imageT11),
                  textureId1,
                ),
                _,
                _,
              ) =
                RenderIMGUITool.buildImageData();

              RenderIMGUITool.testCustomTextureProgramTexCoordBufferData(
                sandbox,
                record^,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );

                  record;
                },
                [|0.1, 0.5, 0.1, 0.8, 1., 0.5, 1., 0.8|],
              );
            })
          );

          describe("enableVertexAttribArray and vertexAttribPointer", () =>
            test("test", () =>
              _testEnableVertexAttribArrayAndVertexAttribPointer(
                "a_texCoord",
                2,
                2,
              )
            )
          );
        });
        describe("test send index buffer data", () =>
          describe("test buffer data", () => {
            test("test draw two labels", () => {
              let (
                (
                  (labelX1, labelY1, labelWidth1, labelHeight1),
                  labelStr1,
                  labelColor1,
                  labelAlign1,
                ),
                (
                  (labelX2, labelY2, labelWidth2, labelHeight2),
                  labelStr2,
                  labelColor2,
                  labelAlign2,
                ),
              ) =
                RenderIMGUITool.buildLabelData();
              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testFontTextureProgramIndexBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      (labelColor1, labelAlign1),
                      record,
                    );
                  let record =
                    label(.
                      (labelX2, labelY2, labelWidth2, labelHeight2),
                      labelStr2,
                      (labelColor2, labelAlign2),
                      record,
                    );

                  record;
                },
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
              );
            });

            describe("test draw one label and draw one image", () => {
              let _buildExecFuncData = () => {
                let (
                  (
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    labelColor1,
                    labelAlign1,
                  ),
                  _,
                ) =
                  RenderIMGUITool.buildLabelData();
                let (
                  (
                    (imageX1, imageY1, imageWidth1, imageHeight1),
                    (imageS01, imageT01, imageS11, imageT11),
                    textureId1,
                  ),
                  _,
                  _,
                ) =
                  RenderIMGUITool.buildImageData();

                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let image = apiJsObj##image;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      (labelColor1, labelAlign1),
                      record,
                    );
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );

                  record;
                };
              };

              beforeEach(() => {
                let newRecord =
                  AssetIMGUIService.setCustomImageArr(
                    AssetTool.buildCustomImageDataArr(),
                    record^,
                  );
                record := RenderIMGUITool.prepareFntData(newRecord);
              });

              test("test customTexture program", () =>
                RenderIMGUITool.testCustomTextureProgramIndexBufferData(
                  sandbox,
                  record^,
                  _buildExecFuncData(),
                  [|0, 1, 2, 3, 2, 1|],
                )
              );
              test("test fontTexture program", () =>
                RenderIMGUITool.testFontTextureProgramIndexBufferData(
                  sandbox,
                  record^,
                  _buildExecFuncData(),
                  [|0, 1, 2, 3, 2, 1|],
                )
              );
            });

            test("test draw two images of different textures", () => {
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
                _,
              ) =
                RenderIMGUITool.buildImageData();
              let record =
                AssetIMGUIService.setCustomImageArr(
                  AssetTool.buildCustomImageDataArr(),
                  record^,
                );
              let record = RenderIMGUITool.prepareFntData(record);

              RenderIMGUITool.testCustomTextureProgramIndexBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );
                  let record =
                    image(.
                      (imageX2, imageY2, imageWidth2, imageHeight2),
                      (imageS02, imageT02, imageS12, imageT12),
                      textureId2,
                      record,
                    );

                  record;
                },
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
              );
            });
            test("test draw three images of the same texture", () => {
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
                  record^,
                );
              let record = RenderIMGUITool.prepareFntData(record);

              RenderIMGUITool.testCustomTextureProgramIndexBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let image = apiJsObj##image;
                  let record =
                    image(.
                      (imageX1, imageY1, imageWidth1, imageHeight1),
                      (imageS01, imageT01, imageS11, imageT11),
                      textureId1,
                      record,
                    );
                  let record =
                    image(.
                      (imageX2, imageY2, imageWidth2, imageHeight2),
                      (imageS02, imageT02, imageS12, imageT12),
                      textureId1,
                      record,
                    );
                  let record =
                    image(.
                      (imageX3, imageY3, imageWidth3, imageHeight3),
                      (imageS03, imageT03, imageS13, imageT13),
                      textureId1,
                      record,
                    );

                  record;
                },
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5, 8, 9, 10, 11, 10, 9|],
              );
            });
          })
        );
        /* describe("fix bug", () =>
             test("font texture draw data's order should keep no change", () => {
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
               let ((buttonX1, buttonY1, buttonWidth1, buttonHeight1), str1) =
                 ButtonIMGUITool.buildButtonData1();
               let ((buttonX2, buttonY2, buttonWidth2, buttonHeight2), str2) =
                 ButtonIMGUITool.buildButtonData2();

               let record = RenderIMGUITool.prepareFntData(record^);

               RenderIMGUITool.testCustomTextureProgramIndexBufferData(
                 sandbox,
                 record,
                 (. _, apiJsObj, record) => {
                   let apiJsObj = Obj.magic(apiJsObj);
                   let radioButton = apiJsObj##radioButton;
                   let button = apiJsObj##button;
                   let (record, _) =
                     radioButton(.
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
                       0,
                       group1,
                       record,
                     );

                   let (record, _) =
                     radioButton(.
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
                       0,
                       group1,
                       record,
                     );

                   let (record, _) =
                     button(.
                       (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                       str1,
                       record,
                     );

                   let (record, _) =
                     button(.
                       (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                       str2,
                       record,
                     );

                   record;
                 },
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
                   41,
                   42,
                   43,
                   44,
                   43,
                   42,
                   47,
                   46,
                   45,
                   48,
                   47,
                   45,
                   49,
                   48,
                   45,
                   50,
                   49,
                   45,
                   51,
                   50,
                   45,
                   52,
                   51,
                   45,
                   53,
                   52,
                   45,
                   54,
                   53,
                   45,
                   55,
                   54,
                   45,
                   58,
                   57,
                   56,
                   59,
                   58,
                   56,
                   60,
                   59,
                   56,
                   61,
                   60,
                   56,
                   62,
                   61,
                   56,
                   63,
                   62,
                   56,
                   64,
                   63,
                   56,
                   65,
                   64,
                   56,
                   66,
                   65,
                   56,
                   67,
                   68,
                   69,
                   70,
                   69,
                   68,
                   71,
                   72,
                   73,
                   74,
                   73,
                   72,
                   77,
                   76,
                   75,
                   78,
                   77,
                   75,
                   79,
                   78,
                   75,
                   80,
                   79,
                   75,
                   81,
                   80,
                   75,
                   82,
                   81,
                   75,
                   83,
                   82,
                   75,
                   84,
                   83,
                   75,
                   85,
                   84,
                   75,
                   86,
                   87,
                   88,
                   89,
                   88,
                   87,
                   90,
                   91,
                   92,
                   93,
                   92,
                   91,
                   94,
                   95,
                   96,
                   97,
                   96,
                   95,
                   98,
                   99,
                   100,
                   101,
                   100,
                   99,
                   102,
                   103,
                   104,
                   105,
                   104,
                   103,
                   106,
                   107,
                   108,
                   109,
                   108,
                   107,
                 |],
               );
             })
           ); */
      });

      describe("set gl state", () => {
        test("set after use program", () => {
          let getExtension = _buildNoVAOExtension(sandbox);
          let program = Obj.magic(1);
          let createProgram = createEmptyStubWithJsObjSandbox(sandbox);
          createProgram |> returns(program);
          let useProgram = createEmptyStubWithJsObjSandbox(sandbox);
          let blendFunc = createEmptyStubWithJsObjSandbox(sandbox);
          let gl =
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~getExtension,
              ~createProgram,
              ~useProgram,
              ~blendFunc,
              (),
            )
            |> Obj.magic;

          let _ = _prepareAndExecEmptyExecFuncData(gl);

          blendFunc
          |> getCall(0)
          |> expect
          |> toCalledAfter(useProgram |> getCall(0));
        });

        test("disable depth test", () => {
          let getExtension = _buildNoVAOExtension(sandbox);
          let getDepthTest = Obj.magic(1);
          let disable = createEmptyStubWithJsObjSandbox(sandbox);
          let gl =
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~getExtension,
              ~disable,
              ~getDepthTest,
              (),
            )
            |> Obj.magic;

          let _ = _prepareAndExecEmptyExecFuncData(gl);

          disable |> withOneArg(getDepthTest) |> getCallCount |> expect >= 1;
        });
        test("enable blend", () => {
          let getExtension = _buildNoVAOExtension(sandbox);
          let getBlend = Obj.magic(1);
          let getSrcAlpha = Obj.magic(2);
          let getOneMinusSrcAlpha = Obj.magic(3);
          let enable = createEmptyStubWithJsObjSandbox(sandbox);
          let blendFunc = createEmptyStubWithJsObjSandbox(sandbox);
          let gl =
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~getExtension,
              ~getBlend,
              ~getSrcAlpha,
              ~getOneMinusSrcAlpha,
              ~enable,
              ~blendFunc,
              (),
            )
            |> Obj.magic;

          let _ = _prepareAndExecEmptyExecFuncData(gl);

          (
            enable |> withOneArg(getBlend) |> getCallCount,
            blendFunc
            |> withTwoArgs(getSrcAlpha, getOneMinusSrcAlpha)
            |> getCallCount,
          )
          |> expect == (1, 1);
        });
      });

      describe("send uniform data", () => {
        describe("bind texture and send texture data", () =>
          test("test draw two labels and three images", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let texture2D = 1;
            let location = 10;
            let program = Obj.magic(1);
            let getUniformLocation = createEmptyStubWithJsObjSandbox(sandbox);
            getUniformLocation
            |> withTwoArgs(program, "u_sampler2D")
            |> returns(location);
            let createProgram = createEmptyStubWithJsObjSandbox(sandbox);
            createProgram |> returns(program);
            let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
            let uniform1i = createEmptyStubWithJsObjSandbox(sandbox);
            let fontTexture = Obj.magic(21);
            let customTexture1 = Obj.magic(22);
            let customTexture2 = Obj.magic(23);
            let customTexture3 = Obj.magic(24);
            let createTexture = createEmptyStubWithJsObjSandbox(sandbox);
            createTexture |> onCall(0) |> returns(fontTexture);
            createTexture |> onCall(1) |> returns(customTexture1);
            createTexture |> onCall(2) |> returns(customTexture2);
            createTexture |> onCall(3) |> returns(customTexture3);
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getExtension,
                ~texture2D,
                ~getUniformLocation,
                ~createProgram,
                ~bindTexture,
                ~uniform1i,
                ~createTexture,
                (),
              )
              |> Obj.magic;
            let canvasWidth = 1000;
            let canvasHeight = 500;
            let (record, _) = _drawTwoLabelsAndThreeImages();
            let record = RenderIMGUITool.prepareFntData(record);

            let record =
              ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

            let bindFontTextureCallCountAfterInit =
              bindTexture
              |> withTwoArgs(texture2D, fontTexture)
              |> getCallCount;
            let bindCustomTexture1CallCountAfterInit =
              bindTexture
              |> withTwoArgs(texture2D, customTexture1)
              |> getCallCount;
            let bindCustomTexture2CallCountAfterInit =
              bindTexture
              |> withTwoArgs(texture2D, customTexture2)
              |> getCallCount;
            let bindCustomTexture3CallCountAfterInit =
              bindTexture
              |> withTwoArgs(texture2D, customTexture3)
              |> getCallCount;

            let record =
              ManageIMGUIAPI.render(
                gl,
                RenderIMGUITool.buildIOData(),
                record,
              );

            (
              (
                (
                  bindTexture
                  |> withTwoArgs(texture2D, customTexture1)
                  |> getCallCount
                )
                - bindCustomTexture1CallCountAfterInit,
                (
                  bindTexture
                  |> withTwoArgs(texture2D, customTexture2)
                  |> getCallCount
                )
                - bindCustomTexture2CallCountAfterInit,
                (
                  bindTexture
                  |> withTwoArgs(texture2D, customTexture3)
                  |> getCallCount
                )
                - bindCustomTexture3CallCountAfterInit,
                (
                  bindTexture
                  |> withTwoArgs(texture2D, fontTexture)
                  |> getCallCount
                )
                - bindFontTextureCallCountAfterInit,
              ),
              uniform1i |> withTwoArgs(location, 0) |> getCallCount,
            )
            |> expect == ((1, 1, 1, 1), 2);
          })
        );

        describe("draw", () =>
          describe(
            {|
              1.draw all custom textures;
              2.draw font texture;
              3.draw no texture;
              |},
            () => {
              let _drawOneLabelAndTwoImages = () => {
                let (
                  (
                    (labelX1, labelY1, labelWidth1, labelHeight1),
                    labelStr1,
                    labelColor1,
                    labelAlign1,
                  ),
                  (
                    (labelX2, labelY2, labelWidth2, labelHeight2),
                    labelStr2,
                    labelColor2,
                    labelAlign2,
                  ),
                ) =
                  RenderIMGUITool.buildLabelData();
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
                    record^,
                  );

                let record =
                  ExecIMGUITool.addExecFuncData(
                    ~record,
                    ~func=
                      (. _, apiJsObj, record) => {
                        let apiJsObj = Obj.magic(apiJsObj);
                        let label = apiJsObj##label;
                        let image = apiJsObj##image;
                        let record =
                          label(.
                            (labelX1, labelY1, labelWidth1, labelHeight1),
                            labelStr1,
                            (labelColor1, labelAlign1),
                            record,
                          );
                        let record =
                          image(.
                            (imageX1, imageY1, imageWidth1, imageHeight1),
                            (imageS01, imageT01, imageS11, imageT11),
                            textureId1,
                            record,
                          );

                        let record =
                          image(.
                            (imageX2, imageY2, imageWidth2, imageHeight2),
                            (imageS02, imageT02, imageS12, imageT12),
                            textureId2,
                            record,
                          );

                        record;
                      },
                    (),
                  );

                (
                  record,
                  (
                    (
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                    ),
                    (
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
                    ),
                  ),
                );
              };

              test("test bind correct texture before draw", () => {
                let getExtension = _buildNoVAOExtension(sandbox);
                let texture2D = 1;
                let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
                let (
                  (
                    fontTexture,
                    customTexture1,
                    customTexture2,
                    customTexture3,
                  ),
                  createTexture,
                ) =
                  RenderIMGUITool.createCreateGlTextureStub(sandbox);
                let drawElements = createEmptyStubWithJsObjSandbox(sandbox);
                let gl =
                  FakeGlTool.buildFakeGl(
                    ~sandbox,
                    ~getExtension,
                    ~texture2D,
                    ~bindTexture,
                    ~createTexture,
                    ~drawElements,
                    (),
                  )
                  |> Obj.magic;
                let canvasWidth = 1000;
                let canvasHeight = 500;
                let (
                  record,
                  (_, ((_, _, textureId1), (_, _, textureId2))),
                ) =
                  _drawOneLabelAndTwoImages();
                let record = RenderIMGUITool.prepareFntData(record);

                let record =
                  ManageIMGUIAPI.init(
                    gl,
                    (canvasWidth, canvasHeight),
                    record,
                  );

                let bindFontTextureCallCountAfterInit =
                  bindTexture
                  |> withTwoArgs(texture2D, fontTexture)
                  |> getCallCount;
                let bindCustomTexture1CallCountAfterInit =
                  bindTexture
                  |> withTwoArgs(texture2D, customTexture1)
                  |> getCallCount;
                let bindCustomTexture2CallCountAfterInit =
                  bindTexture
                  |> withTwoArgs(texture2D, customTexture2)
                  |> getCallCount;

                let record =
                  ManageIMGUIAPI.render(
                    gl,
                    RenderIMGUITool.buildIOData(),
                    record,
                  );

                (
                  (
                    bindTexture
                    |> withTwoArgs(texture2D, customTexture1)
                    |> getCall(0)
                  )
                  ->(calledBefore(drawElements |> getCall(0))),
                  (
                    bindTexture
                    |> withTwoArgs(texture2D, customTexture2)
                    |> getCall(0)
                  )
                  ->(calledBefore(drawElements |> getCall(1))),
                  (
                    bindTexture
                    |> withTwoArgs(texture2D, fontTexture)
                    |> getCall(bindFontTextureCallCountAfterInit)
                  )
                  ->(calledBefore(drawElements |> getCall(2))),
                )
                |> expect == (true, true, true);
              });

              describe("test draw", () => {
                test("test draw three images", () => {
                  let getExtension = _buildNoVAOExtension(sandbox);
                  let texture2D = 1;
                  let triangles = 2;
                  let unsigned_short = 3;
                  let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
                  let (
                    (
                      fontTexture,
                      customTexture1,
                      customTexture2,
                      customTexture3,
                    ),
                    createTexture,
                  ) =
                    RenderIMGUITool.createCreateGlTextureStub(sandbox);
                  let drawElements = createEmptyStubWithJsObjSandbox(sandbox);
                  let gl =
                    FakeGlTool.buildFakeGl(
                      ~sandbox,
                      ~getExtension,
                      ~texture2D,
                      ~triangles,
                      ~unsigned_short,
                      ~bindTexture,
                      ~createTexture,
                      ~drawElements,
                      (),
                    )
                    |> Obj.magic;
                  let canvasWidth = 1000;
                  let canvasHeight = 500;
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
                      record^,
                    );
                  let record =
                    ExecIMGUITool.addExecFuncData(
                      ~record,
                      ~func=
                        (. _, apiJsObj, record) => {
                          let apiJsObj = Obj.magic(apiJsObj);
                          let image = apiJsObj##image;
                          let record =
                            image(.
                              (imageX1, imageY1, imageWidth1, imageHeight1),
                              (imageS01, imageT01, imageS11, imageT11),
                              textureId1,
                              record,
                            );
                          let record =
                            image(.
                              (imageX2, imageY2, imageWidth2, imageHeight2),
                              (imageS02, imageT02, imageS12, imageT12),
                              textureId1,
                              record,
                            );
                          let record =
                            image(.
                              (imageX3, imageY3, imageWidth3, imageHeight3),
                              (imageS03, imageT03, imageS13, imageT13),
                              textureId3,
                              record,
                            );

                          record;
                        },
                      (),
                    );

                  let record =
                    ManageIMGUIAPI.init(
                      gl,
                      (canvasWidth, canvasHeight),
                      record,
                    );
                  let record =
                    ManageIMGUIAPI.render(
                      gl,
                      RenderIMGUITool.buildIOData(),
                      record,
                    );

                  (
                    drawElements
                    |> withFourArgs(triangles, 6 * 2, unsigned_short, 0)
                    |> getCallCount,
                    drawElements
                    |> withFourArgs(triangles, 6, unsigned_short, 6 * 2 * 2)
                    |> getCallCount,
                  )
                  |> expect == (1, 1);
                });

                test("test draw one label and draw two images", () => {
                  let getExtension = _buildNoVAOExtension(sandbox);
                  let texture2D = 1;
                  let triangles = 2;
                  let unsigned_short = 3;
                  let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
                  let (
                    (
                      fontTexture,
                      customTexture1,
                      customTexture2,
                      customTexture3,
                    ),
                    createTexture,
                  ) =
                    RenderIMGUITool.createCreateGlTextureStub(sandbox);
                  let drawElements = createEmptyStubWithJsObjSandbox(sandbox);
                  let gl =
                    FakeGlTool.buildFakeGl(
                      ~sandbox,
                      ~getExtension,
                      ~texture2D,
                      ~triangles,
                      ~unsigned_short,
                      ~bindTexture,
                      ~createTexture,
                      ~drawElements,
                      (),
                    )
                    |> Obj.magic;
                  let canvasWidth = 1000;
                  let canvasHeight = 500;
                  let (record, _) = _drawOneLabelAndTwoImages();
                  let record = RenderIMGUITool.prepareFntData(record);

                  let record =
                    ManageIMGUIAPI.init(
                      gl,
                      (canvasWidth, canvasHeight),
                      record,
                    );
                  let record =
                    ManageIMGUIAPI.render(
                      gl,
                      RenderIMGUITool.buildIOData(),
                      record,
                    );

                  (
                    drawElements
                    |> withFourArgs(triangles, 6 * 1, unsigned_short, 0)
                    |> getCallCount,
                    drawElements
                    |> withFourArgs(triangles, 6 * 1, unsigned_short, 12)
                    |> getCallCount,
                  )
                  |> expect == (2, 1);
                });
              });
            },
          )
        );
      });
    });
  });