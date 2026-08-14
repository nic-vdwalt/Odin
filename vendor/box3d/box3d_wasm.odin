#+build wasm32, wasm64p32
package vendor_box3d

import "base:runtime"
import "core:mem"

@(private)
box3d_wasm_alloc :: proc "c" (size, alignment: i32) -> rawptr {
	context = runtime.default_context()
	memory, err := mem.alloc(int(size), int(alignment), context.allocator)
	if err != nil {
		return nil
	}
	return memory
}

@(private)
box3d_wasm_free :: proc "c" (memory: rawptr) {
	context = runtime.default_context()
	_ = mem.free(memory, context.allocator)
}

@(init)
box3d_wasm_init :: proc "contextless" () {
	SetAllocator(box3d_wasm_alloc, box3d_wasm_free)
}
