open IMGUIType;

let _getFuncMap = record => record.extendData.customControlData.funcMap;

let registerCustomControl = (name, customControlFunc, {extendData} as record) => {
  ...record,
  extendData: {
    ...extendData,
    customControlData: {
      ...extendData.customControlData,
      funcMap:
        _getFuncMap(record)
        |> WonderCommonlib.ImmutableHashMapService.set(
             name,
             (.
               customControlFuncInputData,
               customControlFunctionShowData,
               imguiRecord,
             ) =>
             customControlFunc(.
               customControlFuncInputData,
               customControlFunctionShowData,
               imguiRecord.extendData.customControlData.apiJsObj,
               imguiRecord,
             )
           ),
    },
  },
};

let hasCustomControl = (name, record) =>
  _getFuncMap(record) |> WonderCommonlib.ImmutableHashMapService.has(name);

let unsafeGetCustomControl =
  (. name, record) =>
    _getFuncMap(record)
    |> WonderCommonlib.ImmutableHashMapService.unsafeGet(name);