open Js.Typed_array;

open FontType;

open IMGUIType;

let buildCanvasSize = (~canvasWidth=1000, ~canvasHeight=500, ()) => (
  canvasWidth,
  canvasHeight,
);

let buildIOData =
    (
      ~pointUp=false,
      ~pointDown=false,
      ~pointPosition=(0, 0),
      ~pointMovementDelta=(0, 0),
      (),
    ) => {
  "pointUp": pointUp,
  "pointDown": pointDown,
  "pointPosition": pointPosition,
  "pointMovementDelta": pointMovementDelta,
};

let buildLabelData = () => {
  let labelX1 = 10;
  let labelY1 = 20;
  let labelWidth1 = 100;
  let labelHeight1 = 200;
  let labelStr1 = "a";
  let labelAlign1 = Left;
  let labelColor1 = [|1., 0.5, 1.|];

  let labelX2 = 20;
  let labelY2 = 20;
  let labelWidth2 = 200;
  let labelHeight2 = 200;
  let labelStr2 = "c";
  let labelAlign2 = Left;
  let labelColor2 = [|0., 1., 1.|];

  (
    (
      (labelX1, labelY1, labelWidth1, labelHeight1),
      labelStr1,
      labelColor1,
      labelAlign1,
    ),
    (
      (labelX2, labelY2, labelWidth2, labelHeight2),
      labelStr2,
      labelColor2,
      labelAlign2,
    ),
  );
};

let buildImageData = () => {
  let customImageDataArr = AssetTool.buildCustomImageDataArr();
  let (_, id1, _) = customImageDataArr[0];
  let (_, id2, _) = customImageDataArr[1];
  let (_, id3, _) = customImageDataArr[2];

  let imageX1 = 50;
  let imageY1 = 60;
  let imageWidth1 = 150;
  let imageHeight1 = 250;
  let imageS01 = 0.1;
  let imageT01 = 0.5;
  let imageS11 = 1.;
  let imageT11 = 0.8;

  let imageX2 = 51;
  let imageY2 = 61;
  let imageWidth2 = 251;
  let imageHeight2 = 251;
  let imageS02 = 0.3;
  let imageT02 = 0.6;
  let imageS12 = 0.9;
  let imageT12 = 0.7;

  let imageX3 = 52;
  let imageY3 = 62;
  let imageWidth3 = 352;
  let imageHeight3 = 352;
  let imageS03 = 0.2;
  let imageT03 = 0.3;
  let imageS13 = 0.6;
  let imageT13 = 0.5;

  (
    (
      (imageX1, imageY1, imageWidth1, imageHeight1),
      (imageS01, imageT01, imageS11, imageT11),
      id1,
    ),
    (
      (imageX2, imageY2, imageWidth2, imageHeight2),
      (imageS02, imageT02, imageS12, imageT12),
      id2,
    ),
    (
      (imageX3, imageY3, imageWidth3, imageHeight3),
      (imageS03, imageT03, imageS13, imageT13),
      id3,
    ),
  );
};

/* let drawOneLabel = record => {
     let labelX = 10;
     let labelY = 20;
     let labelWidth = 100;
     let labelHeight = 200;
     let labelStr = "ab";

     let record =
       record
       |> FixedLayoutControlIMGUIAPI.label(
            (labelX, labelY, labelWidth, labelHeight),
            labelStr,
          );

     (record, (labelX, labelY, labelWidth, labelHeight), labelStr);
   }; */

/* let drawOneImage = record => {
     let imageX = 50;
     let imageY = 60;
     let imageWidth = 150;
     let imageHeight = 250;
     let imageS0 = 0.1;
     let imageT0 = 0.5;
     let imageS1 = 1.;
     let imageT1 = 0.8;

     let record =
       record
       |> FixedLayoutControlIMGUIAPI.image(
            (imageX, imageY, imageWidth, imageHeight),
            (imageS0, imageT0, imageS1, imageT1),
          );

     (
       record,
       (imageX, imageY, imageWidth, imageHeight),
       (imageS0, imageT0, imageS1, imageT1),
     );
   }; */

let buildNoVAOExtension = sandbox => {
  let getExtension = Sinon.createEmptyStubWithJsObjSandbox(sandbox);
  getExtension |> Sinon.returns(Js.Nullable.null);
  getExtension;
};

let createCreateGlTextureStub = sandbox => {
  open Sinon;

  let fontTexture = Obj.magic(21);
  let customTexture1 = Obj.magic(22);
  let customTexture2 = Obj.magic(23);
  let customTexture3 = Obj.magic(24);
  let createTexture = createEmptyStubWithJsObjSandbox(sandbox);
  createTexture |> onCall(0) |> returns(fontTexture);
  createTexture |> onCall(1) |> returns(customTexture1);
  createTexture |> onCall(2) |> returns(customTexture2);
  createTexture |> onCall(3) |> returns(customTexture3);

  (
    (fontTexture, customTexture1, customTexture2, customTexture3),
    createTexture,
  );
};

