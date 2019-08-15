open IMGUIType;

open SkinType;

let getAllSkinDataMap = record => record.extendData.skinData.allSkinDataMap;

let _setAllSkinDataMap = (allSkinDataMap, record) => {
  ...record,
  extendData: {
    ...record.extendData,
    skinData: {
      allSkinDataMap: allSkinDataMap,
    },
  },
};

let clearAllSkins = record =>
  _setAllSkinDataMap(
    WonderCommonlib.ImmutableHashMapService.createEmpty(),
    record,
  );

let addSkinData = (skinName, skinData, record) =>
  _setAllSkinDataMap(
    getAllSkinDataMap(record)
    |> AllSkinDataMapService.addSkinData(skinName, skinData),
    record,
  );

let removeSkinData = (skinName, record) =>
  _setAllSkinDataMap(
    getAllSkinDataMap(record)
    |> AllSkinDataMapService.removeSkinData(skinName),
    record,
  );

let unsafeGetSkinData = (skinName, record) =>
  getAllSkinDataMap(record)
  |> AllSkinDataMapService.unsafeGetSkinData(skinName);

let setSkinData = (skinName, skinData, record) =>
  (
    getAllSkinDataMap(record)
    |> AllSkinDataMapService.setSkinData(skinName, skinData)
  )
  ->(_setAllSkinDataMap(record));

let createSkinData = (buttonSkinData, allCustomStyleData) => {
  buttonSkinData,
  allCustomStyleData,
};

let createAllCustomStyleData = () =>
  WonderCommonlib.ImmutableHashMapService.createEmpty();

let createSingleCustomStyleData = () =>
  WonderCommonlib.ImmutableHashMapService.createEmpty();

let addCustomStyleData = (name, data: customStyleData, singleCustomStyleData) =>
  singleCustomStyleData
  |> WonderCommonlib.ImmutableHashMapService.set(name, data);

let removeCustomStyleData =
    (name, singleCustomStyleData: singleCustomStyleData) =>
  singleCustomStyleData
  |> WonderCommonlib.ImmutableHashMapService.deleteVal(name);

let addSingleCustomStyleData =
    (name, singleCustomStyle: singleCustomStyleData, allCustomStyleData) =>
  allCustomStyleData
  |> WonderCommonlib.ImmutableHashMapService.set(name, singleCustomStyle);

let removeSingleCustomStyleData =
    (name, allCustomStyleData: allCustomStyleData) =>
  allCustomStyleData
  |> WonderCommonlib.ImmutableHashMapService.deleteVal(name);

let _getAllCustomStyleData = skinData => skinData.allCustomStyleData;

let unsafeGetSingleCustomStyleDataMap = (singleCustomStyleName, skinData) =>
  _getAllCustomStyleData(skinData)
  |> WonderCommonlib.ImmutableHashMapService.unsafeGet(singleCustomStyleName);

let unsafeGetCustomStyleData = (customStyleName, singleCustomStyleDataMap) =>
  singleCustomStyleDataMap
  |> WonderCommonlib.ImmutableHashMapService.unsafeGet(customStyleName);

let hasCustomStyleData = (customStyleName, singleCustomStyleDataMap) =>
  singleCustomStyleDataMap
  |> WonderCommonlib.ImmutableHashMapService.has(customStyleName);

let mergeAllSkinDataMaps = (sourceAllSkinDataMap, record) =>
  _setAllSkinDataMap(
    AllSkinDataMapService.mergeAllSkinDataMaps(
      sourceAllSkinDataMap,
      getAllSkinDataMap(record),
    ),
    record,
  );

let hasSingleCustomStyleName = singleCustomStyleName =>
  !(singleCustomStyleName |> Js.Nullable.isNullable);

let parseSingleCustomStyleName = singleCustomStyleName =>
  singleCustomStyleName |> Js.Nullable.toOption |> OptionService.unsafeGet;