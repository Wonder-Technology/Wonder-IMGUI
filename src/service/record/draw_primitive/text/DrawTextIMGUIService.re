open IMGUIType;

open DrawDataArrType;

let _generateVertices = ({position, data, index, line}, verticeArr) => {
  let (charX, charY, charWidth, charHeight) = data.rect;
  /*
   TODO need convert?
   position = CoordinateUtils.convertLeftCornerPositionToCenterPositionInWebGL(Vector2.create(layoutCharData.position[0], layoutCharData.position[1]), bitmapFontWidth, bitmapFontHeight), */
  let (x, y) = position;
  let w = charWidth |> NumberType.intToFloat;
  let h = charHeight |> NumberType.intToFloat;
  let x = x |> NumberType.intToFloat;
  let y = y |> NumberType.intToFloat;

  verticeArr
  |> DrawDataArrayService.addPoints([|
       x,
       -. y,
       x,
       -. (y +. h),
       x +. w,
       -. (y +. h),
       x +. w,
       -. y,
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
  let (charX, charY, charWidth, charHeight) = data.rect;
  let bw = charX + charWidth;
  let bh = charY + charHeight;
  let u0 =
    (charX |> NumberType.intToFloat) /. (textureWidth |> NumberType.intToFloat);
  let v0 =
    (charY |> NumberType.intToFloat)
    /. (textureHeight |> NumberType.intToFloat);
  let u1 =
    (bw |> NumberType.intToFloat) /. (textureWidth |> NumberType.intToFloat);
  let v1 =
    (bh |> NumberType.intToFloat) /. (textureHeight |> NumberType.intToFloat);

  /* if (flipY) {
         v0= (textureHeight - rect.y) / textureHeight;
         v1 = (textureHeight - bh) / textureHeight;
     } */

  texCoordArr
  |> DrawDataArrayService.addPoints([|u0, v0, u0, v1, u1, v1, u1, v0|]);
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
  let fntData = AssetIMGUIService.unsafeGetFntData(record);

  let layoutDataArr =
    BitmapFontLayoutIMGUIService.getLayoutData(
      str,
      (width, 4, 0, align),
      record,
    );

  let (verticeArr, colorArr, texCoordArr, indexArr) =
    layoutDataArr
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (. (verticeArr, colorArr, texCoordArr, indexArr), layoutData) => {
           let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

           (
             verticeArr |> _generateVertices(layoutData),
             colorArr |> _generateColor(textColorArr),
             texCoordArr
             |> _generateTexCoords(layoutData, fntData.scaleW, fntData.scaleH),
             indexArr |> _generateIndices(baseIndex),
           );
         },
         ([||], [||], [||], [||]),
       );

  {
    ...record,
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