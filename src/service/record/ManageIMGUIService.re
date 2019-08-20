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

let _getCustomTextureProgram = record => {
  let {customTextureShaderData} = record.webglData |> OptionService.unsafeGet;

  customTextureShaderData.program;
};

let _getFontTextureProgram = record => {
  let {fontTextureShaderData} = record.webglData |> OptionService.unsafeGet;

  fontTextureShaderData.program;
};

let _getNoTextureProgram = record => {
  let {noTextureShaderData} = record.webglData |> OptionService.unsafeGet;

  noTextureShaderData.program;
};

let _sendTextureProgramUniformData = (gl, program, canvasSize) => {
  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize);

  uniform1i(gl |> getUniformLocation(program, "u_sampler2D"), 0, gl);

  ();
};

let _sendNoTextureProgramUniformData = (gl, program, canvasSize) => {
  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize);

  ();
};

let sendCustomTextureProgramUniformProjectionMatData =
    (gl, canvasSize, record) => {
  let program = _getCustomTextureProgram(record);

  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize) |> ignore;

  record;
};

let sendFontTextureProgramUniformProjectionMatData = (gl, canvasSize, record) => {
  let program = _getFontTextureProgram(record);

  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize) |> ignore;

  record;
};

let sendNoTextureProgramUniformProjectionMatData = (gl, canvasSize, record) => {
  let program = _getNoTextureProgram(record);

  useProgram(program, gl);

  _sendUniformProjectionMatData(gl, program, canvasSize) |> ignore;

  record;
};

let init = (gl, canvasSize, record) =>
  !AssetIMGUIService.LoadAsset.isLoadAsset(record) ?
    record :
    {
      let customTextureProgram =
        gl
        |> createProgram
        |> WonderWebgl.ProgramService.initShader(
             ShaderData.vs_customTexture,
             ShaderData.fs_customTexture,
             gl,
           );
      let fontTextureProgram =
        gl
        |> createProgram
        |> WonderWebgl.ProgramService.initShader(
             ShaderData.vs_fontTexture,
             ShaderData.fs_fontTexture,
             gl,
           );
      let noTextureProgram =
        gl
        |> createProgram
        |> WonderWebgl.ProgramService.initShader(
             ShaderData.vs_noTexture,
             ShaderData.fs_noTexture,
             gl,
           );

      let fontTexture =
        _createFontTexture(
          gl,
          AssetIMGUIService.unsafeGetBitmap(record)
          |> WonderWebgl.GlType.imageElementToTextureSource,
        );

      _sendTextureProgramUniformData(gl, customTextureProgram, canvasSize);
      _sendTextureProgramUniformData(gl, fontTextureProgram, canvasSize);
      _sendNoTextureProgramUniformData(gl, noTextureProgram, canvasSize);

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
            customTextureShaderData: {
              program: customTextureProgram,
              aPositonLocation:
                gl |> getAttribLocation(customTextureProgram, "a_position"),
              aColorLocation:
                gl |> getAttribLocation(customTextureProgram, "a_color"),
              aTexCoordLocation:
                gl |> getAttribLocation(customTextureProgram, "a_texCoord"),
              positionBuffer: _createArrayBuffer(gl),
              colorBuffer: _createArrayBuffer(gl),
              texCoordBuffer: _createArrayBuffer(gl),
              indexBuffer: _createElementArrayBuffer(gl),
            },
            fontTextureShaderData: {
              program: fontTextureProgram,
              aPositonLocation:
                gl |> getAttribLocation(fontTextureProgram, "a_position"),
              aColorLocation:
                gl |> getAttribLocation(fontTextureProgram, "a_color"),
              aTexCoordLocation:
                gl |> getAttribLocation(fontTextureProgram, "a_texCoord"),
              positionBuffer: _createArrayBuffer(gl),
              colorBuffer: _createArrayBuffer(gl),
              texCoordBuffer: _createArrayBuffer(gl),
              indexBuffer: _createElementArrayBuffer(gl),
            },
            noTextureShaderData: {
              program: noTextureProgram,
              aPositonLocation:
                gl |> getAttribLocation(noTextureProgram, "a_position"),
              aColorLocation:
                gl |> getAttribLocation(noTextureProgram, "a_color"),
              positionBuffer: _createArrayBuffer(gl),
              colorBuffer: _createArrayBuffer(gl),
              indexBuffer: _createElementArrayBuffer(gl),
            },
            fontTexture,
            lastWebglData: None,
          }),
      };
    };

