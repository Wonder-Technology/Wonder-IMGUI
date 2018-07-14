type webgl1Context;

type program;

type shader;

type buffer;

type texture;

type textureSource;

type attributeLocation = int;

type uniformLocation;

type enum;

type precisionFormat = {. "precision": int};

type extension;

type vaoExtension;

external imageElementToTextureSource :
  DomExtendType.imageElement => textureSource =
  "%identity";

external parameterIntToNullableProgram : int => Js.Nullable.t(program) =
  "%identity";

external parameterIntToBuffer : int => buffer = "%identity";

external parameterIntToNullableTexture : int => Js.Nullable.t(texture) =
  "%identity";