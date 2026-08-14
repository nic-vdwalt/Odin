#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
wasm_cc=${WASM_CC:-clang}
wasm_ld=${WASM_LD:-wasm-ld}

./build.sh
"$wasm_cc" -c -O2 -std=gnu17 --target=wasm32 -ffreestanding -matomics -mbulk-memory \
	-nostdlibinc -isystem wasm_include -Iinclude -include wasm_compat.h \
	test_wasm_threads.c -o build/test_wasm_threads.o
"$wasm_ld" --no-entry --import-memory --shared-memory --max-memory=33554432 \
	--export=__stack_pointer \
	-o build/test_wasm_threads.wasm build/test_wasm_threads.o ../lib/box3d_wasm_threads.o
node test_wasm_threads.mjs build/test_wasm_threads.wasm
