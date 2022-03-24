.PHONY: all clean
-include *.d
all: gemm.out 
	/opt/RISCV/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=128 gemm_fixed.out 
	/opt/RISCV/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=128 gemm.out 

gemm.out : gemm.cpp
	/riscv/llvm-project/build/bin/clang++ -D_FIXED -march=rv64gcv --target=riscv64-unknown-linux-gnu --sysroot=/opt/RISCV/sysroot --gcc-toolchain=/opt/RISCV -O2 $< -MMD -o gemm_fixed.out
	/riscv/llvm-project/build/bin/clang++ -march=rv64gcv --target=riscv64-unknown-linux-gnu --sysroot=/opt/RISCV/sysroot --gcc-toolchain=/opt/RISCV -O2 $< -MMD -o gemm.out 

clean:
	rm -f *.out *.d