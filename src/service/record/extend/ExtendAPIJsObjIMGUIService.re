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
    (. rect, str, cssData, record) =>
      DrawTextIMGUIService.draw(rect, str, cssData, record),
  "drawImage":
    (. rect, uv, id, record) =>
      DrawImageIMGUIService.draw(rect, uv, id, record),
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
  "hasSingleCustomStyleName":
    (. singleCustomStyleName) =>
      ManageSkinIMGUIService.hasSingleCustomStyleName(singleCustomStyleName),
  "parseSingleCustomStyleName":
    (. singleCustomStyleName) =>
      ManageSkinIMGUIService.parseSingleCustomStyleName(
        singleCustomStyleName,
      ),
  "hasCustomStyleData":
    (. customStyleName, singleCustomStyleDataMap) =>
      ManageSkinIMGUIService.hasCustomStyleData(
        customStyleName,
        singleCustomStyleDataMap,
      ),
};