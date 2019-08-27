open IMGUIType;

let getSettedAssetData = record => record.assetData.settedAssetData;

let createSettedAssetData = () => {
  fntName: None,
  fntContent: None,
  bitmapName: None,
  bitmapArrayBuffer: None,
  customImageDataArr: WonderCommonlib.ArrayService.createEmpty(),
};