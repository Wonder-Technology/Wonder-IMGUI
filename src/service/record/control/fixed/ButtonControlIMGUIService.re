open IMGUIType;

let _isLastColorNotEqualCurrentColor = (currentColor, record) => {
  let {lastColor} = RecordIMGUIService.getButtonData(record);

  switch (lastColor) {
  | Some(lastColor) when lastColor == currentColor => false
  | _ => true
  };
};

let _setLastColor = (currentColor, record) => {
  ...record,
  buttonData: {
    lastColor: Some(currentColor),
  },
};

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

  let record =
    _isLastColorNotEqualCurrentColor(color, record) ?
      record
      |> BufferDataIMGUIService.markNeedUpdateBufferData
      |> _setLastColor(color) :
      record;

  (
    record
    |> DrawBoxIMGUIService.draw(rect, color)
    |> DrawTextIMGUIService.draw(rect, str, FontType.Center),
    isButtonClick,
  );
};