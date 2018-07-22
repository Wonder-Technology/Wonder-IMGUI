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

let _buildDrawData = (groupDataArr, group: string, record) => {
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

  let dataArr =
    groupDataArr
    |> Js.Array.map(((rect, str)) => {
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

         let (isSelected, innerColor, outerColor) =
           HitService.isInCircle(
             outerCircleRect,
             pointPosition
             |> StructureService.convertIntPositionToFloatPosition,
           ) ?
             IOIMGUIService.isClick(record) ?
               (
                 true,
                 radioButtonInnerColorHover,
                 radioButtonOuterColorHover,
               ) :
               (false, radioButtonInnerColorHover, radioButtonOuterColorHover) :
             (false, radioButtonInnerColor, radioButtonOuterColor);

         (
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
           (isSelected, innerColor, outerColor),
         );
       });

  switch (_getSelectedRadioButtonIndex(dataArr)) {
  | Some(index) =>
    _getIsSelectedMap(record)
    |> WonderCommonlib.HashMapService.set(group, index)
    |> ignore;

    (dataArr, Some(index));

  | None =>
    switch (
      _getIsSelectedMap(record) |> WonderCommonlib.HashMapService.get(group)
    ) {
    | None => (dataArr, None)
    | Some(index) =>
      let (data, (isSelected, innerColor, outerColor)) =
        Array.unsafe_get(dataArr, index);

      Array.unsafe_set(
        dataArr,
        index,
        (data, (true, innerColor, outerColor)),
      );

      (dataArr, Some(index));
    }
  };
};

/* let radioButton = ((x, y, width, height), str, group: string, record) => { */
let radioButton = (groupDataArr, group: string, record) => {
  let {radioButtonCircleSegments} =
    RecordIMGUIService.getSetting(record).radioButtonSetting;

  let (dataArr, selectIndexOption) =
    _buildDrawData(groupDataArr, group, record);

  let record =
    dataArr
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (.
           record,
           (
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
             (isSelected, innerColor, outerColor),
           ),
         ) => {
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

  (record, selectIndexOption);
};