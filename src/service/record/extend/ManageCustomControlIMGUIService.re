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
             name, (. customControlFuncInputData, imguiRecord) =>
             customControlFunc(.
               customControlFuncInputData,
               imguiRecord.extendData.customControlData.apiJsObj,
               imguiRecord,
             )
           ),
    },
  },
};

let unsafeGetCustomControl =
  (. name, record) =>
    _getFuncMap(record)
    |> WonderCommonlib.ImmutableHashMapService.unsafeGet(name);