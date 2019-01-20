open IMGUIType;

let _getIsSelectedMap = record =>
  RecordIMGUIService.getRadioButtonData(record).isSelectedMap;

let _getSelectedRadioButtonIndex = dataArr =>
  switch (
    dataArr
    |> Js.Array.map(((_, (isSelected, innerColor, outerColor))) =>
         isSelected
       )
    |> Js.Array.indexOf(true)
  ) {
  | (-1) => None
  | index => Some(index)
  };

let _getSelectIndex = (defaultSelectIndex, group, record) =>
  switch (
    _getIsSelectedMap(record) |> WonderCommonlib.HashMapService.get(group)
  ) {
  | None => defaultSelectIndex
  | Some(index) => index
  };

let _buildDrawData = (defaultSelectIndex, groupDataArr, group: string, record) => {
  let {radioButtonInnerRadius, radioButtonOuterRadius} =
    RecordIMGUIService.getSetting(record).radioButtonSetting;
  let {pointPosition} = RecordIMGUIService.getIOData(record);

  let selectIndex = _getSelectIndex(defaultSelectIndex, group, record);

  let (dataArr, ioSelectIndex, ioInHoverIndex) =
    groupDataArr
    |> WonderCommonlib.ArrayService.reduceOneParami(
         (.
           (dataArr, resultSelectIndex, resultInHoverIndex),
           (rect, str),
           index,
         ) => {
           let (x, y, width, height) =
             GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(
               rect,
               record,
             )
             |> StructureService.convertIntRectToFloatRect;

           let innerRadius = height /. 2. *. radioButtonInnerRadius;
           let outerRadius = height /. 2. *. radioButtonOuterRadius;

           let innerCircleWidth = innerRadius *. 2.;
           let innerCircleHeight = innerRadius *. 2.;
           let outerCircleWidth = outerRadius *. 2.;
           let outerCircleHeight = outerRadius *. 2.;

           let innerCircleRect = (x, y, innerCircleWidth, innerCircleHeight);
           let outerCircleRect = (x, y, outerCircleWidth, outerCircleHeight);

           let isInCircle =
             HitService.isInCircle(
               outerCircleRect,
               pointPosition
               |> StructureService.convertIntPositionToFloatPosition,
             );

           let isIOSelected = isInCircle && IOIMGUIService.isClick(record);

           (
             dataArr
             |> ArrayService.push((
                  x,
                  y,
                  width,
                  height,
                  str,
                  innerRadius,
                  outerRadius,
                  innerCircleWidth,
                  innerCircleHeight,
                  outerCircleWidth,
                  outerCircleHeight,
                  innerCircleRect,
                  outerCircleRect,
                )),
             switch (resultSelectIndex) {
             | None => isIOSelected ? Some(index) : None
             | _ => resultSelectIndex
             },
             switch (resultInHoverIndex) {
             | None => isInCircle ? Some(index) : None
             | _ => resultInHoverIndex
             },
           );
         },
         ([||], None, None),
       );

  (
    dataArr,
    switch (ioSelectIndex) {
    | None => selectIndex
    | Some(index) => index
    },
    ioInHoverIndex,
  );
};

let radioButton =
    (groupDataArr, defaultSelectIndex: int, group: string, record) => {
  WonderLog.Contract.requireCheck(
    () =>
      WonderLog.(
        Contract.(
          Operators.(
            test(
              Log.buildAssertMessage(
                ~expect={j|defaultSelectIndex < radioButton count|j},
                ~actual={j|not|j},
              ),
              () =>
              defaultSelectIndex < (groupDataArr |> Js.Array.length)
            )
          )
        )
      ),
    true,
  );

  let {
    radioButtonCircleSegments,
    radioButtonOuterColor,
    radioButtonInnerColor,
    radioButtonOuterColorHover,
    radioButtonInnerColorHover,
  } =
    RecordIMGUIService.getSetting(record).radioButtonSetting;

  let (dataArr, selectIndex, ioInHoverIndex) =
    _buildDrawData(defaultSelectIndex, groupDataArr, group, record);

  _getIsSelectedMap(record)
  |> WonderCommonlib.HashMapService.set(group, selectIndex)
  |> ignore;

  let record =
    dataArr
    |> WonderCommonlib.ArrayService.reduceOneParami(
         (.
           record,
           (
             x,
             y,
             width,
             height,
             str,
             innerRadius,
             outerRadius,
             innerCircleWidth,
             innerCircleHeight,
             outerCircleWidth,
             outerCircleHeight,
             innerCircleRect,
             outerCircleRect,
           ),
           index,
         ) => {
           let isSelected = selectIndex === index;
           let (innerColor, outerColor) =
             isSelected ?
               (radioButtonInnerColorHover, radioButtonOuterColorHover) :
               (
                 switch (ioInHoverIndex) {
                 | None => (radioButtonInnerColor, radioButtonOuterColor)
                 | Some(ioInHoverIndex) =>
                   ioInHoverIndex === index ?
                     (
                       radioButtonInnerColorHover,
                       radioButtonOuterColorHover,
                     ) :
                     (radioButtonInnerColor, radioButtonOuterColor)
                 }
               );

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

           record
           |> DrawTextIMGUIService.draw(
                (x +. outerCircleWidth, y, width -. outerCircleWidth, height),
                str,
                FontType.Center,
              );
         },
         record,
       );

  (record, selectIndex);
};