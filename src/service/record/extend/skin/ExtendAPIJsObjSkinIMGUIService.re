open IMGUIType;

let parseShowData =
  (. showData: customControlFunctionShowData) =>
    switch (Js.Nullable.toOption(showData)) {
    | None => (
        DataSkinIMGUIService.getDefaultSkinName(),
        DataSkinIMGUIService.getDefaultCustomStyleName(),
      )
    | Some(showData) => showData
    };