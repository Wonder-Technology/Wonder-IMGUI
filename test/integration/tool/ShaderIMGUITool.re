let getCustomTextureProgram = record =>
  ManageIMGUIService._getCustomTextureProgram(record);

let getFontTextureProgram = record =>
  ManageIMGUIService._getFontTextureProgram(record);

let getNoTextureProgram = record =>
  ManageIMGUIService._getNoTextureProgram(record);

let getCustomTextureShaderData = record =>
  RecordIMGUITool.unsafeGetWebglData(record).customTextureShaderData;

let getFontTextureShaderData = record =>
  RecordIMGUITool.unsafeGetWebglData(record).fontTextureShaderData;

let getNoTextureShaderData = record =>
  RecordIMGUITool.unsafeGetWebglData(record).noTextureShaderData;