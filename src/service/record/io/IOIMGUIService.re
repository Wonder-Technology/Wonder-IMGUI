let buildIOData =
    (
      ~pointUp=false,
      ~pointDown=false,
      ~pointPosition=(0, 0),
      ~pointMovementDelta=(0, 0),
      (),
    ) => {
  "pointUp": pointUp,
  "pointDown": pointDown,
  "pointPosition": pointPosition,
  "pointMovementDelta": pointMovementDelta,
};