type customStyleData;

type singleCustomStyleData =
  WonderCommonlib.ImmutableHashMapService.t(customStyleData);

type allCustomStyleData =
  WonderCommonlib.ImmutableHashMapService.t(singleCustomStyleData);

type color = array(float);

type imageId = string;

type buttonSkinData = {
  buttonColor: color,
  hoverButtonColor: color,
  clickButtonColor: color,
  buttonImage: Js.Nullable.t(imageId),
  hoverButtonImage: Js.Nullable.t(imageId),
  clickButtonImage: Js.Nullable.t(imageId),
};

type singleSkinData = {
  buttonSkinData,
  allCustomStyleData,
};

type allSkinDataMap =
  WonderCommonlib.ImmutableHashMapService.t(singleSkinData);

type skinData = {allSkinDataMap};