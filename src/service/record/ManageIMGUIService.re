open DrawDataArrType;

open IMGUIType;

open WonderWebgl.Gl;

open Js.Typed_array;

let _createArrayBuffer = gl => {
  let buffer = gl |> createBuffer;

  bindBuffer(getArrayBuffer(gl), buffer, gl);

  bufferFloat32Data(
    getArrayBuffer(gl),
    Float32Array.make([||]),
    getDynamicDraw(gl),
    gl,
  );

  buffer;
};

let _createElementArrayBuffer = gl => {
  let buffer = gl |> createBuffer;

  bindBuffer(getElementArrayBuffer(gl), buffer, gl);

  bufferUint16Data(
    getElementArrayBuffer(gl),
    Uint16Array.make([||]),
    getDynamicDraw(gl),
    gl,
  );

  buffer;
};

let _createFontTexture = (gl, source) => {
  let texture = createTexture(gl);
  let target = getTexture2D(gl);

  bindTexture(target, texture, gl);

  texParameteri(target, getTextureMinFilter(gl), getLinear(gl), gl);
  texParameteri(target, getTextureMagFilter(gl), getLinear(gl), gl);
  texParameteri(target, getTextureWrapS(gl), getClampToEdge(gl), gl);
  texParameteri(target, getTextureWrapT(gl), getClampToEdge(gl), gl);

  let format = getRgba(gl);

  texImage2D(target, 0, format, format, getUnsignedByte(gl), source, gl);

  texture;
};

let init = (gl, record) =>
  ! AssetIMGUIService.isLoadAsset(record) ?
    record :
    {
      let program =
        gl
        |> createProgram
        |> WonderWebgl.ProgramService.initShader(
             ShaderData.vs,
             ShaderData.fs,
             gl,
           );

      let positionBuffer = _createArrayBuffer(gl);
      let colorBuffer = _createArrayBuffer(gl);
      let texCoordBuffer = _createArrayBuffer(gl);
      let indexBuffer = _createElementArrayBuffer(gl);

      let fontTexture =
        _createFontTexture(
          gl,
          AssetIMGUIService.unsafeGetBitmap(record)
          |> WonderWebgl.GlType.imageElementToTextureSource,
        );

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
            uSampler2DLocation:
              gl |> getUniformLocation(program, "u_sampler2D"),
            lastWebglData: None,
            currentFontTextureDrawDataBaseIndex: 0,
          }),
      };
    };

let _prepare = record => {
  ...record,
  drawDataArr: [||],
  webglData:
    Some({
      ...RecordIMGUIService.unsafeGetWebglData(record),
      currentFontTextureDrawDataBaseIndex: 0,
    }),
};
/* record; */

let _unbindVAO = gl =>
  switch (getExtension("OES_vertex_array_object", gl) |> Js.toOption) {
  | Some(ext) => ext |> Obj.magic |> unbindVertexArrayOES(Js.Nullable.null)
  | None => ()
  };

let _backupGlState = (gl, record) => {
  ...record,
  webglData:
    Some({
      ...RecordIMGUIService.unsafeGetWebglData(record),
      lastWebglData:
        Some({
          lastProgram:
            gl
            |> getParameter(getCurrentProgram(gl))
            |> WonderWebgl.GlType.parameterIntToNullableProgram
            |> Js.Nullable.toOption,
          lastElementArrayBuffer:
            gl
            |> getParameter(getBindingElementArrayBuffer(gl))
            |> WonderWebgl.GlType.parameterIntToBuffer,
          lastArrayBuffer:
            gl
            |> getParameter(getBindingArrayBuffer(gl))
            |> WonderWebgl.GlType.parameterIntToBuffer,
          lastTexture:
            gl
            |> getParameter(getBindingTexture2D(gl))
            |> WonderWebgl.GlType.parameterIntToNullableTexture
            |> Js.Nullable.toOption,
          lastIsEnableDepthTest: gl |> isEnabled(getDepthTest(gl)),
          lastIsEnableBlend: gl |> isEnabled(getBlend(gl)),
        }),
    }),
};

