let load = record => FontIMGUIService.load(FetchService.fetch, record);

let addFont = (fntFilePath, bitmapFilePath, record) =>
  FontIMGUIService.addFont(
    (fntFilePath, bitmapFilePath),
    ("fnt", "bitmap"),
    record,
  );