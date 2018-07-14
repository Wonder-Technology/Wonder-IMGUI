open DrawDataArrType;

let addPoints = (points, pointArr) => pointArr |> Js.Array.concat(points);

let getBaseIndex = verticeArr => (verticeArr |> Js.Array.length) / 2;