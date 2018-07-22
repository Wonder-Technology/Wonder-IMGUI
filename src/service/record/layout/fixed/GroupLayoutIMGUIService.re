open IMGUIType;

let _getGroupData = record => record.layoutData.groupData;

let beginGroup =
  (. position, record) => {
    let groupData = _getGroupData(record);

    Array.unsafe_set(groupData.positionArr, groupData.index, position);

    {
      ...record,
      layoutData: {
        ...record.layoutData,
        groupData: {
          index: groupData.index + 1,
          positionArr: groupData.positionArr,
        },
      },
    };
  };

let endGroup =
  (. record) => {
    let groupData = _getGroupData(record);

    {
      ...record,
      layoutData: {
        ...record.layoutData,
        groupData: {
          ...groupData,
          index: groupData.index - 1,
        },
      },
    };
  };

let _getGroupPosition = record => {
  let groupData = _getGroupData(record);

  groupData.index <= 0 ?
    None :
    {
      let (_, position) =
        ArrayService.range(0, groupData.index - 1)
        |> WonderCommonlib.ArrayService.reduceOneParam(
             (. (positionArr, (x, y)), index) => {
               let (posX, posY) = Array.unsafe_get(positionArr, index);

               (positionArr, (x + posX, y + posY));
             },
             (groupData.positionArr, (0, 0)),
           );

      position |. Some;
    };
};

let computeRectBasedOnTopLeftOfView = (rect, record) =>
  switch (_getGroupPosition(record)) {
  | None => rect
  | Some((groupX, groupY)) =>
    let (x, y, width, height) = rect;

    (x + groupX, y + groupY, width, height);
  };