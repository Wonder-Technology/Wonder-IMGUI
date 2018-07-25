open IMGUIType;

let _getIndex = record => record.controlData.checkboxData.index;

let _addIndex = record => {
  ...record,
  controlData: {
    ...record.controlData,
    checkboxData: {
      ...record.controlData.checkboxData,
      index: record.controlData.checkboxData.index |> succ,
    },
  },
};

let _getIsSelectedByDefaultAndMap = (index, defaultIsSelected, record) =>
  switch (
    record.controlData.checkboxData.isSelectedMap
    |> WonderCommonlib.SparseMapService.get(index)
  ) {
  | None => defaultIsSelected
  | Some(isSelected) => isSelected
  };

let _setIsSlected = (index, value, record) => {
  record.controlData.checkboxData.isSelectedMap
  |> WonderCommonlib.SparseMapService.set(index, value)
  |> ignore;

  record;
};

let checkbox =
    ((x, y, width, height) as rect, defaultIsSelected: bool, str, record) => {
  let {
    checkboxOuterColor,
    checkboxInnerColor,
    checkboxOuterColorHover,
    checkboxInnerColorHover,
    checkboxInnerSizeRatio,
    checkboxOuterSizeRatio,
  } =
    RecordIMGUIService.getSetting(record).checkboxSetting;
  let {pointPosition} = RecordIMGUIService.getIOData(record);

  let innerSize = height *. checkboxInnerSizeRatio;
  let outerSize = height *. checkboxOuterSizeRatio;

  let outerBoxRect = (x, y, outerSize, outerSize);

  let index = _getIndex(record);

  let isSelectedByDefaultAndMap =
    _getIsSelectedByDefaultAndMap(index, defaultIsSelected, record);

  let isInBox =
    HitService.isInBox(
      outerBoxRect,
      pointPosition |> StructureService.convertIntPositionToFloatPosition,
    );

  let isSelected =
    isInBox && IOIMGUIService.isClick(record) ?
      isSelectedByDefaultAndMap ? false : true : isSelectedByDefaultAndMap;

  let (innerColor, outerColor) =
    isSelected ?
      (checkboxInnerColorHover, checkboxOuterColorHover) :
      isInBox ?
        (checkboxInnerColorHover, checkboxOuterColorHover) :
        (checkboxInnerColor, checkboxOuterColor);

  let record = _setIsSlected(index, isSelected, record);

  let record = record |> DrawBoxIMGUIService.draw(outerBoxRect, outerColor);

  let record =
    isSelected ?
      record
      |> DrawBoxIMGUIService.draw(
           (
             0.5 *. (x +. (x +. outerSize) -. innerSize) |> Js.Math.round,
             0.5 *. (y +. (y +. outerSize) -. innerSize) |> Js.Math.round,
             innerSize,
             innerSize,
           ),
           innerColor,
         ) :
      record;

  let record =
    record
    |> DrawTextIMGUIService.draw(
         (x +. outerSize, y, width -. outerSize, height),
         str,
         FontType.Center,
       );

  let record = _addIndex(record);

  (record, isSelected);
};