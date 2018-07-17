open IMGUIType;

let label =
  (. rect: StructureType.rect, str, align, record) =>
    record |> DrawTextIMGUIService.draw(rect, str, align);

let image =
  (. rect, uv, texture, record) =>
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