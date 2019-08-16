open IMGUIType;

let getSettedAssetData = record => record.assetData.settedAssetData;

let createSettedAssetData = () => {
  fntContent: None,
  bitmapArrayBuffer: None,
  customImageDataArr: WonderCommonlib.ArrayService.createEmpty(),
};