let load = (customTextureSourceDataArr, handleWhenLoadingFunc, record) =>
  AssetIMGUIService.LoadAsset.load(
    switch (Js.Nullable.toOption(customTextureSourceDataArr)) {
    | None => [||]
    | Some(customTextureSourceDataArr) => customTextureSourceDataArr
    },
    (FetchService.fetch, handleWhenLoadingFunc),
    record,
  );

let addFont = (fntFilePath, bitmapFilePath, record) =>
  FontIMGUIService.addFont((fntFilePath, bitmapFilePath), record);

let buildIOData = (pointUp, pointDown, pointPosition, pointMovementDelta, ()) =>
  IOIMGUIService.buildIOData(
    ~pointUp,
    ~pointDown,
    ~pointPosition,
    ~pointMovementDelta,
    (),
  );