let _createEmptyDrawData = () => {
  noTextureDrawData: {
    verticeArr: [||],
    colorArr: [||],
    indexArr: [||],
  },
  customTextureDrawData: {
    customTexture: None,
    verticeArr: [||],
    colorArr: [||],
    texCoordArr: [||],
    indexArr: [||],
  },
  fontTextureDrawData: {
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

let _drawElements = (count, countOffset, gl) =>
  count === 0 ?
    gl :
    {
      drawElements(
        getTriangles(gl),
        count,
        getUnsignedShort(gl),
        countOffset,
        gl,
      );

      gl;
    };

let _drawCustomTexture = (drawElementsDataArr, record, gl) => {
  drawElementsDataArr
  |> WonderCommonlib.ArrayService.forEach(
       (. {customTexture, count, countOffset}: customTextureDrawElementsData) => {
       let texture = customTexture |> OptionService.unsafeGet;

       bindTexture(getTexture2D(gl), texture, gl);

       _drawElements(count, countOffset, gl) |> ignore;
     });

  gl;
};

let _drawFontTexture =
    (drawElementData: DrawDataType.fontTextureDrawElementsData, record, gl) => {
  let {fontTexture} = RecordIMGUIService.unsafeGetWebglData(record);

  bindTexture(getTexture2D(gl), fontTexture, gl);

  _drawElements(drawElementData.count, 0, gl);
};

let _drawNoTexture = (drawElementData, gl) =>
  _drawElements(drawElementData.count, 0, gl);

let _restoreGlState = (record, gl) => {
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

let _buildAllDrawData = record => {
  let fontTextureDrawData = RecordIMGUIService.getFontTextureDrawData(record);
  let customTextureDrawDataMap =
    RecordIMGUIService.getCustomTextureDrawDataMap(record);
  let noTextureDrawData = RecordIMGUIService.getNoTextureDrawData(record);

  let (_, baseIndex, customTextureDrawDataArr) =
    customTextureDrawDataMap
    |> WonderCommonlib.MutableHashMapService.getValidValues
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (.
           (lastVerticeArr, baseIndex, resultDrawDataArr),
           ({verticeArr, indexArr}: DrawDataType.customTextureDrawData) as drawData,
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

  /* let newBaseIndex =
       switch (ArrayService.getLast(customTextureDrawDataArr)) {
       | None => baseIndex
       | Some(({verticeArr}: DrawDataType.customTextureDrawData)) =>
         DrawDataArrayService.getBaseIndex(verticeArr) + baseIndex
       };

     let fontTextureDrawData = {
       ...fontTextureDrawData,
       indexArr:
         fontTextureDrawData.indexArr
         |> Js.Array.map(index => index + newBaseIndex),
     };

     let newBaseIndex =
       DrawDataArrayService.getBaseIndex(fontTextureDrawData.verticeArr)
       + newBaseIndex;

     let noTextureDrawData = {
       ...noTextureDrawData,
       indexArr:
         noTextureDrawData.indexArr |> Js.Array.map(index => index + newBaseIndex),
     }; */

  (record, customTextureDrawDataArr, fontTextureDrawData, noTextureDrawData);
};

let _renderCustomTextures = (customTextureDrawDataArr, record, gl) => {
  let {customTextureShaderData} =
    RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(customTextureShaderData.program, gl);

  let (record, drawElementsDataArr) =
    BufferDataIMGUIService.bufferCustomTextureDataAndSend(
      gl,
      customTextureDrawDataArr,
      record,
    );

  gl |> _drawCustomTexture(drawElementsDataArr, record);
};

let _renderFontTexture = (fontTextureDrawData, record, gl) => {
  let {fontTextureShaderData} =
    RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(fontTextureShaderData.program, gl);

  let (record, fontTextureDrawElementData) =
    BufferDataIMGUIService.bufferFontTextureDataAndSend(
      gl,
      fontTextureDrawData,
      record,
    );

  gl |> _drawFontTexture(fontTextureDrawElementData, record);
};

let _renderNoTexture = (noTextureDrawData, record, gl) => {
  let {noTextureShaderData} = RecordIMGUIService.unsafeGetWebglData(record);

  useProgram(noTextureShaderData.program, gl);

  let (record, noTextureDrawElementData) =
    BufferDataIMGUIService.bufferNoTextureDataAndSend(
      gl,
      noTextureDrawData,
      record,
    );

  gl |> _drawNoTexture(noTextureDrawElementData);
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

  let (
    record,
    customTextureDrawDataArr,
    fontTextureDrawData,
    noTextureDrawData,
  ) =
    record |> _buildAllDrawData;

  let {customTextureShaderData, fontTextureShaderData, noTextureShaderData} =
    RecordIMGUIService.unsafeGetWebglData(record);

  _setGlState(gl);

  gl
  |> _renderCustomTextures(customTextureDrawDataArr, record)
  |> _renderFontTexture(fontTextureDrawData, record)
  |> _renderNoTexture(noTextureDrawData, record)
  |> _restoreGlState(record)
  |> ignore;

  record->(setRecordFunc(data));
};

let getCustomData = (execFuncName, {execData}) =>
  execData.execFuncDataArr
  |> Js.Array.find(({name}: IMGUIType.execFuncData) =>
       name == execFuncName
     )
  |> Js.Option.map((. {customData}: IMGUIType.execFuncData) => customData);

let unsafeGetCustomData = (execFuncName, record) =>
  getCustomData(execFuncName, record) |> OptionService.unsafeGet;

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

let getExecFunc = (execFuncName, {execData}) =>
  execData.execFuncDataArr
  |> Js.Array.find(({name}: IMGUIType.execFuncData) =>
       name == execFuncName
     )
  |> Js.Option.map((. {execFunc}: IMGUIType.execFuncData) => execFunc);

let unsafeGetExecFunc = (execFuncName, record) =>
  getExecFunc(execFuncName, record) |> OptionService.unsafeGet;

let getExecFuncDataArr = record => record.execData.execFuncDataArr;

let addExecFuncData = (execFuncName, customData, zIndex, func, record) =>
  {
    ...record,
    execData: {
      ...record.execData,
      execFuncDataArr:
        getExecFuncDataArr(record)
        |> ArrayService.push({
             execFunc: func,
             customData,
             zIndex,
             name: execFuncName,
           })
        |> Js.Array.sortInPlaceWith((execFuncData1, execFuncData2) =>
             execFuncData1.zIndex - execFuncData2.zIndex
           ),
    },
  }
  |> _clearData;

let removeExecFuncData = (execFuncName, record) =>
  {
    ...record,
    execData: {
      ...record.execData,
      execFuncDataArr:
        getExecFuncDataArr(record)
        |> Js.Array.filter(({name}: IMGUIType.execFuncData) =>
             name !== execFuncName
           ),
    },
  }
  |> _clearData;

let clearExecFuncDataArr = record =>
  {
    ...record,
    execData: {
      ...record.execData,
      execFuncDataArr: WonderCommonlib.ArrayService.createEmpty(),
    },
  }
  |> _clearData;

let getAPIJsObj = ({execData}) => execData.apiJsObj;

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
  let {execData} as record = getRecordFunc(data);

  getExecFuncDataArr(record)
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. data, {customData, execFunc}: execFuncData) =>
         execFunc(. customData, apiJsObj, data),
       data,
     );
};

let render = (gl, ioData, apiJsObj, (getRecordFunc, setRecordFunc), data) => {
  let record = getRecordFunc(data);

  !AssetIMGUIService.LoadAsset.isLoadAsset(record) ?
    data :
    data
    |> _prepare(ioData, (getRecordFunc, setRecordFunc))
    |> _exec(apiJsObj, getRecordFunc)
    |> _finish(gl, (getRecordFunc, setRecordFunc));
};

let createRecord = () => {
  assetData: {
    fntId: "fnt",
    bitmapId: "bitmap",
    fntDataMap: WonderCommonlib.MutableHashMapService.createEmpty(),
    bitmapMap: WonderCommonlib.MutableHashMapService.createEmpty(),
    customImageArr: [||],
    customTextureMap: WonderCommonlib.MutableHashMapService.createEmpty(),
    settedAssetData: RecordAssetIMGUIService.createSettedAssetData(),
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
  execData: {
    apiJsObj: _buildAPIJsObj() |> Obj.magic,
    execFuncDataArr: WonderCommonlib.ArrayService.createEmpty(),
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