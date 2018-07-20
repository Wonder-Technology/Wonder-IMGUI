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
    RecordIMGUIService.getSetting(record);

  let {mouseClick, mouseDown, mousePosition, mouseMovementDelta} =
    RecordIMGUIService.getIOData(record);

  let (isButtonClick, color) =
    HitService.inBox(rect, mousePosition) ?
      mouseClick ?
        (true, clickButtonColor) :
        mouseDown ? (false, clickButtonColor) : (false, hoverButtonColor) :
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