import assert from "node:assert/strict";
import fs from "node:fs";
import { Worker } from "node:worker_threads";

const bytes = fs.readFileSync(process.argv[2]);
const module = await WebAssembly.compile(bytes);
const memory = new WebAssembly.Memory({ initial: 512, maximum: 512, shared: true });
const main = await WebAssembly.instantiate(module, { env: { memory } });
main.exports.__stack_pointer.value = 2 * 1024 * 1024;
main.exports.box3d_mutex_test_init();

const workerCounts = [1, 2, 3, 4];
const iterations = 100000;
let previous = 0;

for (const workerCount of workerCounts) {
	const workers = [];
	for (let index = 0; index < workerCount; index += 1) {
		workers.push(new Worker(new URL("./test_wasm_threads_worker.mjs", import.meta.url), {
			workerData: {
				bytes,
				memory,
				stackPointer: (index + 3) * 1024 * 1024,
			},
		}));
	}

	await Promise.all(workers.map((worker) => new Promise((resolve, reject) => {
		worker.once("message", resolve);
		worker.once("error", reject);
	})));
	await Promise.all(workers.map((worker) => new Promise((resolve, reject) => {
		worker.once("message", resolve);
		worker.once("error", reject);
		worker.postMessage({ count: iterations });
	})));

	const expected = previous + workerCount * iterations;
	assert.equal(main.exports.box3d_mutex_test_value(), expected);
	previous = expected;
	await Promise.all(workers.map((worker) => worker.terminate()));
}
