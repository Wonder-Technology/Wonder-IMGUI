let addSkinData = (skinName, skinData, map) =>
  map |> WonderCommonlib.ImmutableHashMapService.set(skinName, skinData);

let removeSkinData = (skinName, map) =>
  map |> WonderCommonlib.ImmutableHashMapService.deleteVal(skinName);

let unsafeGetSkinData = (skinName, map) =>
  map |> WonderCommonlib.ImmutableHashMapService.unsafeGet(skinName);

let setSkinData = (skinName, skinData, map) =>
  map |> WonderCommonlib.ImmutableHashMapService.set(skinName, skinData);

let mergeAllSkinDataMaps = (sourceMap, targetMap) =>
  sourceMap
  |> WonderCommonlib.ImmutableHashMapService.getValidEntries
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. targetMap, (skinName, singleSkinData)) =>
         targetMap
         |> WonderCommonlib.ImmutableHashMapService.set(
              skinName,
              singleSkinData,
            ),
       targetMap,
     );