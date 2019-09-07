open IMGUIType;

module SetAsset = {
  let getCustomImageDataArr = record =>
    RecordAssetIMGUIService.getSettedAssetData(record).customImageDataArr;

  let hasCustomImageData = (imageId, record) =>
    getCustomImageDataArr(record)
    |> Js.Array.find(((_, id, _)) => id === imageId)
    |> Js.Option.isSome;

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

  let removeCustomImageData = (imageId, record) => {
    ...record,
    assetData: {
      ...record.assetData,
      settedAssetData: {
        ...RecordAssetIMGUIService.getSettedAssetData(record),
        customImageDataArr:
          getCustomImageDataArr(record)
          |> Js.Array.filter(((_, id, _)) => id !== imageId),
      },
    },
  };
};