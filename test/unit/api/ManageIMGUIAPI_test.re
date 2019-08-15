open Wonder_jest;

open Js.Typed_array;

let _ =
  describe("ManageIMGUIAPI", () => {
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

    describe("sendCustomTextureProgramUniformProjectionMatData", () => {
      let _buildNoVAOExtension = sandbox =>
        RenderIMGUITool.buildNoVAOExtension(sandbox);

      let _prepareEmptyIMGUIFunc = gl => {
        let canvasWidth = 1000;
        let canvasHeight = 500;
        let record =
          record^
          |> ManageIMGUIAPI.setIMGUIFunc(
               RenderIMGUITool.buildCustomData(), (. _, _, record) =>
               record
             );

        let record =
          ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

        (record, canvasWidth, canvasHeight);
      };

      test("use program", () => {
        let getExtension = _buildNoVAOExtension(sandbox);
        let program = Obj.magic(1);
        let useProgram = createEmptyStubWithJsObjSandbox(sandbox);
        let createProgram = createEmptyStubWithJsObjSandbox(sandbox);
        createProgram |> returns(program);
        let gl =
          FakeGlTool.buildFakeGl(
            ~sandbox,
            ~getExtension,
            ~useProgram,
            ~createProgram,
            (),
          )
          |> Obj.magic;
        let (record, canvasWidth, canvasHeight) = _prepareEmptyIMGUIFunc(gl);

        let record =
          ManageIMGUIAPI.sendCustomTextureProgramUniformProjectionMatData(
            gl,
            (0, 0),
            record,
          );

        useProgram |> getCall(1) |> expect |> toCalledWith([|program|]);
      });

      describe("send u_projectionMat", () =>
        test("build value by canvas size", () => {
          let getExtension = _buildNoVAOExtension(sandbox);
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
              ~getExtension,
              ~createProgram,
              ~getUniformLocation,
              ~uniformMatrix4fv,
              (),
            )
            |> Obj.magic;
          let (record, canvasWidth, canvasHeight) =
            _prepareEmptyIMGUIFunc(gl);

          let record =
            ManageIMGUIAPI.sendCustomTextureProgramUniformProjectionMatData(
              gl,
              (500, 250),
              record,
            );

          uniformMatrix4fv
          |> getCall((uniformMatrix4fv |> getCallCount) - 1)
          |> expect
          |> toCalledWith([|
               location |> Obj.magic,
               false |> Obj.magic,
               Float32Array.make([|
                 0.004000000189989805,
                 0.,
                 0.,
                 0.,
                 0.,
                 (-0.00800000037997961),
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
        })
      );
    });
  });