open GlType;

open IMGUIType;

open DrawDataArrType;

let draw =
    (
      (x, y, width, height),
      /* ((s0, t0), (s0, t1), (s1, t0), (s1, t1)), */
      /* (topLeftUv, bottomLeftUv, topRightUv, bottomRightUv), */
      (s0, t0, s1, t1),
      texture,
      {drawDataArr} as record,
    ) => {
  let baseIndex = 0;

  {
    ...record,
    drawDataArr:
      drawDataArr
      |> ArrayService.push({
           drawType: CustomTexture,
           customTexture: Some(texture),
           verticeArr: [|
             x,
             y,
             x,
             y + height,
             x + width,
             y,
             x + width,
             y + height,
           |],
           colorArr: [|1., 1., 1.|],
           texCoordArr: [|s0, t0, s0, t1, s1, t0, s1, t1|],
           indexArr: [|
             baseIndex,
             baseIndex + 1,
             baseIndex + 2,
             baseIndex + 3,
             baseIndex + 2,
             baseIndex + 1,
           |],
         }),
  };
};