open IMGUIType;

open DrawDataType;

let draw = ((x, y, width, height), color, record) => {
  let {currentFontTextureDrawDataBaseIndex} as webglData =
    RecordIMGUIService.unsafeGetWebglData(record);
  let {fontTexUvForWhite} = RecordIMGUIService.getSetting(record);

  let {verticeArr, colorArr, texCoordArr, indexArr} =
    RecordIMGUIService.getFontTextureDrawData(record);
  /* let baseIndex = currentFontTextureDrawDataBaseIndex; */
  let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

  {
    ...record,
    drawData: {
      ...record.drawData,
      fontTextureDrawData: {
        ...record.drawData.fontTextureDrawData,
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
               color
               |> DrawDataArrayService.addPoints(color)
               |> DrawDataArrayService.addPoints(color)
               |> DrawDataArrayService.addPoints(color),
             ),
        texCoordArr:
          texCoordArr
          |> DrawDataArrayService.addPoints(
               fontTexUvForWhite
               |> DrawDataArrayService.addPoints(fontTexUvForWhite)
               |> DrawDataArrayService.addPoints(fontTexUvForWhite)
               |> DrawDataArrayService.addPoints(fontTexUvForWhite),
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