let isInBox = ((x, y, width, height), (posX, posY)) => {
  let minX = x;
  let minY = y;
  let maxX = x +. width;
  let maxY = y +. height;

  minX <= posX && posX <= maxX && minY <= posY && posY <= maxY;
};

let isInCircle = ((x, y, width, height), (posX, posY)) => {
  let (centerX, centerY) = (x +. 0.5 *. width, y +. 0.5 *. height);

  let radius = width /. 2.;

  let dist = Js.Math.sqrt((x -. centerX) ** 2. +. (y -. centerY) ** 2.);

  dist <= radius;
};