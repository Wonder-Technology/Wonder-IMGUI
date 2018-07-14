open IMGUIType;

open Gl;

open Js.Typed_array;

let _createArrayBuffer = gl => {
  let buffer = gl |> createBuffer |> bindBuffer(getArrayBuffer(gl), _, gl);

  bufferFloat32Data(
    getArrayBufferBuffer(gl),
    Float32Array.make([||]),
    getDynamicDraw(gl),
    gl,
  );

  buffer;
};

let _createElementArrayBuffer = gl => {
  let buffer = gl |> createBuffer |> bindBuffer(getArrayBuffer(gl), _, gl);

  bufferFloat32Data(
    getElementArrayBufferBuffer(gl),
    Uint16Array.make([||]),
    getDynamicDraw(gl),
    gl,
  );

  buffer;
};

let _createFontTexture = gl => {
  let texture = createTexture(gl);
  let target = getTexture2D(gl);

  let gl =
    gl
    |> bindTexture(target, texture)
    |> texParameteri(target, getTextureMinFilter(gl), getLinear(gl))
    |> texParameteri(target, getTextureMagFilter(gl), getLinear(gl))
    |> texParameteri(target, getTextureWrapS(gl), getClampToEdge(gl))
    |> texParameteri(target, getTextureWrapT(gl), getClampToEdge(gl));

  let (uint8Array, width, height) = FontBitmapData.uint8ArrayData;

  let format = getRgba(gl);

  gl
  |> texImage2DWithArrayBufferView(
       target,
       0,
       format,
       width,
       height,
       format,
       getUnsignedByte(gl),
       uint8Array,
     );

  texture;
};

let init = (gl, record) => {
  let program =
    gl
    |> createProgram
    |> ProgramService.initShader(ShaderData.vs, ShaderData.fs, gl);

  let positionBuffer = _createArrayBuffer(gl);
  let colorBuffer = _createArrayBuffer(gl);
  let texCoordBuffer = _createArrayBuffer(gl);
  let indexBuffer = _createElementArrayBuffer(gl);

  let fontTexture = _createFontTexture(gl);

  {
    ...record,
    webglData:
      Some({
        program,
        positionBuffer,
        colorBuffer,
        texCoordBuffer,
        indexBuffer,
        fontTexture,
        aPositonLocation: gl |> getAttribLocation(program, "a_position"),
        aColorLocation: gl |> getAttribLocation(program, "a_color"),
        aTexCoordLocation: gl |> getAttribLocation(program, "a_texCoord"),
        uProjectionMatLocation:
          gl |> getUniformLocation(program, "u_projectionMat"),
        uSampler2DLocation: gl |> getUniformLocation(program, "u_sampler2D"),
        lastWebglData: None,
      }),
  };
};

let _prepare = record =>
  /* {
             ...record,
     webglData: {
       ...record.webglData,

     }
         } */
  record;

let _unbindVAO = gl =>
  switch (getExtension("OES_vertex_array_object") |> Obj.magic |> Js.toOption) {
  | Some(ext) => ext |> unbindVertexArrayOES(Js.Nullable.null)
  | None => ()
  };

let _backupGlState = (gl, record) => {
  ...record,
  webglData:
    Some({
      ...record.webglData,
      lastWebglData:
        Some({
          lastProgram: gl |> getParameter(getCurrentProgram(gl)),
          lastElementArrayBuffer:
            gl |> getParameter(getBindingElementArrayBuffer(gl)),
          lastArrayBuffer: gl |> getParameter(getBindingArrayBuffer(gl)),
          lastTexture: gl |> getParameter(getBindingTexture2D(gl)),
          lastIsEnableDepthTest:
            gl |> getParameter(getIsEnabled(getDepthTest(gl))),
          lastIsEnableBlend: gl |> getParameter(getIsEnabled(getBlend(gl))),
        }),
    }),
};

let _getWebglData = record => record.webglData |> OptionService.unsafeGet;

let _getLastWebglData = record =>
  _getWebglData(record).lastWebglData |> OptionService.unsafeGet;

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
    getArrayBuffer(gl),
    Uint16Array.make(pointArr),
    getDynamicDraw(gl),
    gl,
  );

  gl;
};

