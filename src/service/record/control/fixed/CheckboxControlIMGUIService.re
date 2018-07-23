open IMGUIType;

let checkbox = (rect, str, record) => {
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

  let (x, y, width, height) as rect =
    rect |> StructureService.convertIntRectToFloatRect;

  let innerSize = height *. checkboxInnerSizeRatio;
  let outerSize = height *. checkboxOuterSizeRatio;

  let outerBoxRect = (x, y, outerSize, outerSize);

  let (isSelected, innerColor, outerColor) =
    HitService.isInBox(
      outerBoxRect,
      pointPosition |> StructureService.convertIntPositionToFloatPosition,
    ) ?
      IOIMGUIService.isClick(record) ?
        (true, checkboxInnerColorHover, checkboxOuterColorHover) :
        (false, checkboxInnerColorHover, checkboxOuterColorHover) :
      (false, checkboxInnerColor, checkboxOuterColor);

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

  (record, isSelected);
};