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

let _sendUniformData = (gl, program, canvasSize) => {
  useProgram(program, gl);

  uniformMatrix4fv(
    gl |> getUniformLocation(program, "u_projectionMat"),
    false,
    _buildOrthoProjectionMat4TypeArr(canvasSize),
    gl,
  );

  uniform1i(gl |> getUniformLocation(program, "u_sampler2D"), 0, gl);

  ();
};

let init = (gl, canvasSize, record) =>
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

      _sendUniformData(gl, program, canvasSize);

      let {customImageArr, customTextureMap} = record.assetData;

      {
        ...record,
        assetData: {
          ...record.assetData,
          customTextureMap:
            AssetIMGUIService.createCustomTextures(
              gl,
              customImageArr,
              customTextureMap,
            ),
        },
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
            lastWebglData: None,
            currentFontTextureDrawDataBaseIndex: 0,
          }),
      };
    };

let _prepare = (ioData, record) => {
  ...record,
  ioData,
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

let _setGlState = gl => {
  /* no depth testing; we handle this by manually placing out widgets in the order we wish them to be rendered.  */
  disable(getDepthTest(gl), gl);

  /* for text rendering, enable alpha blending. */
  enable(getBlend(gl), gl);
  blendFunc(getSrcAlpha(gl), getOneMinusSrcAlpha(gl), gl);
};

let _draw = (gl, drawElementsDataArr, record) => {
  let {fontTexture} = RecordIMGUIService.unsafeGetWebglData(record);

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

let _finish = (gl, record) => {
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
    BufferDataIMGUIService.bufferAllData(gl, groupedDrawDataArr, record);

  let {program} = RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(program, gl);

  _setGlState(gl);

  record |> _draw(gl, drawElementsDataArr) |> _restoreGlState(gl);
};

let getCustomData = ({imguiFuncData}) =>
  imguiFuncData.customDataForIMGUIFunc;

let getIMGUIFunc = ({imguiFuncData}) => imguiFuncData.imguiFunc;

let setIMGUIFunc = (customData, func, record) =>
  {
    ...record,
    imguiFuncData: {
      ...record.imguiFuncData,
      imguiFunc: Some(func),
      customDataForIMGUIFunc: Some(customData),
    },
  }
  |> BufferDataIMGUIService.markNeedUpdateBufferData;

let _getAPIJsObj = ({imguiFuncData}) => imguiFuncData.apiJsObj;

let _buildAPIJsObj = () => {
  "label": FixedLayoutControlIMGUIService.label,
  "image": FixedLayoutControlIMGUIService.image,
  "button": FixedLayoutControlIMGUIService.button,
  "box": FixedLayoutControlIMGUIService.box,
  "beginGroup": GroupLayoutIMGUIService.beginGroup,
  "endGroup": GroupLayoutIMGUIService.endGroup,
};

let _exec = ({imguiFuncData} as record) =>
  switch (getIMGUIFunc(record)) {
  | None => record
  | Some(func) =>
    func(
      imguiFuncData.customDataForIMGUIFunc |> OptionService.unsafeGet,
      _getAPIJsObj(record),
      record,
    )
  };

let render = (gl, ioData, record) =>
  ! AssetIMGUIService.isLoadAsset(record) ?
    record : record |> _prepare(ioData) |> _exec |> _finish(gl);

let createRecord = () => {
  setting: {
    textColor: [|1., 1., 1.|],
    buttonColor: [|0.35, 0.1, 0.1|],
    hoverButtonColor: [|0.40, 0.1, 0.1|],
    clickButtonColor: [|0.50, 0.1, 0.1|],
    fontTexUvForWhite: [|0., 0.|],
  },
  assetData: {
    fntId: "fnt",
    bitmapId: "bitmap",
    fntDataMap: WonderCommonlib.HashMapService.createEmpty(),
    bitmapMap: WonderCommonlib.HashMapService.createEmpty(),
    customImageArr: [||],
    customTextureMap: WonderCommonlib.HashMapService.createEmpty(),
  },
  fontData: None,
  webglData: None,
  drawDataArr: [||],
  ioData: {
    pointUp: false,
    pointDown: false,
    pointPosition: (0, 0),
    pointMovementDelta: (0, 0),
  },
  imguiFuncData: {
    apiJsObj: _buildAPIJsObj(),
    imguiFunc: None,
    customDataForIMGUIFunc: None,
  },
  buttonData: {
    lastColor: None,
  },
  needUpdateBufferData: true,
  layoutData: {
    groupData: {
      positionArr: [||],
      index: 0,
    },
  },
};