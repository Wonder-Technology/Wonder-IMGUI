open WonderWebgl.GlType;

open IMGUIType;

open DrawDataType;

let _getOrCreateCustomTextureDrawData = (id, record): customTextureDrawData =>
  switch (
    RecordIMGUIService.getCustomTextureDrawDataMap(record)
    |> WonderCommonlib.MutableHashMapService.get(id)
  ) {
  | None => {
      customTexture:
        AssetIMGUIService.unsafeGetCustomTexture(id, record)->Some,
      verticeArr: [||],
      colorArr: [||],
      texCoordArr: [||],
      indexArr: [||],
    }
  | Some(drawData) => drawData
  };

let draw = ((x, y, width, height), (s0, t0, s1, t1), id, record) => {
  let ({verticeArr, colorArr, texCoordArr, indexArr}: customTextureDrawData) as drawData =
    _getOrCreateCustomTextureDrawData(id, record);

  let baseIndex = DrawDataArrayService.getBaseIndex(verticeArr);

  let drawData = {
    ...drawData,
    verticeArr:
      verticeArr
      |> DrawDataArrayService.addPoints([|
           x,
           y,
           x,
           y +. height,
           x +. width,
           y,
           x +. width,
           y +. height,
         |]),
    colorArr:
      colorArr
      |> DrawDataArrayService.addPoints([|
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
           1.,
         |]),
    texCoordArr:
      texCoordArr
      |> DrawDataArrayService.addPoints([|s0, t0, s0, t1, s1, t0, s1, t1|]),
    indexArr:
      indexArr
      |> DrawDataArrayService.addPoints([|
           baseIndex,
           baseIndex + 1,
           baseIndex + 2,
           baseIndex + 3,
           baseIndex + 2,
           baseIndex + 1,
         |]),
  };

  {
    ...record,
    drawData: {
      ...record.drawData,
      customTextureDrawDataMap:
        record.drawData.customTextureDrawDataMap
        |> WonderCommonlib.MutableHashMapService.set(id, drawData),
    },
  };
};