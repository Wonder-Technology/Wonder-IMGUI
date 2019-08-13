open IMGUIType;

let createAPIJsObj = () => {
  "getIOData": (. record) => RecordIMGUIService.getIOData(record),
  "getPointUp": (. ioData) => ioData.pointUp,
  "getPointDown": (. ioData) => ioData.pointDown,
  "getPointPosition": (. ioData) => ioData.pointPosition,
  "isClick": (. record) => IOIMGUIService.isClick(record),
  "drawBox":
    (. rect, color, record) => DrawBoxIMGUIService.draw(rect, color, record),
  "drawText":
    (. rect, str, align, record) =>
      DrawTextIMGUIService.draw(rect, str, align, record),
  "parseShowData": ExtendAPIJsObjSkinIMGUIService.parseShowData,
  "unsafeGetSkinData":
    (. skinName, record) =>
      ManageSkinIMGUIService.unsafeGetSkinData(skinName, record),
  "unsafeGetSingleCustomStyleDataMap":
    (. singleCustomStyleName, skinData) =>
      ManageSkinIMGUIService.unsafeGetSingleCustomStyleDataMap(
        singleCustomStyleName,
        skinData,
      ),
  "unsafeGetCustomStyleData":
    (. customStyleName, singleCustomStyleDataMap) =>
      ManageSkinIMGUIService.unsafeGetCustomStyleData(
        customStyleName,
        singleCustomStyleDataMap,
      ),
};