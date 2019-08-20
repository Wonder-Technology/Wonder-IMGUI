open Wonder_jest;

let _ =
  describe("ManageSkinIMGUIService", () => {
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

    describe("mergeAllSkinDataMaps", () =>
      test("merge two allSkinDataMaps", () => {
        let record = DataSkinIMGUIService.addDefaultSkinData(record^);

        let record =
          ManageSkinIMGUIService.mergeAllSkinDataMaps(
            WonderCommonlib.ImmutableHashMapService.createEmpty()
            |> WonderCommonlib.ImmutableHashMapService.set(
                 "a2",
                 Obj.magic(1),
               )
            |> WonderCommonlib.ImmutableHashMapService.set(
                 "a3",
                 Obj.magic(2),
               )
            |> WonderCommonlib.ImmutableHashMapService.deleteVal("a2"),
            record,
          );

        ManageSkinIMGUIService.getAllSkinDataMap(record)
        |> ImmutableHashMapService.getKeys
        |> expect == [|DataSkinIMGUIService.getDefaultSkinName(), "a3"|];
      })
    );
  });