let _bufferAllData = (gl, groupedDrawDataArr, record) => {
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
    _getWebglData(record);

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
             offset,
             totalVerticeArr,
             totalColorArr,
             totalTexCoordArr,
             totalIndexArr,
           ),
           {drawType, verticeArr, colorArr, texCoordArr, indexArr}: DrawDataArrType.drawData,
         ) => {
           let count = indexArr |> Js.Array.length;

           (
             drawElementsDataArr
             |> ArrayService.push(
                  {drawType, count, offset}: DrawDataArrType.drawElementsData,
                ),
             offset + count,
             totalVerticeArr |> Js.Array.concat(verticeArr),
             totalColorArr |> Js.Array.concat(colorArr),
             totalTexCoordArr |> Js.Array.concat(texCoordArr),
             totalIndexArr |> Js.Array.concat(indexArr),
           );
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
  |> _bufferArrayBufferData((colorBuffer, totalColorArr, aColorLocation, 4))
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

/* TODO test */
let _groupByDrawTypeAndCustomTexture = (getDrawTypeFunc, drawDataArr) => {
  let (totalResultArr, _) =
    drawDataArr
    |> Js.Array.copy
    |> Js.Array.sortInPlaceWith((valueA, valueB) =>
         getDrawTypeFunc(valueA) - getDrawTypeFunc(valueB)
       )
    |> Js.Array.sortInPlaceWith((valueA, valueB) =>
         switch (valueA.customTexture, valueB.customTexture) {
         | (Some(customTextureA), Some(customTextureB)) =>
           customTextureA === customTextureB ? 0 : 1
         | _ => 0
         }
       )
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (.
           (totalResultArr, oneGroupDrawData),
           {verticeArr, colorArr, texCoordArr, indexArr} as drawData,
         ) => {
           let lastDrawType = oneGroupDrawData |> getDrawTypeFunc;

           getDrawTypeFunc(drawData) === lastDrawType ?
             switch (lastDrawType) {
             | FontTexture => (
                 totalResultArr,
                 {
                   ...oneGroupDrawData,
                   verticeArr:
                     oneGroupDrawData.verticeArr
                     |> Js.Array.concat(verticeArr),
                   colorArr:
                     oneGroupDrawData.colorArr |> Js.Array.concat(colorArr),
                   texCoordArr:
                     oneGroupDrawData.texCoordArr
                     |> Js.Array.concat(texCoordArr),
                   indexArr:
                     oneGroupDrawData.indexArr |> Js.Array.concat(indexArr),
                 },
               )
             | CustomTexture =>
               let currentCustomTexture =
                 drawData.customTexture |> OptionService.unsafeGet;
               let lastCustomTexture =
                 oneGroupDrawData.customTexture |> OptionService.unsafeGet;

               currentCustomTexture === lastCustomTexture ?
                 (
                   totalResultArr,
                   {
                     ...oneGroupDrawData,
                     verticeArr:
                       oneGroupDrawData.verticeArr
                       |> Js.Array.concat(verticeArr),
                     colorArr:
                       oneGroupDrawData.colorArr |> Js.Array.concat(colorArr),
                     texCoordArr:
                       oneGroupDrawData.texCoordArr
                       |> Js.Array.concat(texCoordArr),
                     indexArr:
                       oneGroupDrawData.indexArr |> Js.Array.concat(indexArr),
                   },
                 ) :
                 (totalResultArr |> push(oneGroupResultArr), [|drawData|]);
             } :
             (totalResultArr |> push(oneGroupResultArr), [|drawData|]);
         },
         ([||], [||]),
       );

  totalResultArr;
};

let _buildOrthoProjectionMat4TypeArr = ((canvasWidth, canvasHeigt)) =>
  Matrix4Service.ortho(
    0.,
    canvasWidth |> NumberType.intToFloat,
    canvasHeigt |> NumberType.intToFloat,
    0.,
    -1.,
    1.,
  );

let _setGlState = gl => {
  /* no depth testing; we handle this by manually placing out widgets in the order we wish them to be rendered.  */
  disable(getDepthTest(gl), gl);

  /* for text rendering, enable alpha blending. */
  enable(getBlend(gl), gl);
  blendFunc(getSrcAlpha(gl), getOneMinusSrcAlpha(gl), gl);
};

