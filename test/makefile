LLVM_BUILD_PATH = /riscv/llvm-project/build/bin
GNU_TC_PATH = /opt/RISCV

src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %.out, ${src})

.PHONY: all clean
-include *.d

%.out : %.cpp
	$(LLVM_BUILD_PATH)/clang++ -march=rv64gcv --target=riscv64-unknown-linux-gnu --sysroot=$(GNU_TC_PATH)/sysroot --gcc-toolchain=$(GNU_TC_PATH) -O2 $< -MMD -o $@

%. : %.out
	$(GNU_TC_PATH)/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=128 $^
	$(GNU_TC_PATH)/bin/qemu-riscv64 -cpu rv64,x-v=true,vlen=256 $^
all: ${target}

clean:
	rm -f ${target}