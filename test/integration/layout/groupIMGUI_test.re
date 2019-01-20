open Wonder_jest;

let _ =
  describe("group", () => {
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

    describe(
      "the controls in group should positioned based on the group’s top-left corner instead of the screen’s top-left corner",
      () => {
        let _testWithIMGUIFunc =
            (bufferData, (testBufferDataFunc, imguiFunc), record) => {
          let record = RenderIMGUITool.prepareFntData(record);

          testBufferDataFunc(sandbox, record, imguiFunc, bufferData);
        };

        describe("test button", () => {
          let _test = (bufferData, testBufferDataFunc, record) =>
            _testWithIMGUIFunc(
              bufferData,
              (
                testBufferDataFunc,
                (. _, apiJsObj, record) => {
                  let (
                    (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                    str1,
                  ) =
                    ButtonIMGUITool.buildButtonData1();

                  let apiJsObj = Obj.magic(apiJsObj);
                  let buttonFunc = apiJsObj##button;
                  let beginGroup = apiJsObj##beginGroup;
                  let endGroup = apiJsObj##endGroup;

                  let record =
                    GroupLayoutIMGUIAPI.beginGroup(. (100, 200), record);

                  let (record, isButtonClick) =
                    buttonFunc(.
                      (buttonX1, buttonY1, buttonWidth1, buttonHeight1),
                      str1,
                      record,
                    );

                  let record = GroupLayoutIMGUIAPI.endGroup(. record);

                  record;
                },
              ),
              record,
            );

          test("test position buffer data", () =>
            _test(
              [|
                135.,
                245.,
                135.,
                450.,
                240.,
                245.,
                240.,
                450.,
                146.,
                319.,
                146.,
                374.,
                191.,
                319.,
                191.,
                374.,
                189.,
                332.,
                189.,
                374.,
                233.,
                332.,
                233.,
                374.,
              |],
              RenderIMGUITool.testPositionBufferData,
              record^,
            )
          );
        });

        describe("test nested group", () =>
          describe("test button", () => {
            let _test = (bufferData, testBufferDataFunc, record) =>
              _testWithIMGUIFunc(
                bufferData,
                (
                  testBufferDataFunc,
                  (. _, apiJsObj, record) => {
                    let (
                      (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                      str2,
                    ) =
                      ButtonIMGUITool.buildButtonData2();

                    let apiJsObj = Obj.magic(apiJsObj);
                    let buttonFunc = apiJsObj##button;
                    let beginGroup = apiJsObj##beginGroup;
                    let endGroup = apiJsObj##endGroup;

                    let record =
                      GroupLayoutIMGUIAPI.beginGroup(. (100, 200), record);

                    let record =
                      GroupLayoutIMGUIAPI.beginGroup(. (100, 200), record);

                    let (record, isButtonClick) =
                      buttonFunc(.
                        (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                        str2,
                        record,
                      );

                    let record = GroupLayoutIMGUIAPI.endGroup(. record);

                    let (record, isButtonClick) =
                      buttonFunc(.
                        (buttonX2, buttonY2, buttonWidth2, buttonHeight2),
                        str2,
                        record,
                      );

                    let record = GroupLayoutIMGUIAPI.endGroup(. record);

                    record;
                  },
                ),
                record,
              );

            test("test position buffer data", () =>
              _test(
                [|
                  236.,
                  446.,
                  236.,
                  552.,
                  342.,
                  446.,
                  342.,
                  552.,
                  273.,
                  471.,
                  273.,
                  526.,
                  306.,
                  471.,
                  306.,
                  526.,
                  136.,
                  246.,
                  136.,
                  352.,
                  242.,
                  246.,
                  242.,
                  352.,
                  173.,
                  271.,
                  173.,
                  326.,
                  206.,
                  271.,
                  206.,
                  326.,
                |],
                RenderIMGUITool.testPositionBufferData,
                record^,
              )
            );
          })
        );
      },
    );
  });