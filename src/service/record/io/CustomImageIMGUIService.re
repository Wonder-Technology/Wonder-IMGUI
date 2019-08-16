open IMGUIType;

module SetAsset = {
  let getCustomImageDataArr = record =>
    RecordAssetIMGUIService.getSettedAssetData(record).customImageDataArr;

  let addCustomImageData = (data, record) => {
    ...record,
    assetData: {
      ...record.assetData,
      settedAssetData: {
        ...RecordAssetIMGUIService.getSettedAssetData(record),
        customImageDataArr:
          getCustomImageDataArr(record) |> ArrayService.push(data),
      },
    },
  };
};