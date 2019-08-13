open IMGUIType;

let getDefaultSkinName = () => "Wonder_Default_Skin";

let getDefaultCustomStyleName = () => "Wonder_Default_CustomStyle";

let _createDefaultSkinData = () => {
  let allCustomStyleData = ManageSkinIMGUIService.createAllCustomStyleData();

  let singleCustomStyleData =
    ManageSkinIMGUIService.createSingleCustomStyleData();

  let allCustomStyleData =
    allCustomStyleData
    |> ManageSkinIMGUIService.addSingleCustomStyleData(
         getDefaultCustomStyleName(),
         singleCustomStyleData,
       );

  ManageSkinIMGUIService.createSkinData(allCustomStyleData);
  /* TODO add build-in extend custom controls and its default skin data */
};

let addDefaultSkinData = record => {
  ...record,
  extendData: {
    ...record.extendData,
    skinData: {
      allSkinDataMap:
        ManageSkinIMGUIService.getAllSkinDataMap(record)
        |> WonderCommonlib.ImmutableHashMapService.set(
             getDefaultSkinName(),
             _createDefaultSkinData(),
           ),
    },
  },
};