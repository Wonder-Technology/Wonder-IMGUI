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

let checkbox =
  (. rect, str, record) =>
    CheckboxControlIMGUIService.checkbox(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      str,
      record,
    );

let sliderInt =
  (. (rect, textWidth), (min, max), (defaultValue, str), record) =>
    SliderControlIMGUIService.sliderInt(
      (
        GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
        |> StructureService.convertIntRectToFloatRect,
        textWidth |> NumberType.intToFloat,
      ),
      (min, max),
      (defaultValue, str),
      record,
    );

let sliderFloat =
  (.
    (rect, textWidth),
    (min, max, numDecimalDigits),
    (defaultValue, str),
    record,
  ) =>
    SliderControlIMGUIService.sliderFloat(
      (
        GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
        |> StructureService.convertIntRectToFloatRect,
        textWidth |> NumberType.intToFloat,
      ),
      (min, max, numDecimalDigits),
      (defaultValue, str),
      record,
    );