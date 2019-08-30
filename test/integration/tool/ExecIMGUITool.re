let buildEmptyExecFuncData = () => (. _, _, record) => record;

let addExecFuncData =
    (
      ~record,
      ~name="exec",
      ~customData=Obj.magic(-1),
      ~execOrder=0,
      ~func=buildEmptyExecFuncData(),
      (),
    ) =>
  record |> ExecIMGUIAPI.addExecFuncData(name, customData, execOrder, func);