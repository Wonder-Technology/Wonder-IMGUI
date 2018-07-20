let inBox = ((x, y, width, height), (posX, posY)) => {
  let minX = x;
  let minY = y;
  let maxX = x + width;
  let maxY = y + height;

  minX <= posX && posX <= maxX && minY <= y && y <= maxY;
};