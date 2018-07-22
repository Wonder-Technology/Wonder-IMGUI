open IMGUIType;

let label =
  (. rect: StructureType.rect, str, align, record) =>
    record
    |> DrawTextIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
         |> StructureService.convertIntRectToFloatRect,
         str,
         align,
       );

let image =
  (. rect, uv, id, record) =>
    record
    |> DrawImageIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
         |> StructureService.convertIntRectToFloatRect,
         uv,
         AssetIMGUIService.unsafeGetCustomTexture(id, record),
       );

let button =
  (. rect, str, record) =>
    ButtonControlIMGUIService.button(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      str,
      record,
    );

let box =
  (. rect, color, record) =>
    DrawBoxIMGUIService.draw(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      color,
      record,
    );

let radioButton =
  (. groupDataArr, group, record) =>
    RadioButtonControlIMGUIService.radioButton(groupDataArr, group, record);