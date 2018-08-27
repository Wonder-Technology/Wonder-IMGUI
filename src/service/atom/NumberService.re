let clamp = (num, below, up) =>
  /* WonderLog.Contract.requireCheck(
       () =>
         WonderLog.(
           Contract.(
             Operators.(
               test(
                 Log.buildAssertMessage(
                   ~expect={j|below <= up|j},
                   ~actual={j|not|j},
                 ),
                 () =>
                 below <=. up
               )
             )
           )
         ),
       IsDebugMainService.getIsDebug(StateDataMain.stateData),
     ); */
  num < below ?
    below : num > up ? up : num;

let convertStringToInt: string => int = [%bs.raw
  {|
function(str) {
    return parseInt(str, 10);
}
|}
];