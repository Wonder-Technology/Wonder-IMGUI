open IMGUIType;

open DrawDataArrType;

let draw = ((x, y, width, height), color, {drawDataArr} as record) => {
  let {currentFontTextureDrawDataBaseIndex} as webglData =
    RecordIMGUIService.unsafeGetWebglData(record);
  let {fontTexUvForWhite} = RecordIMGUIService.getSetting(record);

  let baseIndex = currentFontTextureDrawDataBaseIndex;

  /* let x = x |> NumberType.intToFloat;
  let y = y |> NumberType.intToFloat;
  let width = width |> NumberType.intToFloat;
  let height = height |> NumberType.intToFloat; */

  let verticeArr = [|
    x,
    y,
    x,
    y +. height,
    x +. width,
    y,
    x +. width,
    y +. height,
  |];

  {
    ...record,
    webglData:
      Some({
        ...webglData,
        currentFontTextureDrawDataBaseIndex:
          currentFontTextureDrawDataBaseIndex
          + DrawDataArrayService.getBaseIndex(verticeArr),
      }),
    drawDataArr:
      drawDataArr
      |> ArrayService.push({
           drawType: FontTexture,
           customTexture: None,
           verticeArr,
           colorArr:
             color
             |> DrawDataArrayService.addPoints(color)
             |> DrawDataArrayService.addPoints(color)
             |> DrawDataArrayService.addPoints(color),
           texCoordArr:
             fontTexUvForWhite
             |> DrawDataArrayService.addPoints(fontTexUvForWhite)
             |> DrawDataArrayService.addPoints(fontTexUvForWhite)
             |> DrawDataArrayService.addPoints(fontTexUvForWhite),
           indexArr: [|
             baseIndex,
             baseIndex + 1,
             baseIndex + 2,
             baseIndex + 3,
             baseIndex + 2,
             baseIndex + 1,
           |],
         }),
  };
};