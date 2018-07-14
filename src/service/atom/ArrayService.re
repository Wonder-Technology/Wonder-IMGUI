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

let getLast = arr => Array.unsafe_get(arr, Js.Array.length(arr) - 1);