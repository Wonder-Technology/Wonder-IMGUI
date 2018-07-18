let load = (customTextureSourceDataArr, record) =>
  AssetIMGUIService.load(
    switch (Js.Nullable.toOption(customTextureSourceDataArr)) {
    | None => [||]
    | Some(customTextureSourceDataArr) => customTextureSourceDataArr
    },
    FetchService.fetch,
    record,
  );

let addFont = (fntFilePath, bitmapFilePath, record) =>
  FontIMGUIService.addFont((fntFilePath, bitmapFilePath), record);
