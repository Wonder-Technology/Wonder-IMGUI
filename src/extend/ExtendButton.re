open SkinType;

module Skin = {
  let createSkinData =
      (
        ~buttonColor=[|0.35, 0.1, 0.1|],
        ~hoverButtonColor=[|0.40, 0.1, 0.1|],
        ~clickButtonColor=[|0.50, 0.1, 0.1|],
        ~buttonImage=Js.Nullable.null,
        ~hoverButtonImage=Js.Nullable.null,
        ~clickButtonImage=Js.Nullable.null,
        (),
      ) => {
    buttonColor,
    hoverButtonColor,
    clickButtonColor,
    buttonImage,
    hoverButtonImage,
    clickButtonImage,
  };

  let createDefaultSkinData = () => createSkinData();

  let getSkinData = skinData => skinData.buttonSkinData;

  let setSkinData = (buttonSkinData, skinData) => {
    ...skinData,
    buttonSkinData,
  };
};

module CustomControl = {
  external toInputData:
    ((StructureType.floatRect, string)) =>
    IMGUIType.customControlFuncInputData =
    "%identity";

  let buildCustomControl = () =>
    (. customControlFuncData, showData, apiJsObj, record) => {
      let (rect, str) = customControlFuncData |> Obj.magic;
      /* TODO move to apiJsObj? */
      let isInBox = ((x, y, width, height), (posX: float, posY: float)) => {
        open Pervasives;

        let minX = x;
        let minY = y;
        let maxX = x +. width;
        let maxY = y +. height;

        minX <= posX && posX <= maxX && minY <= posY && posY <= maxY;
      };

      let convertIntPositionToFloatPosition = ((x, y)) => (
        x |> NumberType.intToFloat,
        y |> NumberType.intToFloat,
      );

      let judge =
          (
            (clickButtonData: 'a, hoverButtonData: 'b, buttonData: 'c),
            (pointDown, pointPosition),
            isClickFunc,
            record,
          ) =>
        isInBox(rect, convertIntPositionToFloatPosition(pointPosition)) ?
          isClickFunc(. record) ?
            (true, clickButtonData) :
            pointDown ? (false, clickButtonData) : (false, hoverButtonData) :
          (false, buttonData);

      let apiJsObj = apiJsObj |> Obj.magic;

      let getIOData = apiJsObj##getIOData;
      let getPointUp = apiJsObj##getPointUp;
      let getPointDown = apiJsObj##getPointDown;
      let getPointPosition = apiJsObj##getPointPosition;
      let isClick = apiJsObj##isClick;

      let drawBox = apiJsObj##drawBox;
      let drawText = apiJsObj##drawText;
      let drawImage = apiJsObj##drawImage;

      let ioData = getIOData(. record);
      let pointUp = getPointUp(. ioData);
      let pointDown = getPointDown(. ioData);
      let pointPosition = getPointPosition(. ioData);

      let parseShowData = apiJsObj##parseShowData;
      let unsafeGetSkinData = apiJsObj##unsafeGetSkinData;
      let unsafeGetSingleCustomStyleDataMap =
        apiJsObj##unsafeGetSingleCustomStyleDataMap;

      let unsafeGetCustomStyleData = apiJsObj##unsafeGetCustomStyleData;

      let hasCustomStyleData = apiJsObj##hasCustomStyleData;

      let (skinName, singleCustomStyleName) = parseShowData(. showData);

      let {
        buttonColor,
        hoverButtonColor,
        clickButtonColor,
        buttonImage,
        hoverButtonImage,
        clickButtonImage,
      } =
        ManageSkinIMGUIService.unsafeGetSkinData(skinName, record)
        |> Skin.getSkinData;

      let (isButtonClick, (imageId, color)) =
        judge(
          (
            (clickButtonImage, clickButtonColor),
            (hoverButtonImage, hoverButtonColor),
            (buttonImage, buttonColor),
          ),
          (pointDown, pointPosition),
          isClick,
          record,
        );

      let record =
        switch (Js.Nullable.toOption(imageId)) {
        | None => drawBox(. rect, color, record)
        | Some(imageId) =>
          drawImage(. rect, (0., 0., 1., 1.), imageId, record)
        };

      let record = drawText(. rect, str, FontType.Center, record);

      (record, isButtonClick |> Obj.magic);
    };

  let getCustomControlName = () => "Wonder_Button";

  let button =
    (. (rect, str), showData, record) => {
      let buttonFunc =
        ManageCustomControlIMGUIService.unsafeGetCustomControl(.
          getCustomControlName(),
          record,
        );

      buttonFunc(.
        (
          GroupLayoutIMGUIService.computeRectBasedOnTopLeftOfView(rect, record)
          |> StructureService.convertIntRectToFloatRect,
          str,
        )
        |> toInputData,
        showData,
        record,
      );
    };
};