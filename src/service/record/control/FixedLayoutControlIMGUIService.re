open IMGUIType;

let label = (rect, str, record) =>
  record |> DrawTextIMGUIService.draw(rect, str);

let image = (rect, uv, record) =>
  record |> DrawImageIMGUIService.draw(rect, uv);

/* let button = () => {

   }; */

/* let beginGroup = () => {

   };


   let endGroup = () => {

   }; */

/*

 let progressBar = () => {

 }; */