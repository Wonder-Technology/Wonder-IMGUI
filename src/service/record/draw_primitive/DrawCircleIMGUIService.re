open IMGUIType;

open DrawDataArrType;

let _unitCircle = (centerX, centerY, theta, radius) => [|
  centerX +. radius *. Js.Math.cos(theta),
  centerY +. radius *. Js.Math.sin(theta),
|];

let rec _drawCircle =
        (
          (
            centerX,
            centerY,
            color,
            fontTexUvForWhite,
            centerVertexIndex,
            radius,
            stepSize,
            max,
          ),
          theta,
          curIndex,
          (verticeArr, colorArr, texCoordArr, indexArr),
        ) =>
  theta <= max ?
    {
      let (verticeArr, colorArr, texCoordArr) =
        BufferDataIMGUIService.coloredVertex(
          _unitCircle(centerX, centerY, theta, radius),
          color,
          (verticeArr, colorArr, texCoordArr),
          fontTexUvForWhite,
        );

      /* let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr); */

      let indexArr =
        theta !== 0. ?
          indexArr
          |> Js.Array.concat([|
               /* baseIndex + curIndex + 0,
               baseIndex + curIndex - 1, */
                curIndex + 0,
                curIndex - 1,
                centerVertexIndex,
             |]) :
          indexArr;

      _drawCircle(
        (
          centerX,
          centerY,
          color,
          fontTexUvForWhite,
          centerVertexIndex,
          radius,
          stepSize,
          max,
        ),
        theta +. stepSize,
        curIndex |> succ,
        (verticeArr, colorArr, texCoordArr, indexArr),
      );
    } :
    (verticeArr, colorArr, texCoordArr, indexArr);

/*
 Render a circle, where the top-left corner of the circle is `position`
 Where `segments` is how many triangle segments the triangle is rendered with.
 */
let draw = ((x, y, width, height), color, segments, {drawDataArr} as record) => {
  let {currentFontTextureDrawDataBaseIndex} as webglData =
    RecordIMGUIService.unsafeGetWebglData(record);
  let {fontTexUvForWhite} = RecordIMGUIService.getSetting(record);

  let baseIndex = currentFontTextureDrawDataBaseIndex;

  let centerX = x +. 0.5 *. width;
  let centerY = y +. 0.5 *. height;

  let radius = width /. 2.;

  let verticeArr = [||];
  let colorArr = [||];
  let texCoordArr = [||];
  let indexArr = [||];

  /* add center vertex. */
  let (verticeArr, colorArr, texCoordArr) =
    BufferDataIMGUIService.coloredVertex(
      [|centerX, centerY|],
      color,
      (verticeArr, colorArr, texCoordArr),
      fontTexUvForWhite,
    );

  let centerVertexIndex = baseIndex + 0;

  let (verticeArr, colorArr, texCoordArr, indexArr) =
    _drawCircle(
      (
        centerX,
        centerY,
        color,
        fontTexUvForWhite,
        centerVertexIndex,
        radius,
        2. *. Js.Math._PI /. (segments |> NumberType.intToFloat),
        2. *. Js.Math._PI +. 0.1,
      ),
      0.,
      baseIndex + 1,
      (verticeArr, colorArr, texCoordArr, indexArr),
    );

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
           colorArr,
           texCoordArr,
           indexArr,
         }),
  };
};