let testBufferDataWithIODataAndAfterInitFuncAndSetExecFuncData =
    (
      sandbox,
      bufferDataIndex,
      ioData,
      record,
      (setExecFuncData, handleAfterInitFunc),
      targetBufferDataArr,
    ) => {
  open Wonder_jest;
  open Expect;
  open Expect.Operators;
  open Sinon;

  let getExtension = buildNoVAOExtension(sandbox);

  let array_buffer = 1;
  let dynamic_draw = 2;
  let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
  let (
    (fontTexture, customTexture1, customTexture2, customTexture3),
    createTexture,
  ) =
    createCreateGlTextureStub(sandbox);
  let gl =
    FakeGlTool.buildFakeGl(
      ~sandbox,
      ~getExtension,
      ~array_buffer,
      ~bufferData,
      ~dynamic_draw,
      ~createTexture,
      (),
    )
    |> Obj.magic;
  let canvasWidth = 1000;
  let canvasHeight = 500;
  /* let record = ExecIMGUITool.addExecFuncData(~record, ~func=execFuncData, ()); */
  let record = setExecFuncData(record);

  let record = ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

  let record = record |> handleAfterInitFunc;

  let bufferDataCallCountAfterInit = bufferData |> getCallCount;
  let record = ManageIMGUIAPI.render(gl, ioData, record);

  bufferData
  |> getCall(bufferDataCallCountAfterInit + bufferDataIndex)
  |> expect
  |> toCalledWith([|
       array_buffer,
       Float32Array.make(targetBufferDataArr) |> Obj.magic,
       dynamic_draw,
     |]);
};

let testBufferDataWithIODataAndAfterInitFunc =
    (
      sandbox,
      bufferDataIndex,
      ioData,
      record,
      (execFuncData, handleAfterInitFunc),
      targetBufferDataArr,
    ) =>
  testBufferDataWithIODataAndAfterInitFuncAndSetExecFuncData(
    sandbox,
    bufferDataIndex,
    ioData,
    record,
    (
      record => ExecIMGUITool.addExecFuncData(~record, ~func=execFuncData, ()),
      handleAfterInitFunc,
    ),
    targetBufferDataArr,
  );

let testBufferDataWithIOData =
    (sandbox, bufferDataIndex, ioData, record, execFuncData, targetBufferDataArr) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    bufferDataIndex,
    ioData,
    record,
    (execFuncData, record => record),
    targetBufferDataArr,
  );

let testCustomTextureProgramColorBufferDataWithIODataAndAfterInitFunc =
    (ioData, sandbox, record, (execFuncData, afterInitFunc), targetBufferDataArr) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    1,
    ioData,
    record,
    (execFuncData, afterInitFunc),
    targetBufferDataArr,
  );

let testNoTextureProgramColorBufferDataWithIODataAndAfterInitFunc =
    (ioData, sandbox, record, (execFuncData, afterInitFunc), targetBufferDataArr) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    9,
    ioData,
    record,
    (execFuncData, afterInitFunc),
    targetBufferDataArr,
  );

let testCustomTextureProgramColorBufferDataWithIOData =
    (ioData, sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferDataWithIOData(
    sandbox,
    1,
    ioData,
    record,
    execFuncData,
    targetBufferDataArr,
  );

let testNoTextureProgramColorBufferDataWithIOData =
    (ioData, sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferDataWithIOData(
    sandbox,
    9,
    ioData,
    record,
    execFuncData,
    targetBufferDataArr,
  );

let testCustomTextureProgramPositionBufferDataWithIODataAndAfterInitFunc =
    (
      ioData,
      sandbox,
      record,
      (execFuncData, handleAfterInitFunc),
      targetBufferDataArr,
    ) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    0,
    ioData,
    record,
    (execFuncData, handleAfterInitFunc),
    targetBufferDataArr,
  );

let testFontTextureProgramPositionBufferDataWithIODataAndAfterInitFunc =
    (
      ioData,
      sandbox,
      record,
      (execFuncData, handleAfterInitFunc),
      targetBufferDataArr,
    ) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    4,
    ioData,
    record,
    (execFuncData, handleAfterInitFunc),
    targetBufferDataArr,
  );

let testCustomTextureProgramTexCoordBufferDataAndAfterInitFunc =
    (sandbox, record, (execFuncData, handleAfterInitFunc), targetBufferDataArr) =>
  testBufferDataWithIODataAndAfterInitFunc(
    sandbox,
    2,
    buildIOData(),
    record,
    (execFuncData, handleAfterInitFunc),
    targetBufferDataArr,
  );

let testBufferData =
    (sandbox, bufferDataIndex, record, execFuncData, targetBufferDataArr) =>
  testBufferDataWithIOData(
    sandbox,
    bufferDataIndex,
    buildIOData(),
    record,
    execFuncData,
    targetBufferDataArr,
  );

let testCustomTextureProgramPositionBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 0, record, execFuncData, targetBufferDataArr);

let testFontTextureProgramPositionBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 4, record, execFuncData, targetBufferDataArr);

let testNoTextureProgramPositionBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 8, record, execFuncData, targetBufferDataArr);

let testCustomTextureProgramColorBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 1, record, execFuncData, targetBufferDataArr);

let testFontTextureProgramColorBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 5, record, execFuncData, targetBufferDataArr);

let testNoTextureProgramColorBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 9, record, execFuncData, targetBufferDataArr);

let testCustomTextureProgramTexCoordBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 2, record, execFuncData, targetBufferDataArr);

let testFontTextureProgramTexCoordBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testBufferData(sandbox, 6, record, execFuncData, targetBufferDataArr);

let _testIndexBufferDataAndAfterInitFunc =
    (
      sandbox,
      bufferDataIndex,
      record,
      (execFuncData, afterInitFunc),
      targetBufferDataArr,
    ) => {
  open Wonder_jest;
  open Expect;
  open Expect.Operators;
  open Sinon;

  let getExtension = buildNoVAOExtension(sandbox);

  let element_array_buffer = 1;
  let dynamic_draw = 2;
  let bufferData = createEmptyStubWithJsObjSandbox(sandbox);
  let gl =
    FakeGlTool.buildFakeGl(
      ~sandbox,
      ~getExtension,
      ~element_array_buffer,
      ~bufferData,
      ~dynamic_draw,
      (),
    )
    |> Obj.magic;
  let canvasWidth = 1000;
  let canvasHeight = 500;
  let record = ExecIMGUITool.addExecFuncData(~record, ~func=execFuncData, ());

  let record = ManageIMGUIAPI.init(gl, (canvasWidth, canvasHeight), record);

  let record = record |> afterInitFunc;

  let bufferDataCallCountAfterInit = bufferData |> getCallCount;
  let record = ManageIMGUIAPI.render(gl, buildIOData(), record);

  bufferData
  |> getCall(bufferDataCallCountAfterInit + bufferDataIndex)
  |> expect
  |> toCalledWith([|
       element_array_buffer,
       Uint16Array.make(targetBufferDataArr) |> Obj.magic,
       dynamic_draw,
     |]);
};

let testCustomTextureProgramIndexBufferDataAndAfterInitFunc =
    (sandbox, record, (execFuncData, afterInitFunc), targetBufferDataArr) =>
  _testIndexBufferDataAndAfterInitFunc(
    sandbox,
    3,
    record,
    (execFuncData, afterInitFunc),
    targetBufferDataArr,
  );

let testFontTextureProgramIndexBufferDataAndAfterInitFunc =
    (sandbox, record, (execFuncData, afterInitFunc), targetBufferDataArr) =>
  _testIndexBufferDataAndAfterInitFunc(
    sandbox,
    7,
    record,
    (execFuncData, afterInitFunc),
    targetBufferDataArr,
  );

let testCustomTextureProgramIndexBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testCustomTextureProgramIndexBufferDataAndAfterInitFunc(
    sandbox,
    record,
    (execFuncData, record => record),
    targetBufferDataArr,
  );

let testFontTextureProgramIndexBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testFontTextureProgramIndexBufferDataAndAfterInitFunc(
    sandbox,
    record,
    (execFuncData, record => record),
    targetBufferDataArr,
  );

let testNoTextureProgramIndexBufferDataAndAfterInitFunc =
    (sandbox, record, (execFuncData, afterInitFunc), targetBufferDataArr) =>
  _testIndexBufferDataAndAfterInitFunc(
    sandbox,
    10,
    record,
    (execFuncData, afterInitFunc),
    targetBufferDataArr,
  );

let testNoTextureProgramIndexBufferData =
    (sandbox, record, execFuncData, targetBufferDataArr) =>
  testNoTextureProgramIndexBufferDataAndAfterInitFunc(
    sandbox,
    record,
    (execFuncData, record => record),
    targetBufferDataArr,
  );

let prepareFntData = record => {
  open IMGUIType;

  let fntFilePath =
    Node.Path.join([|Node.Process.cwd(), "./test/res/font/myFont.fnt"|]);

  let fntData =
    IOIMGUITool.parseFnt(Node.Fs.readFileAsUtf8Sync(fntFilePath));

  {
    ...record,
    assetData: {
      ...record.assetData,
      fntDataMap:
        record.assetData.fntDataMap
        |> WonderCommonlib.MutableHashMapService.set(
             record.assetData.fntId,
             fntData,
           ),
    },
  };
};

let buildCustomData = () => Obj.magic(501);

let getBufferDataCallCountPerRender = () => 4;

let getDrawData = record => record.drawData;