open IMGUIType;

open FontType;

open DrawDataArrType;

let _generateVertices =
    (posX, posY, {position, data, index, line}, verticeArr) => {
  let (charXInImage, charYInImage, charWidthInImage, charHeightInImage) =
    data.rect;
  let (x, y) = position;
  let w = charWidthInImage |> NumberType.intToFloat;
  let h = charHeightInImage |> NumberType.intToFloat;
  let leftUpX =
    (x |> NumberType.intToFloat) +. (posX |> NumberType.intToFloat);
  let leftUpY =
    (y |> NumberType.intToFloat) +. (posY |> NumberType.intToFloat);

  verticeArr
  |> DrawDataArrayService.addPoints([|
       leftUpX,
       leftUpY,
       leftUpX,
       leftUpY +. h,
       leftUpX +. w,
       leftUpY,
       leftUpX +. w,
       leftUpY +. h,
     |]);
};

let _generateColor = (textColorArr, colorArr) =>
  colorArr
  |> DrawDataArrayService.addPoints(textColorArr)
  |> DrawDataArrayService.addPoints(textColorArr)
  |> DrawDataArrayService.addPoints(textColorArr)
  |> DrawDataArrayService.addPoints(textColorArr);

let _generateTexCoords =
    ({position, data, index, line}, textureWidth, textureHeight, texCoordArr) => {
  /* TODO flipY? */
  let flipY = false;
  let (charXInImage, charYInImage, charWidthInImage, charHeightInImage) =
    data.rect;
  let s0 =
    (charXInImage |> NumberType.intToFloat)
    /. (textureWidth |> NumberType.intToFloat);
  let t0 =
    (charYInImage |> NumberType.intToFloat)
    /. (textureHeight |> NumberType.intToFloat);
  let s1 =
    (charXInImage + charWidthInImage |> NumberType.intToFloat)
    /. (textureWidth |> NumberType.intToFloat);
  let t1 =
    (charYInImage + charHeightInImage |> NumberType.intToFloat)
    /. (textureHeight |> NumberType.intToFloat);

  /* if (flipY) {
         t0= (textureHeight - rect.y) / textureHeight;
         t1 = (textureHeight - bh) / textureHeight;
     } */

  texCoordArr
  |> DrawDataArrayService.addPoints([|s0, t0, s0, t1, s1, t0, s1, t1|]);
};

let _generateIndices = (baseIndex, indexArr) =>
  indexArr
  |> DrawDataArrayService.addPoints([|
       baseIndex,
       baseIndex + 1,
       baseIndex + 2,
       baseIndex + 3,
       baseIndex + 2,
       baseIndex + 1,
     |]);

let draw = ((x, y, width, height), str, align, {drawDataArr} as record) => {
  let {textColorArr} = ManageIMGUIService.getSetting(record);

  switch (AssetIMGUIService.getFntData(record)) {
  | None =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="getLayoutData",
        ~description={j|impossible to create font: not find fnt file|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  | Some(fntData) =>
    let layoutDataArr =
      BitmapFontLayoutIMGUIService.getLayoutData(
        str,
        (width, 4, 0, align),
        fntData,
        record,
      );

    let {currentFontTextureDrawDataBaseIndex} as webglData =
      RecordIMGUIService.unsafeGetWebglData(record);

    let (verticeArr, colorArr, texCoordArr, indexArr) =
      layoutDataArr
      |> WonderCommonlib.ArrayService.reduceOneParam(
           (. (verticeArr, colorArr, texCoordArr, indexArr), layoutData) => {
             let baseIndex =
               DrawDataArrayService.getBaseIndex(verticeArr)
               + currentFontTextureDrawDataBaseIndex;

             (
               verticeArr |> _generateVertices(x, y, layoutData),
               colorArr |> _generateColor(textColorArr),
               texCoordArr
               |> _generateTexCoords(
                    layoutData,
                    fntData.scaleW,
                    fntData.scaleH,
                  ),
               indexArr |> _generateIndices(baseIndex),
             );
           },
           ([||], [||], [||], [||]),
         );

    {
      ...record,
      webglData:
        Some({
          ...webglData,
          currentFontTextureDrawDataBaseIndex:
            DrawDataArrayService.getBaseIndex(verticeArr),
        }),
      drawDataArr:
        drawDataArr
        |> ArrayService.push({
             drawType: FontTexture,
             customTexture: None,
             verticeArr,
             colorArr,
             texCoordArr,
             indexArr,
           }),
    };
  };
};