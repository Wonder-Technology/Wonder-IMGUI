open Js.Promise;

open IMGUIType;

open WonderBsMost;

let getFntData = ({assetData} as record) => {
  let {fntDataMap} = assetData;

  fntDataMap |> WonderCommonlib.MutableHashMapService.get(assetData.fntId);
};

let unsafeGetFntData = record =>
  getFntData(record) |> OptionService.unsafeGet;

let setFntData = (fntData, {assetData} as record) => {
  let {fntDataMap} = assetData;

  fntDataMap
  |> WonderCommonlib.MutableHashMapService.set(assetData.fntId, fntData)
  |> ignore;

  record;
};

let getBitmap = ({assetData} as record) => {
  let {bitmapMap} = assetData;

  bitmapMap |> WonderCommonlib.MutableHashMapService.get(assetData.bitmapId);
};

let unsafeGetBitmap = record => getBitmap(record) |> OptionService.unsafeGet;

let setBitmap = (bitmap, {assetData} as record) => {
  let {bitmapMap} = assetData;

  bitmapMap
  |> WonderCommonlib.MutableHashMapService.set(assetData.bitmapId, bitmap)
  |> ignore;

  record;
};

let isLoadAsset = record =>
  /* RecordIMGUIService.getFontData(record)
     |> Js.Option.isSome */
  getBitmap(record) |> Js.Option.isSome;

let load =
    (
      customTextureSourceDataArr,
      (fetchFunc, handleWhenLoadingFunc),
      {assetData} as record,
    ) => {
  let customImageArr = assetData.customImageArr;
  let imguiRecord = ref(Obj.magic(1));

  Most.mergeArray(
    [|
      FontIMGUIService.load(fetchFunc, handleWhenLoadingFunc, record)
      |> then_(record => {
           imguiRecord := record;
           () |> resolve;
         })
      |> Most.fromPromise,
    |]
    |> Js.Array.concat(
         customTextureSourceDataArr
         |> Js.Array.map(((imagePath, imageId)) =>
              Most.fromPromise(
                fetchFunc(. imagePath)
                |> then_(response => {
                     handleWhenLoadingFunc(
                       FetchService.getContentLength(response),
                       imagePath,
                     );

                     response |> resolve;
                   })
                |> then_(Fetch.Response.blob),
              )
              |> Most.flatMap(blob =>
                   ImageService.loadImageByBlobPromise(
                     blob |> Blob.createObjectURL,
                   )
                   |> Most.tap(image => Blob.revokeObjectURL(blob))
                 )
              |> Most.map(image => {
                   customImageArr
                   |> ArrayService.push((
                        image,
                        imageId,
                        ImageService.getType(imagePath),
                      ))
                   |> ignore;
                   ();
                 })
            ),
       ),
  )
  |> Most.drain
  |> then_(() =>
       {
         ...imguiRecord^,
         assetData: {
           ...imguiRecord^.assetData,
           customImageArr,
         },
       }
       |> resolve
     );
};

let createCustomTextures = (gl, customImageArr, customTextureMap) => {
  open WonderWebgl;
  open Gl;

  let rgb = getRgb(gl);
  let rgba = getRgba(gl);
  let target = getTexture2D(gl);

  customImageArr
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. customTextureMap, data) => {
         let (image, id, type_) = data;
         let format =
           switch (type_) {
           | ImageType.Jpg => rgb
           | ImageType.Png => rgba
           | _ =>
             WonderLog.Log.fatal(
               WonderLog.Log.buildFatalMessage(
                 ~title="createCustomTextures",
                 ~description=
                   {j|unknown image type. type should be jpg or png.|j},
                 ~reason="",
                 ~solution={j||j},
                 ~params={j||j},
               ),
             )
           };

         let texture = createTexture(gl);

         bindTexture(target, texture, gl);

         texParameteri(target, getTextureMinFilter(gl), getLinear(gl), gl);

         texImage2D(
           target,
           0,
           format,
           format,
           getUnsignedByte(gl),
           image |> WonderWebgl.GlType.imageElementToTextureSource,
           gl,
         );

         customTextureMap |> WonderCommonlib.MutableHashMapService.set(id, texture);
       },
       customTextureMap,
     );
};

let unsafeGetCustomTexture = (id, {assetData}) =>
  assetData.customTextureMap |> WonderCommonlib.MutableHashMapService.unsafeGet(id);

let getCustomImageArr = ({assetData}) => assetData.customImageArr;

let setCustomImageArr = (customImageArr, record) => {
  ...record,
  assetData: {
    ...record.assetData,
    customImageArr,
  },
};