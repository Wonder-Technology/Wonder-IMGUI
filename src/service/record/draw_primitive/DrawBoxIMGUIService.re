open IMGUIType;

open DrawDataType;

let draw = ((x, y, width, height), color, record) => {
  let {verticeArr, colorArr, indexArr} =
    RecordIMGUIService.getNoTextureDrawData(record);

  let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

  {
    ...record,
    drawData: {
      ...record.drawData,
      noTextureDrawData: {
        verticeArr:
          verticeArr
          |> DrawDataArrayService.addPoints([|
               x,
               y,
               x,
               y +. height,
               x +. width,
               y,
               x +. width,
               y +. height,
             |]),
        colorArr:
          colorArr
          |> DrawDataArrayService.addPoints(
               DrawDataArrayService.concatArrays([|
                 color,
                 color,
                 color,
                 color,
               |]),
             ),
        indexArr:
          indexArr
          |> DrawDataArrayService.addPoints([|
               baseIndex,
               baseIndex + 1,
               baseIndex + 2,
               baseIndex + 3,
               baseIndex + 2,
               baseIndex + 1,
             |]),
      },
    },
  };
};