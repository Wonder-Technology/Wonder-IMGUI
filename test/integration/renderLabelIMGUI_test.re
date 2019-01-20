open Wonder_jest;

open Js.Typed_array;

open IMGUIType;

open FontType;

let _ =
  describe("render label", () => {
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

    describe("test render", () =>
      describe("buffer all data", () =>
        describe("test draw label(the yAlignment is center)", () => {
          describe("test draw single line", () => {
            let _test = (bufferData, testBufferDataFunc, record) => {
              let labelX = 10;
              let labelY = 20;
              let labelWidth = 200;
              let labelHeight = 200;
              let labelStr = "aaa";
              let labelAlign = Left;

              let record = RenderIMGUITool.prepareFntData(record);

              testBufferDataFunc(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX, labelY, labelWidth, labelHeight),
                      labelStr,
                      labelAlign,
                      record,
                    );

                  record;
                },
                bufferData,
              );
            };

            test("test position buffer data", () =>
              _test(
                [|
                  12.,
                  104.,
                  12.,
                  147.,
                  59.,
                  104.,
                  59.,
                  147.,
                  55.,
                  104.,
                  55.,
                  147.,
                  102.,
                  104.,
                  102.,
                  147.,
                  98.,
                  104.,
                  98.,
                  147.,
                  145.,
                  104.,
                  145.,
                  147.,
                |],
                RenderIMGUITool.testPositionBufferData,
                record^,
              )
            );
            test("test color buffer data", () =>
              _test(
                [|
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
                RenderIMGUITool.testColorBufferData,
                record^,
              )
            );
            test("test texCoord buffer data", () =>
              _test(
                [|
                  0.6693877551020408,
                  0.6544789762340036,
                  0.6693877551020408,
                  0.7330895795246801,
                  0.7653061224489796,
                  0.6544789762340036,
                  0.7653061224489796,
                  0.7330895795246801,
                  0.6693877551020408,
                  0.6544789762340036,
                  0.6693877551020408,
                  0.7330895795246801,
                  0.7653061224489796,
                  0.6544789762340036,
                  0.7653061224489796,
                  0.7330895795246801,
                  0.6693877551020408,
                  0.6544789762340036,
                  0.6693877551020408,
                  0.7330895795246801,
                  0.7653061224489796,
                  0.6544789762340036,
                  0.7653061224489796,
                  0.7330895795246801,
                |],
                RenderIMGUITool.testTexCoordBufferData,
                record^,
              )
            );
            test("test index buffer data", () =>
              _test(
                [|0, 1, 2, 3, 2, 1, 4, 5, 6, 7, 6, 5, 8, 9, 10, 11, 10, 9|],
                RenderIMGUITool.testIndexBufferData,
                record^,
              )
            );
          });

          describe("test draw multi line", () => {
            test("test exceed max width", () => {
              let labelX = 10;
              let labelY = 20;
              let labelWidth = 50;
              let labelHeight = 200;
              let labelStr = "a";
              let labelAlign = Left;

              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX, labelY, labelWidth, labelHeight),
                      labelStr,
                      labelAlign,
                      record,
                    );

                  record;
                },
                [|12., 104., 12., 147., 59., 104., 59., 147.|],
              );
            });
            test("test text has newline char", () => {
              let labelX = 10;
              let labelY = 20;
              let labelWidth = 200;
              let labelHeight = 200;
              let labelStr = "a\nc";
              let labelAlign = Left;

              let record = RenderIMGUITool.prepareFntData(record^);

              RenderIMGUITool.testPositionBufferData(
                sandbox,
                record,
                (. _, apiJsObj, record) => {
                  let apiJsObj = Obj.magic(apiJsObj);
                  let label = apiJsObj##label;
                  let record =
                    label(.
                      (labelX, labelY, labelWidth, labelHeight),
                      labelStr,
                      labelAlign,
                      record,
                    );

                  record;
                },
                [|
                  12.,
                  59.,
                  12.,
                  102.,
                  59.,
                  59.,
                  59.,
                  102.,
                  12.,
                  149.,
                  12.,
                  192.,
                  59.,
                  149.,
                  59.,
                  192.,
                |],
              );
            });

            describe("test text has space char", () => {
              test(
                "if space is in the inner of the line, add corresponding space char",
                () => {
                let labelX = 10;
                let labelY = 20;
                let labelWidth = 300;
                let labelHeight = 200;
                let labelStr = "a  b";
                let labelAlign = Left;

                let record = RenderIMGUITool.prepareFntData(record^);

                RenderIMGUITool.testPositionBufferData(
                  sandbox,
                  record,
                  (. _, apiJsObj, record) => {
                    let apiJsObj = Obj.magic(apiJsObj);
                    let label = apiJsObj##label;
                    let record =
                      label(.
                        (labelX, labelY, labelWidth, labelHeight),
                        labelStr,
                        labelAlign,
                        record,
                      );

                    record;
                  },
                  [|
                    12.,
                    104.,
                    12.,
                    147.,
                    59.,
                    104.,
                    59.,
                    147.,
                    53.,
                    146.,
                    53.,
                    146.,
                    53.,
                    146.,
                    53.,
                    146.,
                    74.,
                    146.,
                    74.,
                    146.,
                    74.,
                    146.,
                    74.,
                    146.,
                    99.,
                    92.,
                    99.,
                    147.,
                    144.,
                    92.,
                    144.,
                    147.,
                  |],
                );
              });
              test("eat whitespace at start of line", () => {
                let labelX = 10;
                let labelY = 20;
                let labelWidth = 300;
                let labelHeight = 200;
                let labelStr = "a\n  b";
                let labelAlign = Left;

                let record = RenderIMGUITool.prepareFntData(record^);

                RenderIMGUITool.testPositionBufferData(
                  sandbox,
                  record,
                  (. _, apiJsObj, record) => {
                    let apiJsObj = Obj.magic(apiJsObj);
                    let label = apiJsObj##label;
                    let record =
                      label(.
                        (labelX, labelY, labelWidth, labelHeight),
                        labelStr,
                        labelAlign,
                        record,
                      );

                    record;
                  },
                  [|
                    12.,
                    59.,
                    12.,
                    102.,
                    59.,
                    59.,
                    59.,
                    102.,
                    14.,
                    137.,
                    14.,
                    192.,
                    59.,
                    137.,
                    59.,
                    192.,
                  |],
                );
              });
            });

            describe("test text has tab char", () =>
              test(
                "if fntData not has tab char data, then use the default tab data(tab->xAdvance = 4 * space char->xAdvance)",
                () => {
                  let labelX = 10;
                  let labelY = 20;
                  let labelWidth = 300;
                  let labelHeight = 200;
                  let labelStr = "a\tb";
                  let labelAlign = Left;

                  let record = RenderIMGUITool.prepareFntData(record^);

                  RenderIMGUITool.testPositionBufferData(
                    sandbox,
                    record,
                    (. _, apiJsObj, record) => {
                      let apiJsObj = Obj.magic(apiJsObj);
                      let label = apiJsObj##label;
                      let record =
                        label(.
                          (labelX, labelY, labelWidth, labelHeight),
                          labelStr,
                          labelAlign,
                          record,
                        );

                      record;
                    },
                    [|
                      12.,
                      104.,
                      12.,
                      147.,
                      59.,
                      104.,
                      59.,
                      147.,
                      53.,
                      75.,
                      53.,
                      75.,
                      53.,
                      75.,
                      53.,
                      75.,
                      141.,
                      92.,
                      141.,
                      147.,
                      186.,
                      92.,
                      186.,
                      147.,
                    |],
                  );
                },
              )
            );

            describe("test x alignment", () => {
              test("test center", () => {
                let labelX = 10;
                let labelY = 20;
                let labelWidth = 300;
                let labelHeight = 200;
                let labelStr = "ab";
                let labelAlign = Center;

                let record = RenderIMGUITool.prepareFntData(record^);

                RenderIMGUITool.testPositionBufferData(
                  sandbox,
                  record,
                  (. _, apiJsObj, record) => {
                    let apiJsObj = Obj.magic(apiJsObj);
                    let label = apiJsObj##label;
                    let record =
                      label(.
                        (labelX, labelY, labelWidth, labelHeight),
                        labelStr,
                        labelAlign,
                        record,
                      );

                    record;
                  },
                  [|
                    116.,
                    104.,
                    116.,
                    147.,
                    163.,
                    104.,
                    163.,
                    147.,
                    161.,
                    92.,
                    161.,
                    147.,
                    206.,
                    92.,
                    206.,
                    147.,
                  |],
                );
              });
              test("test right", () => {
                let labelX = 10;
                let labelY = 20;
                let labelWidth = 300;
                let labelHeight = 200;
                let labelStr = "ab";
                let labelAlign = Right;

                let record = RenderIMGUITool.prepareFntData(record^);

                RenderIMGUITool.testPositionBufferData(
                  sandbox,
                  record,
                  (. _, apiJsObj, record) => {
                    let apiJsObj = Obj.magic(apiJsObj);
                    let label = apiJsObj##label;
                    let record =
                      label(.
                        (labelX, labelY, labelWidth, labelHeight),
                        labelStr,
                        labelAlign,
                        record,
                      );

                    record;
                  },
                  [|
                    220.,
                    104.,
                    220.,
                    147.,
                    267.,
                    104.,
                    267.,
                    147.,
                    265.,
                    92.,
                    265.,
                    147.,
                    310.,
                    92.,
                    310.,
                    147.,
                  |],
                );
              });
              /*
                not pass the test!!!

                describe
                ("ignore space char in the end of line",
                (
                () => {
               test(
               "test center x alignment",
               (
               () => {
                   let labelX = 10;
                  let labelY = 20;
                  let labelWidth = 300;
                  let labelHeight = 200;
                  let labelStr = "ab  ";
                  let labelAlign = Center;

                  let record = RenderIMGUITool. prepareFntData(record^);

                  RenderIMGUITool.testPositionBufferData(sandbox, record,
                    (record) => {
                      let record =
                        record
                        |> FixedLayoutControlIMGUIAPI.label(
                             (labelX, labelY, labelWidth, labelHeight),
                             labelStr,
                             labelAlign,
                           );

                      record;
                    },
                    [|
                    |],
                  );

               })
               );
                })
                ); */
            });
          });
        })
      )
    );
  });