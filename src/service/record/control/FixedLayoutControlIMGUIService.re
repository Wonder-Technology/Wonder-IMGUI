open IMGUIType;

let label = (rect, str, record) => record |> DrawTextService.draw(rect, str);

let image = (rect, uv, record) =>
  DrawImageIMGUIService.draw(rect, uv, record);

/* let button = () => {

   }; */

/* let beginGroup = () => {

   };


   let endGroup = () => {

   }; */

/*

 let progressBar = () => {

 }; */