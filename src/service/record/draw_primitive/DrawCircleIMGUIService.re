open IMGUIType;

open DrawDataType;

let _unitCircle = (centerX, centerY, theta, radius) => [|
  centerX +. radius *. Js.Math.cos(theta),
  centerY +. radius *. Js.Math.sin(theta),
|];

let rec _drawCircle =
        (
          (centerX, centerY, color, centerVertexIndex, radius, stepSize, max),
          theta,
          curIndex,
          (verticeArr, colorArr, indexArr),
        ) =>
  theta <= max ?
    {
      let (verticeArr, colorArr) =
        BufferDataIMGUIService.coloredNoTexturedVertex(
          centerX +. radius *. Js.Math.cos(theta),
          centerY +. radius *. Js.Math.sin(theta),
          color,
          (verticeArr, colorArr),
        );

      let indexArr =
        theta !== 0. ?
          indexArr
          |> ArrayService.push(curIndex + 0)
          |> ArrayService.push(curIndex - 1)
          |> ArrayService.push(centerVertexIndex) :
          /* indexArr
             |> DrawDataArrayService.addPoints([|
                  curIndex + 0,
                  curIndex - 1,
                  centerVertexIndex,
                |]) : */
          indexArr;

      _drawCircle(
        (centerX, centerY, color, centerVertexIndex, radius, stepSize, max),
        theta +. stepSize,
        curIndex |> succ,
        (verticeArr, colorArr, indexArr),
      );
    } :
    (verticeArr, colorArr, indexArr);

/*
 Render a circle, where the top-left corner of the circle is `position`
 Where `segments` is how many triangle segments the triangle is rendered with.
 */
let draw = ((x, y, width, height), color, segments, record) => {
  /* let {currentFontTextureDrawDataBaseIndex} as webglData =
     RecordIMGUIService.unsafeGetWebglData(record); */

  let centerX = x +. 0.5 *. width;
  let centerY = y +. 0.5 *. height;

  let radius = width /. 2.;

  let {verticeArr, colorArr, indexArr} =
    RecordIMGUIService.getNoTextureDrawData(record);

  /* let baseIndex = currentFontTextureDrawDataBaseIndex; */
  let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

  /* add center vertex. */
  let (verticeArr, colorArr) =
    BufferDataIMGUIService.coloredNoTexturedVertex(
      /* [|centerX, centerY|], */
      centerX,
      centerY,
      color,
      (verticeArr, colorArr),
    );

  let centerVertexIndex = baseIndex + 0;

  let (verticeArr, colorArr, indexArr) =
    _drawCircle(
      (
        centerX,
        centerY,
        color,
        centerVertexIndex,
        radius,
        2. *. Js.Math._PI /. (segments |> NumberType.intToFloat),
        2. *. Js.Math._PI +. 0.1,
      ),
      0.,
      baseIndex + 1,
      (verticeArr, colorArr, indexArr),
    );

  {
    ...record,
    drawData: {
      ...record.drawData,
      noTextureDrawData: {
        ...record.drawData.noTextureDrawData,
        verticeArr,
        colorArr,
        indexArr,
      },
    },
  };
};