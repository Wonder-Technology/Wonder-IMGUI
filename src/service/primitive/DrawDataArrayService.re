open DrawDataType;

let addPoints = (points, pointArr) =>
  points
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. arr, point) => arr |> ArrayService.push(point),
       pointArr,
     );


let concatArrays = [%raw arrays => {|
  return [].concat.apply([], arrays);  
  |}];


let getBaseIndex = verticeArr => (verticeArr |> Js.Array.length) / 2;