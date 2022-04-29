LLVM_BUILD_PATH = /riscv/llvm-project/build/bin
GNU_TC_PATH = /opt/RISCV

.PHONY: run clean
-include *.d

run :run_x86 run_sse run_rvv
	
run_x86: saxpy_x86.out
	./saxpy_x86.out
run_sse: saxpy_sse.out
	./saxpy_sse.out
run_rvv: saxpy_rvv.out
	$(GNU_TC_PATH)/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=128 saxpy_rvv.out
	$(GNU_TC_PATH)/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=1024 saxpy_rvv.out

saxpy_x86.out : saxpy.cpp
	$(LLVM_BUILD_PATH)/clang++ -mno-sse -O2 $< -MMD -o $@
saxpy_sse.out : saxpy.cpp
	$(LLVM_BUILD_PATH)/clang++ -msse -O2 $< -MMD -o $@
saxpy_rvv.out : saxpy.cpp
	$(LLVM_BUILD_PATH)/clang++ -march=rv64gcv --target=riscv64-unknown-linux-gnu --sysroot=$(GNU_TC_PATH)/sysroot --gcc-toolchain=$(GNU_TC_PATH) -O2 $< -MMD -o $@


clean:
	rm -f *.out *.d