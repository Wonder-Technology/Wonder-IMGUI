open IMGUIType;

let parseShowData =
  (. showData: customControlFunctionShowData) =>
    switch (Js.Nullable.toOption(showData)) {
    | None => (
        DataSkinIMGUIService.getDefaultSkinName(),
        Js.Nullable.return(DataSkinIMGUIService.getDefaultCustomStyleName()),
      )
    | Some(showData) => showData
    };