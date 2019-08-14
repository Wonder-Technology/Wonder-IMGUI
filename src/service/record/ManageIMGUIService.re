open DrawDataType;

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

let _sendUniformProjectionMatData = (gl, program, canvasSize) =>
  uniformMatrix4fv(
    gl |> getUniformLocation(program, "u_projectionMat"),
    false,
    _buildOrthoProjectionMat4TypeArr(canvasSize),
    gl,
  );

let _getProgram = record => {
  let {program} = record.webglData |> OptionService.unsafeGet;

  program;
};

let _sendUniformData = (gl, program, canvasSize) => {
  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize);

  uniform1i(gl |> getUniformLocation(program, "u_sampler2D"), 0, gl);

  ();
};

let sendUniformProjectionMatData = (gl, canvasSize, record) => {
  let program = _getProgram(record);

  useProgram(program, gl);

  uniformMatrix4fv(
    gl |> getUniformLocation(program, "u_projectionMat"),
    false,
    _buildOrthoProjectionMat4TypeArr(canvasSize),
    gl,
  );

  record;
};

let init = (gl, canvasSize, record) =>
  !AssetIMGUIService.isLoadAsset(record) ?
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

      let record =
        record
        |> ManageSkinIMGUIService.clearAllSkins
        |> DataSkinIMGUIService.addDefaultSkinData
        |> ExtendIMGUIAPI.registerCustomControl(
             ExtendButton.CustomControl.getCustomControlName(),
             ExtendButton.CustomControl.buildCustomControl(),
           );

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
          }),
      };
    };

let _createEmptyDrawData = () => {
  fontTextureDrawData: {
    drawType: DrawDataType.FontTexture,
    customTexture: None,
    verticeArr: [||],
    colorArr: [||],
    texCoordArr: [||],
    indexArr: [||],
  },
  customTextureDrawDataMap:
    WonderCommonlib.MutableHashMapService.createEmpty(),
};

let _prepare = (ioData, (getRecordFunc, setRecordFunc), data) => {
  let record = getRecordFunc(data);

  {
    ...record,
    ioData,
    /* controlData: {
         ...record.controlData,
         sliderData: {
           ...record.controlData.sliderData,
           index: 0,
         },
         checkboxData: {
           ...record.controlData.checkboxData,
           index: 0,
         },
       }, */
    drawData: _createEmptyDrawData(),
  }
  ->(setRecordFunc(data));
};

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
               ~title="_draw",
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

let _buildGroupedDrawDataArr = record => {
  let fontTextureDrawData = RecordIMGUIService.getFontTextureDrawData(record);
  let customTextureDrawDataMap =
    RecordIMGUIService.getCustomTextureDrawDataMap(record);

  let (_, baseIndex, customTextureDrawDataArr) =
    customTextureDrawDataMap
    |> WonderCommonlib.MutableHashMapService.getValidValues
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (.
           (lastVerticeArr, baseIndex, resultDrawDataArr),
           ({verticeArr, indexArr}: DrawDataType.drawData) as drawData,
         ) => {
           let baseIndex =
             DrawDataArrayService.getBaseIndex(lastVerticeArr) + baseIndex;

           (
             verticeArr,
             baseIndex,
             resultDrawDataArr
             |> ArrayService.push({
                  ...drawData,
                  indexArr:
                    indexArr |> Js.Array.map(index => index + baseIndex),
                }),
           );
         },
         ([||], 0, [||]),
       );

  let newBaseIndex =
    switch (ArrayService.getLast(customTextureDrawDataArr)) {
    | None => baseIndex
    | Some(({verticeArr}: DrawDataType.drawData)) =>
      DrawDataArrayService.getBaseIndex(verticeArr) + baseIndex
    };

  let fontTextureDrawData = {
    ...fontTextureDrawData,
    indexArr:
      fontTextureDrawData.indexArr
      |> Js.Array.map(index => index + newBaseIndex),
  };

  (
    record,
    DrawDataArrayService.concatArrays([|
      customTextureDrawDataArr,
      [|fontTextureDrawData|],
    |]),
  );
};

let _finish = (gl, (getRecordFunc, setRecordFunc), data) => {
  let record = getRecordFunc(data);

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
  let record = _backupGlState(gl, record);

  let (record, groupedDrawDataArr) = record |> _buildGroupedDrawDataArr;

  let (record, drawElementsDataArr) =
    BufferDataIMGUIService.bufferAllData(gl, groupedDrawDataArr, record);

  let {program} = RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(program, gl);

  _setGlState(gl);

  (record |> _draw(gl, drawElementsDataArr) |> _restoreGlState(gl))
  ->(setRecordFunc(data));
};

let getCustomData = ({imguiFuncData}) =>
  imguiFuncData.customDataForIMGUIFunc;

let _clearData = record => {
  ...record,
  /* controlData: {
       ...record.controlData,
       radioButtonData: {
         isSelectedMap: WonderCommonlib.MutableHashMapService.createEmpty(),
       },
       checkboxData: {
         index: 0,
         isSelectedMap: WonderCommonlib.MutableSparseMapService.createEmpty(),
       },
       sliderData: {
         index: 0,
         valueMap: WonderCommonlib.MutableSparseMapService.createEmpty(),
       },
     }, */
  layoutData: {
    groupData: {
      positionArr: [||],
      index: 0,
    },
  },
};

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
  |> _clearData;

