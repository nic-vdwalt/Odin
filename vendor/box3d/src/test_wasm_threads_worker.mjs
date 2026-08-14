import { parentPort, workerData } from "node:worker_threads";

const module = await WebAssembly.compile(workerData.bytes);
const instance = await WebAssembly.instantiate(module, {
	env: { memory: workerData.memory },
});
instance.exports.__stack_pointer.value = workerData.stackPointer;

parentPort.on("message", ({ count }) => {
	instance.exports.box3d_mutex_test_run(count);
	parentPort.postMessage("done");
});

parentPort.postMessage("ready");
