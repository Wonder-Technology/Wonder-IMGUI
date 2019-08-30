open Wonder_jest;

let _ =
  describe("test exec imgui", () => {
    open Expect;
    open Expect.Operators;
    open Sinon;

    let sandbox = getSandboxDefaultVal();

    let record = ref(Obj.magic(0));

    beforeEach(() => {
      sandbox := createSandbox();
      record := ManageIMGUIAPI.createRecord();
    });
    afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));

    describe("addExecFuncData", () =>
      test("add to arr", () => {
        let record =
          ExecIMGUITool.addExecFuncData(~record=record^, ~name="e1", ());
        let record = ExecIMGUITool.addExecFuncData(~record, ~name="e2", ());

        ExecIMGUIAPI.getExecFuncDataArr(record)
        |> Js.Array.length
        |> expect == 2;
      })
    );

    describe("removeExecFuncData", () =>
      test("remove from arr", () => {
        let record =
          ExecIMGUITool.addExecFuncData(~record=record^, ~name="e1", ());
        let record = ExecIMGUITool.addExecFuncData(~record, ~name="e2", ());

        let record = ExecIMGUIAPI.removeExecFuncData("e1", record);

        ExecIMGUIAPI.getExecFuncDataArr(record)
        |> Js.Array.length
        |> expect == 1;
      })
    );

    describe("getCustomData", () =>
      test("get custom data by exec func's name", () => {
        let customData1 = Obj.magic(1);
        let customData2 = Obj.magic(2);

        let record =
          ExecIMGUITool.addExecFuncData(
            ~record=record^,
            ~name="e1",
            ~customData=customData1,
            (),
          );
        let record =
          ExecIMGUITool.addExecFuncData(
            ~record,
            ~name="e2",
            ~customData=customData2,
            (),
          );

        ExecIMGUIAPI.getCustomData("e2", record)
        |> expect == Some(customData2);
      })
    );

    describe("exec func", () =>
      describe("exec all funcs sorted by execOrder ", () =>
        describe("test no texture program->position buffer data", () =>
          test("test", () => {
            let record = record^ |> AssetTool.prepareFontAsset;

            let record = RenderIMGUITool.prepareFntData(record);

            RenderIMGUITool.testBufferDataWithIODataAndAfterInitFuncAndSetExecFuncData(
              sandbox,
              8,
              RenderIMGUITool.buildIOData(),
              record,
              (
                record => {
                  let record =
                    ExecIMGUITool.addExecFuncData(
                      ~record,
                      ~func=
                        (. _, apiJsObj, record) => {
                          let ((boxX1, boxY1, boxWidth1, boxHeight1), color) =
                            BoxIMGUITool.buildBoxData1();

                          let apiJsObj = Obj.magic(apiJsObj);

                          let box = apiJsObj##box;

                          let record =
                            box(. (0, 0, 100, 200), color, record);

                          record;
                        },
                      ~execOrder=2,
                      (),
                    );

                  let record =
                    ExecIMGUITool.addExecFuncData(
                      ~record,
                      ~func=
                        (. _, apiJsObj, record) => {
                          let ((boxX1, boxY1, boxWidth1, boxHeight1), color) =
                            BoxIMGUITool.buildBoxData1();

                          let apiJsObj = Obj.magic(apiJsObj);

                          let box = apiJsObj##box;

                          let record =
                            box(. (50, 50, 100, 200), color, record);

                          record;
                        },
                      ~execOrder=1,
                      (),
                    );

                  record;
                },
                record => record,
              ),
              [|
                50.,
                50.,
                50.,
                250.,
                150.,
                50.,
                150.,
                250.,
                0.,
                0.,
                0.,
                200.,
                100.,
                0.,
                100.,
                200.,
              |],
            );
          })
        )
      )
    );
  });