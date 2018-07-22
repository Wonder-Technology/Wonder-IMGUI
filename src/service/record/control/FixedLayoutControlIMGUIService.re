open IMGUIType;

let label =
  (. rect: StructureType.rect, str, align, record) =>
    record
    |> DrawTextIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(
           rect,
           record,
         ),
         str,
         align,
       );

let image =
  (. rect, uv, id, record) =>
    record
    |> DrawImageIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(
           rect,
           record,
         ),
         uv,
         AssetIMGUIService.unsafeGetCustomTexture(id, record),
       );

let button =
  (. rect, str, record) =>
    ButtonControlIMGUIService.button(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record),
      str,
      record,
    );