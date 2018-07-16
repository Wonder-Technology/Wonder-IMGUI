open Wonder_jest;

open Js.Typed_array;

open WonderBsMost;

open Js.Promise;

let _ =
  describe("initIMGUI", () => {
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

    describe("init", () =>
      describe("create program", () => {
        test("test vs shader source", () => {
          let shaderSource = createEmptyStubWithJsObjSandbox(sandbox);

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(~sandbox, ~shaderSource, ()) |> Obj.magic,
              record^,
            );

          shaderSource
          |> getCall(0)
          |> getArgs
          |. List.nth(1)
          |> expect == ShaderData.vs;
        });
        test("test fs shader source", () => {
          let shaderSource = createEmptyStubWithJsObjSandbox(sandbox);

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(~sandbox, ~shaderSource, ()) |> Obj.magic,
              record^,
            );

          shaderSource
          |> getCall(1)
          |> getArgs
          |. List.nth(1)
          |> expect == ShaderData.fs;
        });
        test("set program to record", () => {
          let program = Obj.magic(1);
          let createProgram =
            createEmptyStubWithJsObjSandbox(sandbox)
            |> onCall(0)
            |> returns(program);

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(~sandbox, ~createProgram, ())
              |> Obj.magic,
              record^,
            );

          RecordIMGUITool.unsafeGetWebglData(record).program |> expect == program;
        });
      })
    );
    describe("create and init vao buffers", () => {
      describe("create and init position buffer", () =>
        test("test", () => {
          let buffer = Obj.magic(1);
          let createBuffer =
            createEmptyStubWithJsObjSandbox(sandbox)
            |> onCall(0)
            |> returns(buffer);
          let bindBuffer = createEmptyStubWithJsObjSandbox(sandbox);
          let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
          let array_buffer = 2;
          let dynamic_draw = 3;

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~array_buffer,
                ~dynamic_draw,
                ~bindBuffer,
                ~createBuffer,
                ~bufferData,
                (),
              )
              |> Obj.magic,
              record^,
            );

          (
            bindBuffer |> withTwoArgs(array_buffer, buffer) |> getCallCount,
            bufferData |> getCall(0) |> getArgs,
          )
          |>
          expect == (
                      1,
                      [
                        array_buffer,
                        Float32Array.make([||]) |> Obj.magic,
                        dynamic_draw,
                      ],
                    );
        })
      );

      describe("create and init index buffer", () =>
        test("test", () => {
          let buffer = Obj.magic(1);
          let createBuffer =
            createEmptyStubWithJsObjSandbox(sandbox)
            |> onCall(3)
            |> returns(buffer);
          let bindBuffer = createEmptyStubWithJsObjSandbox(sandbox);
          let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
          let element_array_buffer = 2;
          let dynamic_draw = 3;

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~element_array_buffer,
                ~dynamic_draw,
                ~bindBuffer,
                ~createBuffer,
                ~bufferData,
                (),
              )
              |> Obj.magic,
              record^,
            );

          (
            bindBuffer
            |> withTwoArgs(element_array_buffer, buffer)
            |> getCallCount,
            bufferData |> getCall(3) |> getArgs,
          )
          |>
          expect == (
                      1,
                      [
                        element_array_buffer,
                        Uint16Array.make([||]) |> Obj.magic,
                        dynamic_draw,
                      ],
                    );
        })
      );
    });
    /* TODO test more */
    describe("create and init font texture", () => {
      test("test create, bind, texParameteri texture", () => {
        let texture = Obj.magic(1);
        let createTexture =
          createEmptyStubWithJsObjSandbox(sandbox)
          |> onCall(0)
          |> returns(texture);
        let bindTexture = createEmptyStubWithJsObjSandbox(sandbox);
        let texParameteri = createEmptyStubWithJsObjSandbox(sandbox);
        let texture2D = 2;
        let textureMinFilter = 3;
        let textureMagFilter = 4;
        let textureWrapS = 5;
        let textureWrapT = 6;
        let linear = 7;
        let clampToEdge = 8;

        let record =
          ManageIMGUIAPI.init(
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~texture2D,
              ~textureMinFilter,
              ~textureMagFilter,
              ~textureWrapS,
              ~textureWrapT,
              ~linear,
              ~clampToEdge,
              ~createTexture,
              ~bindTexture,
              ~texParameteri,
              (),
            )
            |> Obj.magic,
            record^,
          );

        (
          bindTexture |> withTwoArgs(texture2D, texture) |> getCallCount,
          texParameteri
          |> withThreeArgs(texture2D, textureMinFilter, linear)
          |> getCallCount,
          texParameteri
          |> withThreeArgs(texture2D, textureMagFilter, linear)
          |> getCallCount,
          texParameteri
          |> withThreeArgs(texture2D, textureWrapS, clampToEdge)
          |> getCallCount,
          texParameteri
          |> withThreeArgs(texture2D, textureWrapT, clampToEdge)
          |> getCallCount,
        )
        |> expect == (1, 1, 1, 1, 1);
      });
      test("test texImage2D font texture source", () => {
        let texture = Obj.magic(1);
        let texImage2D = createEmptyStubWithJsObjSandbox(sandbox);
        let texture2D = 2;
        let rgba = 3;
        let unsignedByte = 4;

        let record =
          ManageIMGUIAPI.init(
            FakeGlTool.buildFakeGl(
              ~sandbox,
              ~texture2D,
              ~rgba,
              ~unsignedByte,
              ~texImage2D,
              (),
            )
            |> Obj.magic,
            record^,
          );

        texImage2D
        |> expect
        |> toCalledWith([|
             texture2D,
             0,
             rgba,
             rgba,
             unsignedByte,
             AssetTool.unsafeGetBitmap(record) |> Obj.magic,
           |]);
      });
    });

    describe("get shader location", () => {
      describe("get attribute location", () => {
        let _test = (name, getRecordLocationFunc) => {
          let location = 10;
          let program = Obj.magic(1);

          let getAttribLocation = createEmptyStubWithJsObjSandbox(sandbox);

          getAttribLocation
          |> withTwoArgs(program, name)
          |> returns(location);

          let createProgram =
            createEmptyStubWithJsObjSandbox(sandbox) |> returns(program);

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getAttribLocation,
                ~createProgram,
                (),
              )
              |> Obj.magic,
              record^,
            );

          getRecordLocationFunc(record) |> expect == location;
        };

        test("test get a_position location", () =>
          _test("a_position", record =>
            RecordIMGUITool.unsafeGetWebglData(record).aPositonLocation
          )
        );
        test("test get a_color location", () =>
          _test("a_color", record =>
            RecordIMGUITool.unsafeGetWebglData(record).aColorLocation
          )
        );
        test("test get a_texCoord location", () =>
          _test("a_texCoord", record =>
            RecordIMGUITool.unsafeGetWebglData(record).aTexCoordLocation
          )
        );
      });

      describe("get uniform location", () => {
        let _test = (name, getRecordLocationFunc) => {
          let location = Obj.magic(10);
          let program = Obj.magic(1);

          let getUniformLocation = createEmptyStubWithJsObjSandbox(sandbox);

          getUniformLocation
          |> withTwoArgs(program, name)
          |> returns(location);

          let createProgram =
            createEmptyStubWithJsObjSandbox(sandbox) |> returns(program);

          let record =
            ManageIMGUIAPI.init(
              FakeGlTool.buildFakeGl(
                ~sandbox,
                ~getUniformLocation,
                ~createProgram,
                (),
              )
              |> Obj.magic,
              record^,
            );

          getRecordLocationFunc(record) |> expect == location;
        };

        test("test get u_projectionMat location", () =>
          _test("u_projectionMat", record =>
            RecordIMGUITool.unsafeGetWebglData(record).uProjectionMatLocation
          )
        );
        test("test get u_sampler2D location", () =>
          _test("u_sampler2D", record =>
            RecordIMGUITool.unsafeGetWebglData(record).uSampler2DLocation
          )
        );
      });
    });
  });