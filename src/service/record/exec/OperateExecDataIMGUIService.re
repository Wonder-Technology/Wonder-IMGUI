open IMGUIType;

let _clearData = record => {
  ...record,
  layoutData: {
    groupData: {
      positionArr: [||],
      index: 0,
    },
  },
};

let getExecFuncDataArr = record => record.execData.execFuncDataArr;

let hasExecFuncData = (execFuncName, record) =>
  getExecFuncDataArr(record)
  |> Js.Array.find(({name}) => name === execFuncName)
  |> Js.Option.isSome;

let addExecFuncData = (execFuncName, customData, execOrder, func, record) =>
  {
    ...record,
    execData: {
      ...record.execData,
      execFuncDataArr:
        getExecFuncDataArr(record)
        |> ArrayService.push({
             execFunc: func,
             customData,
             execOrder,
             name: execFuncName,
           })
        |> Js.Array.sortInPlaceWith((execFuncData1, execFuncData2) =>
             execFuncData1.execOrder - execFuncData2.execOrder
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

let getCustomData = (execFuncName, {execData}) =>
  execData.execFuncDataArr
  |> Js.Array.find(({name}: IMGUIType.execFuncData) => name == execFuncName)
  |> Js.Option.map((. {customData}: IMGUIType.execFuncData) => customData);

let unsafeGetCustomData = (execFuncName, record) =>
  getCustomData(execFuncName, record) |> OptionService.unsafeGet;

let getExecFunc = (execFuncName, {execData}) =>
  execData.execFuncDataArr
  |> Js.Array.find(({name}: IMGUIType.execFuncData) => name == execFuncName)
  |> Js.Option.map((. {execFunc}: IMGUIType.execFuncData) => execFunc);

let unsafeGetExecFunc = (execFuncName, record) =>
  getExecFunc(execFuncName, record) |> OptionService.unsafeGet;

let getExecOrder = (execFuncName, {execData}) =>
  execData.execFuncDataArr
  |> Js.Array.find(({name}: IMGUIType.execFuncData) => name == execFuncName)
  |> Js.Option.map((. {execOrder}: IMGUIType.execFuncData) => execOrder);

let unsafeGetExecOrder = (execFuncName, record) =>
  getExecOrder(execFuncName, record) |> OptionService.unsafeGet;