let getAPIJsObj = ({imguiFuncData}) => imguiFuncData.apiJsObj;

let _buildAPIJsObj = () => {
  "label": FixedLayoutControlIMGUIService.label,
  "image": FixedLayoutControlIMGUIService.image,
  "box": FixedLayoutControlIMGUIService.box,
  "button": ExtendButton.CustomControl.button,
  "beginGroup": GroupLayoutIMGUIService.beginGroup,
  "endGroup": GroupLayoutIMGUIService.endGroup,
  "unsafeGetCustomControl": ManageCustomControlIMGUIService.unsafeGetCustomControl,
};

let _exec = (apiJsObj, getRecordFunc, data) => {
  let {imguiFuncData} as record = getRecordFunc(data);

  switch (getIMGUIFunc(record)) {
  | None => data
  | Some(func) =>
    func(.
      imguiFuncData.customDataForIMGUIFunc |> OptionService.unsafeGet,
      /* getAPIJsObj(record), */
      apiJsObj,
      /* record, */
      data,
    )
  };
};

let render = (gl, ioData, apiJsObj, (getRecordFunc, setRecordFunc), data) => {
  let record = getRecordFunc(data);

  !AssetIMGUIService.isLoadAsset(record) ?
    data :
    data
    |> _prepare(ioData, (getRecordFunc, setRecordFunc))
    |> _exec(apiJsObj, getRecordFunc)
    |> _finish(gl, (getRecordFunc, setRecordFunc));
};

let createRecord = () => {
  setting:
    /* buttonSetting: {
         buttonColor: [|0.35, 0.1, 0.1|],
         hoverButtonColor: [|0.40, 0.1, 0.1|],
         clickButtonColor: [|0.50, 0.1, 0.1|],
       },
       radioButtonSetting: {
         radioButtonOuterColor: [|0.3, 0.3, 0.3|],
         radioButtonInnerColor: [|0.15, 0.15, 0.15|],
         radioButtonOuterColorHover: [|0.33, 0.33, 0.33|],
         radioButtonInnerColorHover: [|0.18, 0.18, 0.18|],
         radioButtonCircleSegments: 9,
         radioButtonInnerRadius: 0.6,
         radioButtonOuterRadius: 1.,
       },
       checkboxSetting: {
         checkboxOuterColor: [|0.3, 0.3, 0.3|],
         checkboxInnerColor: [|0.15, 0.15, 0.15|],
         checkboxOuterColorHover: [|0.33, 0.33, 0.33|],
         checkboxInnerColorHover: [|0.18, 0.18, 0.18|],
         checkboxInnerSizeRatio: 1.4,
         checkboxOuterSizeRatio: 2.,
       },
       sliderSetting: {
         sliderBackgroundColor: [|0.16, 0.16, 0.16|],
         sliderFillColor: [|0., 0.3, 0.6|],
         sliderBackgroundColorHover: [|0.19, 0.19, 0.19|],
         sliderFillColorHover: [|0., 0.3, 0.7|],
       }, */
    {
      textColor: [|1., 1., 1.|],
      fontTexUvForWhite: [|0., 0.|],
    },
  assetData: {
    fntId: "fnt",
    bitmapId: "bitmap",
    fntDataMap: WonderCommonlib.MutableHashMapService.createEmpty(),
    bitmapMap: WonderCommonlib.MutableHashMapService.createEmpty(),
    customImageArr: [||],
    customTextureMap: WonderCommonlib.MutableHashMapService.createEmpty(),
  },
  fontData: None,
  webglData: None,
  drawData: _createEmptyDrawData(),
  /* controlData: {
       radioButtonData: {
         isSelectedMap: WonderCommonlib.MutableHashMapService.createEmpty(),
       },
       checkboxData: {
         index: 0,
         isSelectedMap: WonderCommonlib.MutableSparseMapService.createEmpty(),
       },
       sliderData: {
         index: 0,
         valueMap: WonderCommonlib.MutableSparseMapService.createEmpty(),
       },
     }, */
  ioData: {
    pointUp: false,
    pointDown: false,
    pointPosition: (0, 0),
    pointMovementDelta: (0, 0),
  },
  imguiFuncData: {
    apiJsObj: _buildAPIJsObj() |> Obj.magic,
    imguiFunc: None,
    customDataForIMGUIFunc: None,
  },
  layoutData: {
    groupData: {
      positionArr: [||],
      index: 0,
    },
  },
  extendData: {
    customControlData: {
      apiJsObj: ExtendAPIJsObjIMGUIService.createAPIJsObj() |> Obj.magic,
      funcMap: WonderCommonlib.ImmutableHashMapService.createEmpty(),
    },
    skinData: {
      allSkinDataMap: WonderCommonlib.ImmutableHashMapService.createEmpty(),
    },
  },
};