let buildEmptyExecFuncData = () => (. _, _, record) => record;

let addExecFuncData =
    (
      ~record,
      ~name="exec",
      ~customData=Obj.magic(-1),
      ~zIndex=0,
      ~func=buildEmptyExecFuncData(),
      (),
    ) =>
  record |> ManageIMGUIAPI.addExecFuncData(name, customData, zIndex, func);