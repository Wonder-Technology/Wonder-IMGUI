open IMGUIType;

let _addIndex = record => {
  ...record,
  controlData: {
    ...record.controlData,
    sliderData: {
      ...record.controlData.sliderData,
      index: record.controlData.sliderData.index |> succ,
    },
  },
};

let _getIndex = record => record.controlData.sliderData.index;

let _getValue = (index, defaultValue, record) =>
  switch (
    record.controlData.sliderData.valueMap
    |> WonderCommonlib.SparseMapService.get(index)
  ) {
  | None => defaultValue
  | Some(value) => value
  };

let _setValue = (index, value, record) => {
  record.controlData.sliderData.valueMap
  |> WonderCommonlib.SparseMapService.set(index, value)
  |> ignore;

  record;
};

let _computeValue =
    (
      (isSelected, pointPosition, index, defaultValue),
      (x, y, width, height),
      (min, max, doRounding, numDecimalDigits),
      record,
    ) =>
  isSelected ?
    {
      let xMax = x +. width;
      let xMin = x;

      let (pointPositionX, pointPositionY) = pointPosition;

      /*
       values larger than xMin and xMax should not overflow or underflow the slider.
       */
      let mouseX =
        NumberService.clamp(
          pointPositionX |> NumberType.intToFloat,
          xMin,
          xMax,
        );

      let value = (max -. min) *. ((mouseX -. xMin) /. (xMax -. xMin)) +. min;

      doRounding ? Js.Math.round(value) : value;
    } :
    _getValue(index, defaultValue, record);

let _slider =
    (
      ((x, y, width, height) as rect, textWidth),
      (min, max, doRounding, numDecimalDigits),
      (defaultValue: float, str),
      record,
    ) => {
  let {
    sliderBackgroundColor,
    sliderFillColor,
    sliderBackgroundColorHover,
    sliderFillColorHover,
  } =
    RecordIMGUIService.getSetting(record).sliderSetting;
  let {pointDown, pointPosition} = RecordIMGUIService.getIOData(record);

  let (isSelected, backgroudColor, fillColor) =
    HitService.isInBox(
      rect,
      pointPosition |> StructureService.convertIntPositionToFloatPosition,
    ) ?
      pointDown ?
        (true, sliderBackgroundColorHover, sliderFillColorHover) :
        (false, sliderBackgroundColorHover, sliderFillColorHover) :
      (false, sliderBackgroundColor, sliderFillColor);

  let index = _getIndex(record);

  let value =
    _computeValue(
      (isSelected, pointPosition, index, defaultValue),
      rect,
      (min, max, doRounding, numDecimalDigits),
      record,
    );

  let sliderFill = (value -. min) /. (max -. min);

  let sliderValueStr =
    Js.Float.toFixedWithPrecision(
      value,
      ~digits=doRounding ? 0 : numDecimalDigits,
    );

  let record = record |> DrawBoxIMGUIService.draw(rect, backgroudColor);

  let record =
    record
    |> DrawBoxIMGUIService.draw(
         (x, y, width *. sliderFill, height),
         fillColor,
       );

  let record =
    record |> DrawTextIMGUIService.draw(rect, sliderValueStr, FontType.Center);

  let record =
    record
    |> DrawTextIMGUIService.draw(
         (x +. width, y, textWidth, height),
         str,
         FontType.Left,
       );

  let record = record |> _setValue(index, value) |> _addIndex;

  (record, isSelected, value);
};

let sliderInt =
    ((rect, textWidth), (min, max), (defaultValue, str), record) => {
  let (record, isSelected, value) =
    _slider(
      (rect, textWidth),
      (min |> NumberType.intToFloat, max |> NumberType.intToFloat, true, 0),
      (defaultValue |> NumberType.intToFloat, str),
      record,
    );

  (record, isSelected, value |> NumberType.floatToInt);
};

let sliderFloat =
    (
      (rect, textWidth),
      (min, max, numDecimalDigits),
      (defaultValue, str),
      record,
    ) =>
  _slider(
    (rect, textWidth),
    (min, max, false, numDecimalDigits),
    (defaultValue, str),
    record,
  );