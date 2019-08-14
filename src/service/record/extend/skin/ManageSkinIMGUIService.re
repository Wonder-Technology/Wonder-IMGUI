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
    |> WonderCommonlib.ImmutableHashMapService.set(skinName, skinData),
    record,
  );

let removeSkinData = (skinName, record) =>
  _setAllSkinDataMap(
    getAllSkinDataMap(record)
    |> WonderCommonlib.ImmutableHashMapService.deleteVal(skinName),
    record,
  );

let unsafeGetSkinData = (skinName, record) =>
  getAllSkinDataMap(record)
  |> WonderCommonlib.ImmutableHashMapService.unsafeGet(skinName);

let setSkinData = (skinName, skinData, record) =>
  (
    getAllSkinDataMap(record)
    |> WonderCommonlib.ImmutableHashMapService.set(skinName, skinData)
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