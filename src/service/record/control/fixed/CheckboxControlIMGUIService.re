open IMGUIType;

let _getIndex = record => record.checkboxData.index;

let _addIndex = record => {
  ...record,
  checkboxData: {
    ...record.checkboxData,
    index: record.checkboxData.index |> succ,
  },
};

let _getIsSlected = (index, record) =>
  switch (
    record.checkboxData.isSelectedMap
    |> WonderCommonlib.SparseMapService.get(index)
  ) {
  | None => false
  | Some(isSelected) => isSelected
  };

let _setIsSlected = (index, value, record) => {
  record.checkboxData.isSelectedMap
  |> WonderCommonlib.SparseMapService.set(index, value)
  |> ignore;

  record;
};

let checkbox = ((x, y, width, height) as rect, str, record) => {
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

  let (isSelected, innerColor, outerColor) =
    HitService.isInBox(
      outerBoxRect,
      pointPosition |> StructureService.convertIntPositionToFloatPosition,
    ) ?
      IOIMGUIService.isClick(record) ?
        (true, checkboxInnerColorHover, checkboxOuterColorHover) :
        (false, checkboxInnerColorHover, checkboxOuterColorHover) :
      (false, checkboxInnerColor, checkboxOuterColor);

  let isSelected =
    ! isSelected ?
      _getIsSlected(index, record) :
      _getIsSlected(index, record) ? false : true;

  let record = _setIsSlected(index, isSelected, record);

  let record = record |> DrawBoxIMGUIService.draw(outerBoxRect, outerColor);

  let record =
    isSelected ?
      record
      |> DrawBoxIMGUIService.draw(
           (
             0.5 *. (x +. (x +. width) -. innerSize) |> Js.Math.round,
             0.5 *. (y +. (y +. height) -. innerSize) |> Js.Math.round,
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