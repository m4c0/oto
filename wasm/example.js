const app = document.getElementById("app");
const ctx = app.getContext("2d");

var exp;

const imp = {
  env: {
    oto_draw: (ptr) => {
      const pixels = new Uint8ClampedArray(exp.memory.buffer, Number(ptr), 800 * 600 * 4);
      ctx.putImageData(new ImageData(pixels, 800, 600), 0, 0);
    },
    oto_log: (ptr) => {
      const view = new Uint8Array(exp.memory.buffer);
      var str = '';
      while (view[ptr] != 0) {
        str += String.fromCharCode(view[ptr]);
        ptr++;
      }
      console.log(str);
    },
  },
  wasi_snapshot_preview1: {
    clock_time_get: () => { console.log("clock_time_get") },
    fd_close: () => { console.log("fd_close") },
    fd_seek: () => { console.log("fd_seek") },
    fd_write: () => { console.log("fd_write") },
    proc_exit: () => { throw "oops" }
  },
};

function repaint() {
  exp.casein_repaint();
  window.requestAnimationFrame(repaint);
}
function start(exports) {
  exp = exports;
  console.log(exp);
  exp._start();
  window.requestAnimationFrame(repaint);
}

fetch('poc.wasm')
  .then(response => response.arrayBuffer())
  .then(bytes => WebAssembly.instantiate(bytes, imp))
  .then(obj => obj.instance.exports)
  .then(start);

