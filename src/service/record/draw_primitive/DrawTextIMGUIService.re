open IMGUIType;

open DrawDataArrType;

let _getCharDesc = (char, fftData) =>
  Array.unsafe_get(
    fftData |> WonderCommonlib.HashMap.unsafeGet("chars"),
    char |> Js.String.charCodeAt(0) - 32,
  );

let _getTextSizes = (str, {fftData} as record) => {
  let {textScale} = ManageIMGUIService.unsafeGetSetting(record);

  ArrayService.range(0, str |> Js.String.length - 1)
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. (width, height), char) => {
         let charData = _getCharDesc(char, fftData);
         let h = charData##yoff2 *. textScale -. charData##yoff *. textScale;

         (width + charData##xadvance *. textScale, height <. h ? h : height);
       },
       (0., 0.),
     );
};

/* TODO support draw multi line text */
let draw = ((x, y, _width, _height), str, {drawDataArr} as record) => {
  let {textScale, textColor} = ManageIMGUIService.unsafeGetSetting(record);
  /*
   width of a single pixel in the font atlas.
   */
  let ipw = 1. /. 256.;
  let iph = 1. /. 256.;

  let (verticeArr, colorArr, texCoordArr, indexArr, x, y) =
    ArrayService.range(0, str |> Js.String.length - 1)
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (. (verticeArr, colorArr, texCoordArr, indexArr, x, y), char) => {
           let charData = _getCharDesc(char, fftData);

           let x0 = (x +. charData##xoff) *. textScale;
           let y0 = (y +. charData##yoff) *. textScale;
           let x1 = (x +. charData##xoff2) *. textScale;
           let y1 = (y +. charData##yoff2) *. textScale;

           let s0 = charData##x0 *. ipw;
           let t0 = charData##y0 *. iph;
           let s1 = charData##x1 *. ipw;
           let t1 = charData##y1 *. iph;

           let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

           (
             verticeArr
             |> DrawDataArrayService.addPoints([|
                  x0,
                  y0,
                  x0,
                  y1,
                  x1,
                  y0,
                  x1,
                  y1,
                |]),
             colorArr |> DrawDataArrayService.addPoints(textColor),
             texCoordArr
             |> DrawDataArrayService.addPoints([|
                  s0,
                  t0,
                  s0,
                  t1,
                  s1,
                  t0,
                  s1,
                  t1,
                |]),
             indexArr
             |> DrawDataArrayService.addPoints([|
                  baseIndex,
                  baseIndex + 1,
                  baseIndex + 2,
                  baseIndex + 3,
                  baseIndex + 2,
                  baseIndex + 1,
                |]),
             x + charData##xadvance *. textScale |> Js.Math.unsafe_round,
             y,
           );
         },
         ([||], [||], [||], [||], x, y),
       );

  {
    ...record,
    drawDataArr:
      drawDataArr
      |> ArrayService.push({
           drawType: FontTexture,
           verticeArr,
           colorArr,
           texCoordArr,
           indexArr,
         }),
  };
};