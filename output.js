function draw() {
  const canvas = document.getElementById("canvas");
  if (canvas.getContext) {
    const ctx = canvas.getContext("2d");

// r1
let x = 10, y = 10, width = 5, height = 10;
ctx.fillStyle = "red";
ctx.fillRect(x, y, width, height);
// c1
x = 20; y = 30;
let radius = 5;
ctx.beginPath();
ctx.arc(x, y, radius, 0, 2 * Math.PI);
ctx.fillStyle = "yellow";
ctx.fill();
// l1
x = 5; y = 15;
let length = 15;
ctx.beginPath();
ctx.moveTo(x, y);
ctx.lineTo(x + length, y);
ctx.strokeStyle = "teal";
ctx.stroke();
  }
}
draw();

