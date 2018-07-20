open IMGUIType;

let label =
  (. rect: StructureType.rect, str, align, record) =>
    record |> DrawTextIMGUIService.draw(rect, str, align);

let image =
  (. rect, uv, id, record) =>
    record
    |> DrawImageIMGUIService.draw(
         rect,
         uv,
         AssetIMGUIService.unsafeGetCustomTexture(id, record),
       );

let button =
  (. rect, str, record) =>
    ButtonControlIMGUIService.button(rect, str, record);

/* let beginGroup = () => {

   };


   let endGroup = () => {

   }; */

/*

 let progressBar = () => {

 }; */