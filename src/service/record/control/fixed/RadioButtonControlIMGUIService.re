open IMGUIType;

let _isLastColorNotEqualCurrentColor =
    (currentInnerColor, currentOuterColor, record) => {
  let {lastInnerColor, lastOuterColor} =
    RecordIMGUIService.getRadioButtonData(record);

  switch (lastInnerColor, lastOuterColor) {
  | (Some(lastInnerColor), Some(lastOuterColor))
      when
        lastInnerColor == currentInnerColor
        && lastOuterColor == currentOuterColor =>
    false
  | _ => true
  };
};

let _setLastColor = (currentInnerColor, currentOuterColor, record) => {
  ...record,
  radioButtonData: {
    lastInnerColor: Some(currentInnerColor),
    lastOuterColor: Some(currentOuterColor),
  },
};

let radioButton = ((x, y, width, height), str, record) => {
  let {
    radioButtonOuterColor,
    radioButtonInnerColor,
    radioButtonOuterColorHover,
    radioButtonInnerColorHover,
    radioButtonCircleSegments,
    radioButtonInnerRadius,
    radioButtonOuterRadius,
  } =
    RecordIMGUIService.getSetting(record).radioButtonSetting;

  let {pointPosition} = RecordIMGUIService.getIOData(record);

  let innerRadius = height /. 2. *. radioButtonInnerRadius;
  let outerRadius = height /. 2. *. radioButtonOuterRadius;

  let innerCircleWidth = innerRadius *. 2.;
  let innerCircleHeight = innerRadius *. 2.;
  let outerCircleWidth = outerRadius *. 2.;
  let outerCircleHeight = outerRadius *. 2.;

  let innerCircleRect = (x, y, innerCircleWidth, innerCircleHeight);
  let outerCircleRect = (x, y, outerCircleWidth, outerCircleHeight);

  let (isSelected, innerColor, outerColor) =
    HitService.isInCircle(
      outerCircleRect,
      pointPosition |> StructureService.convertIntPositionToFloatPosition,
    ) ?
      IOIMGUIService.isClick(record) ?
        (true, radioButtonInnerColorHover, radioButtonOuterColorHover) :
        (false, radioButtonInnerColorHover, radioButtonOuterColorHover) :
      (false, radioButtonInnerColor, radioButtonOuterColor);

  let record =
    _isLastColorNotEqualCurrentColor(innerColor, outerColor, record) ?
      record
      |> BufferDataIMGUIService.markNeedUpdateBufferData
      |> _setLastColor(innerColor, outerColor) :
      record;

  let record =
    record
    |> DrawCircleIMGUIService.draw(
         outerCircleRect,
         outerColor,
         radioButtonCircleSegments,
       );

  let record =
    isSelected ?
      record
      |> DrawCircleIMGUIService.draw(
           (
             0.5
             *. (x +. (x +. outerCircleWidth) -. innerRadius *. 2.)
             |> Js.Math.round,
             0.5
             *. (y +. (y +. outerCircleHeight) -. innerRadius *. 2.)
             |> Js.Math.round,
             innerCircleWidth,
             innerCircleHeight,
           ),
           innerColor,
           radioButtonCircleSegments,
         ) :
      record;

  (
    record
    |> DrawTextIMGUIService.draw(
         (x +. outerCircleWidth, y, width -. outerCircleWidth, height),
         str,
         FontType.Center,
       ),
    isSelected,
  );
};