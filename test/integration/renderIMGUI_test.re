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
            labelAlign1,
          ),
          (
            (labelX2, labelY2, labelWidth2, labelHeight2),
            labelStr2,
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
          record
          |> ManageIMGUIAPI.setIMGUIFunc(
               Obj.magic(1),
               (_, apiJsObj, record) => {
                 let label = apiJsObj##label;
                 let image = apiJsObj##image;
                 let record =
                   label(.
                     (labelX1, labelY1, labelWidth1, labelHeight1),
                     labelStr1,
                     labelAlign1,
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
                     labelAlign2,
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

      let _prepareAndExecEmptyIMGUIFunc = gl => {
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(), (_, _, record) =>
               record
             );

        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        (record, canvasWidth, canvasHeight);
      };

      test("clear drawDataArr", () => {
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
        let getExtension = RenderIMGUITool.buildNoVAOExtension(sandbox);
        let gl =
          FakeGlTool.buildFakeGl(~sandbox, ~getExtension, ()) |> Obj.magic;
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(),
               (_, apiJsObj, record) => {
                 let imageFunc = apiJsObj##image;
                 let record =
                   imageFunc(.
                     (imageX1, imageY1, imageWidth1, imageHeight1),
                     (imageS01, imageT01, imageS11, imageT11),
                     textureId1,
                     record,
                   );

                 record;
               },
             );
        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        record.drawDataArr |> Js.Array.length |> expect == 1;
      });

      test("reset currentFontTextureDrawDataBaseIndex", () => {
        let (
          (
            (labelX1, labelY1, labelWidth1, labelHeight1),
            labelStr1,
            labelAlign1,
          ),
          (
            (labelX2, labelY2, labelWidth2, labelHeight2),
            labelStr2,
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
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(),
               (_, apiJsObj, record) => {
                 let label = apiJsObj##label;
                 let record =
                   label(.
                     (labelX1, labelY1, labelWidth1, labelHeight1),
                     labelStr1,
                     labelAlign1,
                     record,
                   );

                 record;
               },
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
        |> getCall(bufferDataCallCountAfterFirstRender + 3)
        |> expect
        |> toCalledWith([|
             element_array_buffer,
             Uint16Array.make([|0, 1, 2, 3, 2, 1|]) |> Obj.magic,
             dynamic_draw,
           |]);
      });

      test("if has no imguiFunc, not error", () => {
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
        let record =
          record
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(), (_, apiJsObj, record) =>
               record
             );
        let record =
          ManageIMGUIAPI.render(gl, RenderIMGUITool.buildIOData(), record);

        bindVertexArrayOES |> expect |> toCalledWith([|Js.Nullable.null|]);
      });

      describe("buffer all data", () => {
        let _testEnableVertexAttribArrayAndVertexAttribPointer = (name, size) => {
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
              labelAlign1,
            ),
            (
              (labelX2, labelY2, labelWidth2, labelHeight2),
              labelStr2,
              labelAlign2,
            ),
          ) =
            RenderIMGUITool.buildLabelData();
          let canvasWidth = 1000;
          let canvasHeight = 500;
          let record =
            record^
            |> ManageIMGUIAPI.setIMGUIFunc(
                 RenderIMGUITool.buildCustomData(),
                 (_, apiJsObj, record) => {
                   let label = apiJsObj##label;
                   let record =
                     label(.
                       (labelX1, labelY1, labelWidth1, labelHeight1),
                       labelStr1,
                       labelAlign1,
                       record,
                     );

                   record;
                 },
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
          |> expect == (1, 1);
        };

        let _getPositionBufferData = () => (
          (
            [|12., 49., 12., 92., 59., 49., 59., 92.|],
            [|22., 49., 22., 92., 69., 49., 69., 92.|],
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

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
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

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
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

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
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
                image2Data
                |> Js.Array.concat(image1Data)
                |> Js.Array.concat(image3Data),
              );
            });

            test("test draw two labels and draw three images", () => {
              let (
                (
                  (labelX1, labelY1, labelWidth1, labelHeight1),
                  labelStr1,
                  labelAlign1,
                ),
                (
                  (labelX2, labelY2, labelWidth2, labelHeight2),
                  labelStr2,
                  labelAlign2,
                ),
              ) =
                RenderIMGUITool.buildLabelData();
              let (
                (label1Data, label2Data),
                (image1Data, image2Data, image3Data),
              ) =
                _getPositionBufferData();

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

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
                  let label = apiJsObj##label;
                  let image = apiJsObj##image;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      labelAlign1,
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
                      labelAlign2,
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
                label1Data
                |> Js.Array.concat(label2Data)
                |> Js.Array.concat(image2Data)
                |> Js.Array.concat(image1Data)
                |> Js.Array.concat(image3Data),
              );
            });
          })
        );

        describe("enableVertexAttribArray and vertexAttribPointer", () =>
          test("test", () =>
            _testEnableVertexAttribArrayAndVertexAttribPointer(
              "a_position",
              2,
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

              RenderIMGUITool.testColorBufferData(
                sandbox,
                record^,
                (_, apiJsObj, record) => {
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
              _testEnableVertexAttribArrayAndVertexAttribPointer("a_color", 3)
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

              RenderIMGUITool.testTexCoordBufferData(
                sandbox,
                record^,
                (_, apiJsObj, record) => {
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
                  labelAlign1,
                ),
                (
                  (labelX2, labelY2, labelWidth2, labelHeight2),
                  labelStr2,
                  labelAlign2,
                ),
              ) =
                RenderIMGUITool.buildLabelData();
              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testIndexBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      labelAlign1,
                      record,
                    );
                  let record =
                    label(.
                      (labelX2, labelY2, labelWidth2, labelHeight2),
                      labelStr2,
                      labelAlign2,
                      record,
                    );

                  record;
                },
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
              );
            });

            test("test draw one label and draw one image", () => {
              let (
                (
                  (labelX1, labelY1, labelWidth1, labelHeight1),
                  labelStr1,
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
              let record =
                AssetIMGUIService.setCustomImageArr(
                  AssetTool.buildCustomImageDataArr(),
                  record^,
                );
              let record = RenderIMGUITool.prepareFntData(record);

              RenderIMGUITool.testIndexBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
                  let label = apiJsObj##label;
                  let image = apiJsObj##image;
                  let record =
                    label(.
                      (labelX1, labelY1, labelWidth1, labelHeight1),
                      labelStr1,
                      labelAlign1,
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
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5|],
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

              RenderIMGUITool.testIndexBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
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

              RenderIMGUITool.testIndexBufferData(
                sandbox,
                record,
                (_, apiJsObj, record) => {
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

        describe("fix bug", () =>
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

            RenderIMGUITool.testIndexBufferData(
              sandbox,
              record,
              (_, apiJsObj, record) => {
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
                36,
                35,
                34,
                37,
                36,
                34,
                38,
                37,
                34,
                39,
                38,
                34,
                40,
                39,
                34,
                41,
                40,
                34,
                42,
                41,
                34,
                43,
                42,
                34,
                44,
                43,
                34,
                45,
                46,
                47,
                48,
                47,
                46,
                49,
                50,
                51,
                52,
                51,
                50,
                55,
                54,
                53,
                56,
                55,
                53,
                57,
                56,
                53,
                58,
                57,
                53,
                59,
                58,
                53,
                60,
                59,
                53,
                61,
                60,
                53,
                62,
                61,
                53,
                63,
                62,
                53,
                64,
                65,
                66,
                67,
                66,
                65,
                68,
                69,
                70,
                71,
                70,
                69,
                72,
                73,
                74,
                75,
                74,
                73,
                76,
                77,
                78,
                79,
                78,
                77,
                80,
                81,
                82,
                83,
                82,
                81,
                84,
                85,
                86,
                87,
                86,
                85,
              |],
            );
          })
        );
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

          let _ = _prepareAndExecEmptyIMGUIFunc(gl);

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

          let _ = _prepareAndExecEmptyIMGUIFunc(gl);

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

          let _ = _prepareAndExecEmptyIMGUIFunc(gl);

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
        test("send u_projectionMat data", () => {
          let getExtension = _buildNoVAOExtension(sandbox);
          let float = 1;
          let location = 10;
          let program = Obj.magic(1);
          let getUniformLocation = createEmptyStubWithJsObjSandbox(sandbox);
          getUniformLocation
          |> withTwoArgs(program, "u_projectionMat")
          |> returns(location);
          let createProgram = createEmptyStubWithJsObjSandbox(sandbox);
          createProgram |> returns(program);
          let uniformMatrix4fv = createEmptyStubWithJsObjSandbox(sandbox);
          let gl =
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~float,
              ~getExtension,
              ~createProgram,
              ~getUniformLocation,
              ~uniformMatrix4fv,
              (),
            )
            |> Obj.magic;
          let (record, canvasWidth, canvasHeight) =
            _prepareAndExecEmptyIMGUIFunc(gl);

          uniformMatrix4fv
          |> expect
          |> toCalledWith([|
               location |> Obj.magic,
               false |> Obj.magic,
               Float32Array.make([|
                 0.0020000000949949026,
                 0.,
                 0.,
                 0.,
                 0.,
                 (-0.004000000189989805),
                 0.,
                 0.,
                 0.,
                 0.,
                 (-1.),
                 0.,
                 (-1.),
                 1.,
                 (-0.),
                 1.,
               |]),
             |]);
        });

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
            |> expect == ((1, 1, 1, 1), 1);
          })
        );

        describe("draw", () => {
          test("test bind correct texture before draw", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let texture2D = 1;
            let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
            let (
              (fontTexture, customTexture1, customTexture2, customTexture3),
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
              (
                _,
                (
                  (_, _, textureId1),
                  (_, _, textureId2),
                  (_, _, textureId3),
                ),
              ),
            ) =
              _drawTwoLabelsAndThreeImages();
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
              bindTexture
              |> withTwoArgs(texture2D, fontTexture)
              |> getCall(bindFontTextureCallCountAfterInit)
              |. calledBefore(drawElements |> getCall(0)),
              bindTexture
              |> withTwoArgs(texture2D, customTexture1)
              |> getCall(0)
              |. calledBefore(drawElements |> getCall(1)),
              bindTexture
              |> withTwoArgs(texture2D, customTexture2)
              |> getCall(0)
              |. calledBefore(drawElements |> getCall(2)),
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
                (fontTexture, customTexture1, customTexture2, customTexture3),
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
                record
                |> ManageIMGUIAPI.setIMGUIFunc(
                     RenderIMGUITool.buildCustomData(),
                     (_, apiJsObj, record) => {
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
                   );

              let record =
                ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);
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
                |> withFourArgs(triangles, 6 * 2, unsigned_short, 6 * 2)
                |> getCallCount,
              )
              |> expect == (1, 1);
            });

            test("test draw two labels and draw three images", () => {
              let getExtension = _buildNoVAOExtension(sandbox);
              let texture2D = 1;
              let triangles = 2;
              let unsigned_short = 3;
              let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
              let (
                (fontTexture, customTexture1, customTexture2, customTexture3),
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
              let (record, _) = _drawTwoLabelsAndThreeImages();
              let record = RenderIMGUITool.prepareFntData(record);

              let record =
                ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);
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
                |> withFourArgs(triangles, 6 * 1, unsigned_short, 24)
                |> getCallCount,
                drawElements
                |> withFourArgs(triangles, 6, unsigned_short, 36)
                |> getCallCount,
                drawElements
                |> withFourArgs(triangles, 6, unsigned_short, 48)
                |> getCallCount,
              )
              |> expect == (1, 1, 1, 1);
            });
          });
        });
        describe("restore gl state", () => {
          test("bind last vao buffers", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let bindBuffer = createEmptyStubWithJsObjSandbox(sandbox);
            let array_buffer = 1;
            let element_array_buffer = 2;
            let getBindingElementArrayBuffer = 3;
            let getBindingArrayBuffer = 4;
            let lastElementArrayBuffer = 10;
            let lastArrayBuffer = 11;
            let getParameter = createEmptyStubWithJsObjSandbox(sandbox);
            getParameter
            |> withOneArg(getBindingElementArrayBuffer)
            |> returns(lastElementArrayBuffer);
            getParameter
            |> withOneArg(getBindingArrayBuffer)
            |> returns(lastArrayBuffer);
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getExtension,
                ~array_buffer,
                ~element_array_buffer,
                ~getBindingElementArrayBuffer,
                ~getBindingArrayBuffer,
                ~bindBuffer,
                ~getParameter,
                (),
              )
              |> Obj.magic;

            let _ = _prepareAndExecEmptyIMGUIFunc(gl);

            (
              bindBuffer
              |> withTwoArgs(element_array_buffer, lastElementArrayBuffer)
              |> getCallCount,
              bindBuffer
              |> withTwoArgs(array_buffer, lastArrayBuffer)
              |> getCallCount,
            )
            |> expect == (1, 1);
          });
          test("use last program", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let getCurrentProgram = 3;
            let useProgram = createEmptyStubWithJsObjSandbox(sandbox);
            let lastProgram = 11;
            let getParameter = createEmptyStubWithJsObjSandbox(sandbox);
            getParameter
            |> withOneArg(getCurrentProgram)
            |> returns(lastProgram);
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getExtension,
                ~getCurrentProgram,
                ~useProgram,
                ~getParameter,
                (),
              )
              |> Obj.magic;

            let _ = _prepareAndExecEmptyIMGUIFunc(gl);

            useProgram
            |> withOneArg(lastProgram)
            |> getCallCount
            |> expect == 1;
          });
          test("bind last textureId2d", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let texture2D = 2;
            let getBindingTexture2D = 3;
            let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
            let lastTexture = 11;
            let getParameter = createEmptyStubWithJsObjSandbox(sandbox);
            getParameter
            |> withOneArg(getBindingTexture2D)
            |> returns(Js.Nullable.return(lastTexture));
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~texture2D,
                ~getExtension,
                ~getBindingTexture2D,
                ~bindTexture,
                ~getParameter,
                (),
              )
              |> Obj.magic;

            let _ = _prepareAndExecEmptyIMGUIFunc(gl);

            bindTexture
            |> withTwoArgs(texture2D, lastTexture)
            |> getCallCount
            |> expect == 1;
          });
          test("test enable last depth test and blend", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let getDepthTest = 2;
            let getBlend = 3;
            let isEnabled = createEmptyStubWithJsObjSandbox(sandbox);
            let enable = createEmptyStubWithJsObjSandbox(sandbox);
            let isEnabled = createEmptyStubWithJsObjSandbox(sandbox);
            isEnabled |> withOneArg(getDepthTest) |> returns(true);
            isEnabled |> withOneArg(getBlend) |> returns(true);
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getDepthTest,
                ~getExtension,
                ~getBlend,
                ~enable,
                ~isEnabled,
                (),
              )
              |> Obj.magic;

            let _ = _prepareAndExecEmptyIMGUIFunc(gl);

            (
              enable |> withOneArg(getDepthTest) |> getCallCount,
              enable |> withOneArg(getBlend) |> getCallCount,
            )
            |> expect == (1, 2);
          });
          test("test disable last depth test and blend", () => {
            let getExtension = _buildNoVAOExtension(sandbox);
            let getDepthTest = 2;
            let getBlend = 3;
            let isEnabled = createEmptyStubWithJsObjSandbox(sandbox);
            let enable = createEmptyStubWithJsObjSandbox(sandbox);
            let disable = createEmptyStubWithJsObjSandbox(sandbox);
            let isEnabled = createEmptyStubWithJsObjSandbox(sandbox);
            isEnabled |> withOneArg(getDepthTest) |> returns(false);
            isEnabled |> withOneArg(getBlend) |> returns(false);
            let gl =
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getDepthTest,
                ~getExtension,
                ~getBlend,
                ~disable,
                ~isEnabled,
                (),
              )
              |> Obj.magic;

            let _ = _prepareAndExecEmptyIMGUIFunc(gl);

            (
              disable |> withOneArg(getDepthTest) |> getCallCount,
              disable |> withOneArg(getBlend) |> getCallCount,
            )
            |> expect == (2, 1);
          });
        });
      });
    });
  });