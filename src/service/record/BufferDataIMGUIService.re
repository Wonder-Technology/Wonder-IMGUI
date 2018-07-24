open DrawDataType;

open IMGUIType;

open WonderWebgl.Gl;

open Js.Typed_array;

let _bufferArrayBufferData = ((buffer, pointArr, location, size), gl) => {
  bindBuffer(getArrayBuffer(gl), buffer, gl);
  bufferFloat32Data(
    getArrayBuffer(gl),
    Float32Array.make(pointArr),
    getDynamicDraw(gl),
    gl,
  );

  enableVertexAttribArray(location, gl);
  vertexAttribPointer(location, size, getFloat(gl), false, 0, 0, gl);
  resetBuffer(getArrayBuffer(gl), Js.Nullable.null, gl);

  gl;
};

let _bufferElementArrayBufferData = (buffer, pointArr, gl) => {
  bindBuffer(getElementArrayBuffer(gl), buffer, gl);
  bufferUint16Data(
    getElementArrayBuffer(gl),
    Uint16Array.make(pointArr),
    getDynamicDraw(gl),
    gl,
  );

  gl;
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

  let (
    drawElementsDataArr,
    _,
    totalVerticeArr,
    totalColorArr,
    totalTexCoordArr,
    totalIndexArr,
  ) =
    groupedDrawDataArr
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
           }: DrawDataType.drawData,
         ) => {
           let count = indexArr |> Js.Array.length;

           switch (count) {
           | 0 => (
               drawElementsDataArr,
               0,
               totalVerticeArr,
               totalColorArr,
               totalTexCoordArr,
               totalIndexArr,
             )
           | count =>
             let newCountOffset = countOffset + count * 2;

             (
               drawElementsDataArr
               |> ArrayService.push(
                    {drawType, customTexture, count, countOffset}: drawElementsData,
                  ),
               newCountOffset,
               /* TODO optimize */
               totalVerticeArr |> Js.Array.concat(verticeArr),
               totalColorArr |> Js.Array.concat(colorArr),
               totalTexCoordArr |> Js.Array.concat(texCoordArr),
               totalIndexArr |> Js.Array.concat(indexArr),
             );
           };
         },
         ([||], 0, [||], [||], [||], [||]),
       );

  gl
  |> _bufferArrayBufferData((
       positionBuffer,
       totalVerticeArr,
       aPositonLocation,
       2,
     ))
  |> _bufferArrayBufferData((colorBuffer, totalColorArr, aColorLocation, 3))
  |> _bufferArrayBufferData((
       texCoordBuffer,
       totalTexCoordArr,
       aTexCoordLocation,
       2,
     ))
  |> _bufferElementArrayBufferData(indexBuffer, totalIndexArr)
  |> ignore;

  (record, drawElementsDataArr);
};

let coloredVertex =
    (position, color, (verticeArr, colorArr, texCoordArr), fontTexUvForWhite) => (
  verticeArr |> Js.Array.concat(position),
  colorArr |> Js.Array.concat(color),
  texCoordArr |> Js.Array.concat(fontTexUvForWhite),
);