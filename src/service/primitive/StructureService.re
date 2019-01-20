let convertIntRectToFloatRect = ((x, y, width, height)) => (
  x |> NumberType.intToFloat,
  y |> NumberType.intToFloat,
  width |> NumberType.intToFloat,
  height |> NumberType.intToFloat,
);

let convertIntPositionToFloatPosition = ((x, y)) => (
  x |> NumberType.intToFloat,
  y |> NumberType.intToFloat,
);