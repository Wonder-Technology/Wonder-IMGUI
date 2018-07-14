open IMGUIType;

let label = (rect, str, record) =>
  record |> DrawTextIMGUIService.draw(rect, str);

let image = (rect, uv, texture, record) =>
  record |> DrawImageIMGUIService.draw(rect, uv, texture);

/* let button = () => {

   }; */

/* let beginGroup = () => {

   };


   let endGroup = () => {

   }; */

/*

 let progressBar = () => {

 }; */