let _draw = (gl, drawElementsDataArr, record) => {
  let {fontTexture, uSampler2DLocation} = _getWebglData(record);

  drawElementsDataArr
  |> WonderCommonlib.ArrayService.forEach(
       (.
         {drawType, customTexture, count, offset}: DrawDataArrType.drawElementsData,
       ) => {
       let texture =
         switch (drawType) {
         | DrawDataArrType.FontTexture => fontTexture
         | DrawDataArrType.CustomTexture =>
           customTexture |> OptionService.unsafeGet
         | type_ =>
           WonderLog.Log.fatal(
             WonderLog.Log.buildFatalMessage(
               ~title="_finish",
               ~description={j|unknown type_: $type_|j},
               ~reason="",
               ~solution={j||j},
               ~params={j||j},
             ),
           )
         };

       bindTexture(getTexture2D(gl), texture, gl);
       uniform1i(uSampler2DLocation, 0, gl);

       drawElements(getTriangles(gl), count, getUnsignedShort(gl), offset);
     });

  record;
};

let _restoreGlState = (gl, record) => {
  let {
    lastProgram,
    lastElementArrayBuffer,
    lastArrayBuffer,
    lastTexture,
    lastIsEnableDepthTest,
    lastIsEnableBlend,
  } =
    _getLastWebglData(record);

  bindBuffer(getElementArrayBuffer(gl), lastElementArrayBuffer, gl);

  bindBuffer(getArrayBuffer(gl), lastArrayBuffer, gl);

  useProgram(lastProgram, gl);

  bindTexture(getTexture2D(gl), lastTexture);

  lastIsEnableDepthTest ?
    enable(getDepthTest(gl), gl) : disable(getDepthTest(gl), gl);

  lastIsEnableblend ? enable(getBlend(gl), gl) : disable(getBlend(gl), gl);

  record;
};

let _finish = (gl, canvasSize, record) => {
  /*
   If a VAO is already bound, we need to unbound it. Otherwise, we will write into a VAO created by the user of the library
   when calling vertexAttribPointer, which means that we would effectively corrupt the user data!
   */
  _unbindVAO(gl);

  /*
   We are changing some GL states when rendering the GUI. So before rendering we backup these states,
   and after rendering we restore these states. This is so that the end-user does not involuntarily have his
   GL-states messed with.
   */
  let {drawDataArr} as record = _backupGlState(gl, record);

  let groupedDrawDataArr =
    drawDataArr
    |> _groupByDrawTypeAndCustomTexture(drawData =>
         drawData.drawType |> DrawDataArrType.drawTypeToInt
       );

  let (record, drawElementsDataArr) =
    _bufferAllData(gl, groupedDrawDataArr, record);

  let {program, fontTexture, uProjectionMatLocation, uSampler2DLocation} =
    _getWebglData(record);

  useProgram(program, gl);

  _setGlState(gl);

  uniformMatrix4fv(
    uProjectionMatLocation,
    false,
    _buildOrthoProjectionMat4TypeArr((canvasWidth, canvasHeigt)),
  );

  record |> _draw(gl, drawElementsDataArr) |> _restoreGlState(gl);
};

let _getIMGUIFunc = ({imguiFunc}) => imguiFunc;

let setIMGUIFunc = (func, record) => {...record, imguiFunc: func};

let _exec = record => (_getIMGUIFunc(record))(. record);

let unsafeGetSetting = record => record.setting |> OptionService.unsafeGet;

let setSetting = (setting, record) => {...record, setting};

let render = (gl, canvasSize, record) =>
  record |> _prepare |> _exec |> _finish(gl, canvasSize);

let createRecord = () => {
  setting: {
    textScale: 1.0,
    textColor: [|1., 1., 1.|],
  },
  fftData: FontFFTData.hashMapData,
  webglData: None,
  drawDataArr: [||],
  /* ioData: {
       mouseLeftDownCur: false,
       mouseLeftDownPrev: false,
       mousePositionCur: (0, 0),
       mousePositionPrev: (0, 0),
     }, */
  imguiFunc: None,
};