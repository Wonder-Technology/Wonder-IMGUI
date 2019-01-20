open IMGUIType;

let button = (rect, str, record) => {
  let {buttonColor, hoverButtonColor, clickButtonColor} =
    RecordIMGUIService.getSetting(record).buttonSetting;

  let {pointUp, pointDown, pointPosition} =
    RecordIMGUIService.getIOData(record);

  let (isButtonClick, color) =
    HitService.isInBox(
      rect,
      StructureService.convertIntPositionToFloatPosition(pointPosition),
    ) ?
      IOIMGUIService.isClick(record) ?
        (true, clickButtonColor) :
        pointDown ? (false, clickButtonColor) : (false, hoverButtonColor) :
      (false, buttonColor);

  (
    record
    |> DrawBoxIMGUIService.draw(rect, color)
    |> DrawTextIMGUIService.draw(rect, str, FontType.Center),
    isButtonClick,
  );
};