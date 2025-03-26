build:
	wasmcc index.cpp -o index.wasm -Wl,--allow-undefined -std=c++17 -I./includes -O3 -mllvm -inline-threshold=10000
	wasm-opt index.wasm -o index.wasm \
		--shrink-level=100000000 \
		--coalesce-locals-learning \
		--vacuum \
		--merge-blocks \
		--merge-locals \
		--flatten \
		--ignore-implicit-traps \
		-ffm \
		--const-hoisting \
		--code-folding \
		--code-pushing \
		--dae-optimizing \
		--dce \
		--simplify-globals-optimizing \
		--simplify-locals-nonesting \
		--reorder-locals \
		--rereloop \
		--precompute-propagate \
		--local-cse \
		--remove-unused-brs \
		--memory-packing \
		-c \
		--avoid-reinterprets \
		-Oz
	hook-cleaner index.wasm
	guard_checker index.wasm
