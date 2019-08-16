open IMGUIType;

let label =
  (. rect: StructureType.rect, str, cssData, record) =>
    record
    |> DrawTextIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
         |> StructureService.convertIntRectToFloatRect,
         str,
         cssData,
       );

let image =
  (. rect, uv, id, record) =>
    record
    |> DrawImageIMGUIService.draw(
         GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
         |> StructureService.convertIntRectToFloatRect,
         uv,
         /* AssetIMGUIService.unsafeGetCustomTexture(id, record), */
         id,
       );

/* let button =
  (. rect, str, record) =>
    ButtonControlIMGUIService.button(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      str,
      record,
    ); */

let box =
  (. rect, color: array(float), record) =>
    DrawBoxIMGUIService.draw(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      color,
      record,
    );

/* let radioButton =
  (. groupDataArr, defaultSelectIndex, group, record) =>
    RadioButtonControlIMGUIService.radioButton(
      groupDataArr,
      defaultSelectIndex,
      group,
      record,
    );

let checkbox =
  (. rect, defaultSelected, str, record) =>
    CheckboxControlIMGUIService.checkbox(
      GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
      |> StructureService.convertIntRectToFloatRect,
      defaultSelected,
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
    ); */