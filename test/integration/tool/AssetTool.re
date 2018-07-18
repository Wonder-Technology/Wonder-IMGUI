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

let _buildCustomImageData = (source, id, imageType) => (
  source,
  id,
  imageType,
);

let buildCustomImageDataArr = () => [|
  _buildCustomImageData(Obj.magic(200), "a1", ImageType.Jpg),
  _buildCustomImageData(Obj.magic(201), "a2", ImageType.Png),
  _buildCustomImageData(Obj.magic(202), "a3", ImageType.Png),
|];