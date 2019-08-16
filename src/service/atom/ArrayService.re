let range = (a: int, b: int) => {
  let result = WonderCommonlib.ArrayService.createEmpty();
  for (i in a to b) {
    Js.Array.push(i, result) |> ignore;
  };
  result;
};

let push = (item, arr) => {
  arr |> Js.Array.push(item) |> ignore;
  arr;
};

let getLast = arr =>
  Array.unsafe_get(arr, Js.Array.length(arr) - 1) |> Obj.magic |> Js.toOption;

let unsafeGetFirst = arr => Array.unsafe_get(arr, 0);

let chunk = (size, arr) => {
  let (resultArr, chunkArr, currentSize) =
    arr
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (. (resultArr, chunkArr, currentSize), value) =>
           currentSize < size ?
             (resultArr, chunkArr |> push(value), currentSize |> succ) :
             (resultArr |> push(chunkArr), [|value|], 1),
         ([||], [||], 0),
       );

  /* WonderLog.Log.print(( chunkArr, currentSize )) |> ignore; */

  resultArr |> push(chunkArr);
};