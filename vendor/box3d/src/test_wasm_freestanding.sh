#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
wasm_cc=${WASM_CC:-clang}
wasm_ld=${WASM_LD:-wasm-ld}
llvm_nm=${LLVM_NM:-llvm-nm}

./build.sh
"$wasm_cc" -c -O2 -std=gnu17 --target=wasm32 -ffreestanding -nostdlibinc \
	-isystem wasm_include -Iinclude -include wasm_compat.h \
	test_wasm_freestanding.c -o build/test_wasm_freestanding.o
"$wasm_ld" --no-entry --export=main --allow-undefined \
	-o build/test_wasm_freestanding.wasm build/test_wasm_freestanding.o ../lib/box3d_wasm.o

undefined=$(
	"$llvm_nm" --undefined-only ../lib/box3d_wasm.o |
		sed -E '/(__indirect_function_table|__stack_pointer)$/d; /^[[:space:]]*$/d'
)
if [[ -n "$undefined" ]]; then
	echo "Unexpected undefined symbols in box3d_wasm.o:"
	echo "$undefined"
	exit 1
fi
