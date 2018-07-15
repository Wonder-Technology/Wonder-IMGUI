let getValidValues = map =>
  map |> Js.Array.filter(value => value |> Obj.magic !== Js.Undefined.empty);