let _getLastWebglData = record =>
  RecordIMGUIService.unsafeGetWebglData(record).lastWebglData
  |> OptionService.unsafeGet;

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

           (
             drawElementsDataArr
             |> ArrayService.push(
                  {drawType, customTexture, count, countOffset}: drawElementsData,
                ),
             countOffset + count * 2,
             totalVerticeArr |> Js.Array.concat(verticeArr),
             totalColorArr |> Js.Array.concat(colorArr),
             totalTexCoordArr |> Js.Array.concat(texCoordArr),
             totalIndexArr
             |> Js.Array.concat(
                  indexArr |> Js.Array.map(index => index + baseIndex),
                ),
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

let _groupByDrawTypeAndCustomTexture = (drawDataArr: drawDataArr) =>
  drawDataArr |> Js.Array.length === 0 ?
    [||] :
    {
      let sortedDrawDataArr =
        drawDataArr
        |> Js.Array.copy
        |> Js.Array.sortInPlaceWith((valueA: drawData, valueB: drawData) =>
             (valueA.drawType |> drawTypeToInt)
             - (valueB.drawType |> drawTypeToInt)
           )
        |> Js.Array.sortInPlaceWith((valueA: drawData, valueB: drawData) =>
             switch (valueA.customTexture, valueB.customTexture) {
             | (Some(customTextureA), Some(customTextureB)) =>
               customTextureA === customTextureB ? 0 : 1
             | _ => 0
             }
           );

      let (totalResultArr, oneGroupDrawData) =
        sortedDrawDataArr
        |> Js.Array.sliceFrom(1)
        |> WonderCommonlib.ArrayService.reduceOneParam(
             (.
               (totalResultArr, oneGroupDrawData: drawData),
               ({verticeArr, colorArr, texCoordArr, indexArr}: drawData) as drawData,
             ) => {
               let lastDrawType = oneGroupDrawData.drawType;

               drawData.drawType
               |> drawTypeToInt === (lastDrawType |> drawTypeToInt) ?
                 switch (lastDrawType) {
                 | FontTexture => (
                     totalResultArr,
                     /* TODO duplicate */
                     {
                       ...oneGroupDrawData,
                       verticeArr:
                         oneGroupDrawData.verticeArr
                         |> Js.Array.concat(verticeArr),
                       colorArr:
                         oneGroupDrawData.colorArr
                         |> Js.Array.concat(colorArr),
                       texCoordArr:
                         oneGroupDrawData.texCoordArr
                         |> Js.Array.concat(texCoordArr),
                       indexArr:
                         oneGroupDrawData.indexArr
                         |> Js.Array.concat(indexArr),
                     }: drawData,
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
                           oneGroupDrawData.colorArr
                           |> Js.Array.concat(colorArr),
                         texCoordArr:
                           oneGroupDrawData.texCoordArr
                           |> Js.Array.concat(texCoordArr),
                         indexArr:
                           oneGroupDrawData.indexArr
                           |> Js.Array.concat(indexArr),
                       },
                     ) :
                     /* TODO duplicate */
                     (
                       totalResultArr |> ArrayService.push(oneGroupDrawData),
                       drawData,
                     );
                 } :
                 (
                   totalResultArr |> ArrayService.push(oneGroupDrawData),
                   drawData,
                 );
             },
             ([||], sortedDrawDataArr[0]),
           );

      totalResultArr |> ArrayService.push(oneGroupDrawData);
    };

let _buildOrthoProjectionMat4TypeArr = ((canvasWidth, canvasHeight)) =>
  WonderWebgl.Matrix4Service.ortho(
    0.,
    canvasWidth |> NumberType.intToFloat,
    canvasHeight |> NumberType.intToFloat,
    0.,
    -1.,
    1.,
    WonderWebgl.Matrix4Service.createIdentityMatrix4(),
  );

let _setGlState = gl => {
  /* no depth testing; we handle this by manually placing out widgets in the order we wish them to be rendered.  */
  disable(getDepthTest(gl), gl);

  /* for text rendering, enable alpha blending. */
  enable(getBlend(gl), gl);
  blendFunc(getSrcAlpha(gl), getOneMinusSrcAlpha(gl), gl);
};

let _draw = (gl, drawElementsDataArr, record) => {
  let {fontTexture, uSampler2DLocation} =
    RecordIMGUIService.unsafeGetWebglData(record);

  drawElementsDataArr
  |> WonderCommonlib.ArrayService.forEach(
       (. {drawType, customTexture, count, countOffset}: drawElementsData) => {
       let texture =
         switch (drawType) {
         | FontTexture => fontTexture
         | CustomTexture => customTexture |> OptionService.unsafeGet
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

       drawElements(
         getTriangles(gl),
         count,
         getUnsignedShort(gl),
         countOffset,
         gl,
       );
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

  switch (lastProgram) {
  | Some(lastProgram) => useProgram(lastProgram, gl)
  | None => ()
  };

  switch (lastTexture) {
  | Some(lastTexture) => bindTexture(getTexture2D(gl), lastTexture, gl)
  | None => ()
  };

  lastIsEnableDepthTest ?
    enable(getDepthTest(gl), gl) : disable(getDepthTest(gl), gl);

  lastIsEnableBlend ? enable(getBlend(gl), gl) : disable(getBlend(gl), gl);

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

  let groupedDrawDataArr = drawDataArr |> _groupByDrawTypeAndCustomTexture;

  let (record, drawElementsDataArr) =
    _bufferAllData(gl, groupedDrawDataArr, record);

  let {program, fontTexture, uProjectionMatLocation, uSampler2DLocation} =
    RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(program, gl);

  _setGlState(gl);

  uniformMatrix4fv(
    uProjectionMatLocation,
    false,
    _buildOrthoProjectionMat4TypeArr(canvasSize),
    gl,
  );

  record |> _draw(gl, drawElementsDataArr) |> _restoreGlState(gl);
};

let getIMGUIFunc = ({imguiFunc}) => imguiFunc;

let setIMGUIFunc = (func, record) => {...record, imguiFunc: Some(func)};

let _exec = record =>
  switch (getIMGUIFunc(record)) {
  | None => record
  | Some(func) => func(. record)
  };

let getSetting = record => record.setting;

let setSetting = (setting, record) => {...record, setting};

let render = (gl, canvasSize, record) =>
  ! AssetIMGUIService.isLoadAsset(record) ?
    record : record |> _prepare |> _exec |> _finish(gl, canvasSize);

let createRecord = () => {
  setting:
    /* textScale: 1.0, */
    {
      textColorArr: [|1., 1., 1.|],
    },
  assetData: {
    fntId: "fnt",
    bitmapId: "bitmap",
    fntDataMap: WonderCommonlib.HashMapService.createEmpty(),
    bitmapMap: WonderCommonlib.HashMapService.createEmpty(),
  },
  fontData: None,
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