let hasCustomControl = ManageCustomControlIMGUIService.hasCustomControl;

let registerCustomControl = ManageCustomControlIMGUIService.registerCustomControl;

let createAllCustomStyleData = ManageSkinIMGUIService.createAllCustomStyleData;

let createSingleCustomStyleData = ManageSkinIMGUIService.createSingleCustomStyleData;

let hasCustomStyleData = ManageSkinIMGUIService.hasCustomStyleData;

let addCustomStyleData = ManageSkinIMGUIService.addCustomStyleData;

let removeCustomStyleData = ManageSkinIMGUIService.removeCustomStyleData;

let addSingleCustomStyleData = ManageSkinIMGUIService.addSingleCustomStyleData;

let removeSingleCustomStyleData = ManageSkinIMGUIService.removeSingleCustomStyleData;

let hasSkinData = ManageSkinIMGUIService.hasSkinData;

let addSkinData = ManageSkinIMGUIService.addSkinData;

let removeSkinData = ManageSkinIMGUIService.removeSkinData;

let createSkinData = ManageSkinIMGUIService.createSkinData;

let unsafeGetSkinData = ManageSkinIMGUIService.unsafeGetSkinData;

let unsafeGetDefaultSkinData = DataSkinIMGUIService.unsafeGetDefaultSkinData;

let setSkinData = ManageSkinIMGUIService.setSkinData;

let clearAllSkins = ManageSkinIMGUIService.clearAllSkins;

let addDefaultSkinData = DataSkinIMGUIService.addDefaultSkinData;

let getDefaultSkinName = DataSkinIMGUIService.getDefaultSkinName;

let getButtonSkinData = ExtendButton.Skin.getSkinData;

let setButtonSkinData = ExtendButton.Skin.setSkinData;

let createButtonSkinData =
    (
      buttonColor,
      hoverButtonColor,
      clickButtonColor,
      buttonImage,
      hoverButtonImage,
      clickButtonImage,
      fontAlign,
      fontColor,
    )
    : SkinType.buttonSkinData => {
  buttonColor,
  hoverButtonColor,
  clickButtonColor,
  buttonImage,
  hoverButtonImage,
  clickButtonImage,
  fontAlign,
  fontColor,
};