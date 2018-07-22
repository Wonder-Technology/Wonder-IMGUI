open DrawDataArrType;

open IMGUIType;

open WonderWebgl.Gl;

open Js.Typed_array;

let _bufferArrayBufferData =
    (isNeedUpdateBufferData, (buffer, pointArr, location, size), gl) => {
  bindBuffer(getArrayBuffer(gl), buffer, gl);
  isNeedUpdateBufferData ?
    bufferFloat32Data(
      getArrayBuffer(gl),
      Float32Array.make(pointArr),
      getDynamicDraw(gl),
      gl,
    ) :
    ();

  enableVertexAttribArray(location, gl);
  vertexAttribPointer(location, size, getFloat(gl), false, 0, 0, gl);
  resetBuffer(getArrayBuffer(gl), Js.Nullable.null, gl);

  gl;
};

let _bufferElementArrayBufferData =
    (isNeedUpdateBufferData, buffer, pointArr, gl) => {
  bindBuffer(getElementArrayBuffer(gl), buffer, gl);
  isNeedUpdateBufferData ?
    bufferUint16Data(
      getElementArrayBuffer(gl),
      Uint16Array.make(pointArr),
      getDynamicDraw(gl),
      gl,
    ) :
    ();

  gl;
};

let _addCustomTextureDrawDataBaseIndex = ({verticeArr, indexArr} as drawData) => {
  let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

  {
    ...drawData,
    indexArr:
      indexArr
      |> ArrayService.chunk(6)
      |> WonderCommonlib.ArrayService.reduceOneParam(
           (. resultArr, singleDrawDataIndexArr) => {
             let totalIndexCount = (resultArr |> Js.Array.length) / 6 * 4;

             resultArr
             |> Js.Array.concat(
                  singleDrawDataIndexArr
                  |> Js.Array.map(index => index + totalIndexCount),
                );
           },
           [||],
         ),
  };
};

let _isNeedUpdateBufferData = record => record.needUpdateBufferData;

let markNotNeedUpdateBufferData = record => {
  ...record,
  needUpdateBufferData: false,
};

let markNeedUpdateBufferData = record => {
  ...record,
  needUpdateBufferData: true,
};

let bufferAllData = (gl, groupedDrawDataArr, record) => {
  let {
    program,
    positionBuffer,
    colorBuffer,
    texCoordBuffer,
    indexBuffer,
    aPositonLocation,
    aColorLocation,
    aTexCoordLocation,
  } =
    RecordIMGUIService.unsafeGetWebglData(record);

  let isNeedUpdateBufferData = _isNeedUpdateBufferData(record);

  let (
    drawElementsDataArr,
    _,
    totalVerticeArr,
    totalColorArr,
    totalTexCoordArr,
    totalIndexArr,
  ) =
    groupedDrawDataArr
    |> Js.Array.map((({drawType}: drawData) as drawData) =>
         switch (drawType) {
         | CustomTexture => _addCustomTextureDrawDataBaseIndex(drawData)
         | _ => drawData
         }
       )
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (.
           (
             drawElementsDataArr,
             countOffset,
             totalVerticeArr,
             totalColorArr,
             totalTexCoordArr,
             totalIndexArr,
           ),
           {
             drawType,
             customTexture,
             verticeArr,
             colorArr,
             texCoordArr,
             indexArr,
           }: drawData,
         ) => {
           let count = indexArr |> Js.Array.length;

           let baseIndex = DrawDataArrayService.getBaseIndex(totalVerticeArr);

           let newCountOffset = countOffset + count * 2;

           isNeedUpdateBufferData ?
             (
               drawElementsDataArr
               |> ArrayService.push(
                    {drawType, customTexture, count, countOffset}: drawElementsData,
                  ),
               newCountOffset,
               totalVerticeArr |> Js.Array.concat(verticeArr),
               totalColorArr |> Js.Array.concat(colorArr),
               totalTexCoordArr |> Js.Array.concat(texCoordArr),
               totalIndexArr
               |> Js.Array.concat(
                    indexArr |> Js.Array.map(index => index + baseIndex),
                  ),
             ) :
             (
               drawElementsDataArr
               |> ArrayService.push(
                    {drawType, customTexture, count, countOffset}: drawElementsData,
                  ),
               newCountOffset,
               totalVerticeArr,
               totalColorArr,
               totalTexCoordArr,
               totalIndexArr,
             );
         },
         ([||], 0, [||], [||], [||], [||]),
       );

  gl
  |> _bufferArrayBufferData(
       isNeedUpdateBufferData,
       (positionBuffer, totalVerticeArr, aPositonLocation, 2),
     )
  |> _bufferArrayBufferData(
       isNeedUpdateBufferData,
       (colorBuffer, totalColorArr, aColorLocation, 3),
     )
  |> _bufferArrayBufferData(
       isNeedUpdateBufferData,
       (texCoordBuffer, totalTexCoordArr, aTexCoordLocation, 2),
     )
  |> _bufferElementArrayBufferData(
       isNeedUpdateBufferData,
       indexBuffer,
       totalIndexArr,
     )
  |> ignore;

  (record |> markNotNeedUpdateBufferData, drawElementsDataArr);
};

let coloredVertex =
    (
      positionArr,
      colorArr,
      (verticeArr, colorArr, texCoordArr),
      fontTexUvForWhite,
    ) => (
  verticeArr |> Js.Array.concat(positionArr),
  colorArr |> Js.Array.concat(colorArr),
  texCoordArr |> Js.Array.concat(fontTexUvForWhite),
);