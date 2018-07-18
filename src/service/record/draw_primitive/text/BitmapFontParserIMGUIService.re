open IMGUIType;

open FontType;

let getKerning = (fntData, left, right) =>
  fntData.kerningArray |> Js.Array.length === 0 ?
    0 :
    (
      switch (
        fntData.kerningArray
        |> WonderCommonlib.ArrayService.reduceOneParam(
             (. amount, kern) =>
               switch (amount) {
               | Some(_) => amount
               | None =>
                 kern.first === left && kern.second === right ?
                   Some(kern.amount) : None
               },
             None,
           )
      ) {
      | None => 0
      | Some(amount) => amount
      }
    );