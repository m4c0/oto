const app = document.getElementById("app");
const ctx = app.getContext("2d");

var obj;

const imp = {
  env: {
    oto_draw: (ptr, w, h) => {
      const pixels = new Uint8ClampedArray(obj.instance.exports.memory.buffer, Number(ptr), w * h * 4);
      ctx.putImageData(new ImageData(pixels, w, h), 0, 0);
    },
    oto_log: (ptr) => {
      const view = new Uint8Array(obj.instance.exports.memory.buffer);
      var str = '';
      while (view[ptr] != 0) {
        str += String.fromCharCode(view[ptr]);
        ptr++;
      }
      console.log(str);
    },
    memory: new WebAssembly.Memory({ initial: 512 }),
  },
  wasi_snapshot_preview1: {
    clock_time_get: (id, precision, out) => { 
      var arr = new Uint32Array(obj.instance.exports.memory.buffer, out, 4);
      arr[0] = Date.now();
      return 0;
    },
    fd_close: () => { console.log("fd_close") },
    fd_seek: () => { console.log("fd_seek") },
    fd_write: () => { console.log("fd_write") },
    proc_exit: () => { throw "oops" }
  },
};

function repaint() {
  obj.instance.exports.casein_repaint();
  window.requestAnimationFrame(repaint);
}
function start(w) {
  obj = w;
  obj.instance.exports.casein_create_window();
  window.requestAnimationFrame(repaint);
}

fetch('poc.wasm')
  .then(response => response.arrayBuffer())
  .then(bytes => WebAssembly.instantiate(bytes, imp))
  .then(start);

