open IMGUIType;

let getAllSkinDataMap = record => record.extendData.skinData.allSkinDataMap;

let clearAllSkins = record => {
  ...record,
  extendData: {
    ...record.extendData,
    skinData: {
      allSkinDataMap: WonderCommonlib.ImmutableHashMapService.createEmpty(),
    },
  },
};

let addSkinData = (skinName, skinData, record) => {
  ...record,
  extendData: {
    ...record.extendData,
    skinData: {
      allSkinDataMap:
        getAllSkinDataMap(record)
        |> WonderCommonlib.ImmutableHashMapService.set(skinName, skinData),
    },
  },
};

let removeSkinData = (skinName, record) => {
  ...record,
  extendData: {
    ...record.extendData,
    skinData: {
      allSkinDataMap:
        getAllSkinDataMap(record)
        |> WonderCommonlib.ImmutableHashMapService.deleteVal(skinName),
    },
  },
};

let unsafeGetSkinData = (skinName, record) =>
  getAllSkinDataMap(record)
  |> WonderCommonlib.ImmutableHashMapService.unsafeGet(skinName);

let createSkinData = allCustomStyleData => {
  allCustomStyleData: allCustomStyleData,
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