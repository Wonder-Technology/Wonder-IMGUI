type customStyleData;

type singleCustomStyleData =
  WonderCommonlib.ImmutableHashMapService.t(customStyleData);

type allCustomStyleData =
  WonderCommonlib.ImmutableHashMapService.t(singleCustomStyleData);

type color = array(float);

type buttonSkinData = {
  buttonColor: color,
  hoverButtonColor: color,
  clickButtonColor: color,
  buttonImage: Js.Nullable.t(ExtendType.customImageId),
  hoverButtonImage: Js.Nullable.t(ExtendType.customImageId),
  clickButtonImage: Js.Nullable.t(ExtendType.customImageId),
  fontAlign: FontType.align,
  fontColor: color,
};

type singleSkinData = {
  buttonSkinData,
  allCustomStyleData,
};

type allSkinDataMap =
  WonderCommonlib.ImmutableHashMapService.t(singleSkinData);

type skinData = {allSkinDataMap};