let unsafeGetFntData = AssetIMGUIService.unsafeGetFntData;

let unsafeGetBitmap = AssetIMGUIService.unsafeGetBitmap;

let prepareFontAsset = record => {
  let record = IOIMGUIAPI.addFont("", "", record);

  let bitmap = Obj.magic(101);

  {
    ...record,
    assetData: {
      ...record.assetData,
      bitmapMap:
        record.assetData.bitmapMap
        |> WonderCommonlib.HashMapService.set(
             record.assetData.bitmapId,
             bitmap,
           ),
    },
  };
};