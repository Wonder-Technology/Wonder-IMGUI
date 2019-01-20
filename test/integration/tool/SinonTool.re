open Sinon;

let withSixArgs = (arg1, arg2, arg3, arg4, arg5, arg6, stub) =>
  stubToJsObj(stub)##withArgs(arg1, arg2, arg3, arg4, arg5, arg6);