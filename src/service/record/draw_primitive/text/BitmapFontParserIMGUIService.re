open IMGUIType;

open FontType;

let getKerning = (fntData, left, right) =>
  switch (
    fntData.kerningMap
    |> WonderCommonlib.SparseMapService.get(
         ParseFntIMGUIService.buildKerningHashMapKey(left, right),
       )
  ) {
  | None => 0
  | Some(amount) => amount
  };