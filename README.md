# Universal Intrinsic for RISC-V Vector

The goal of this project is to optimize the current [Universal Intrinsic](https://docs.opencv.org/4.x/df/d91/group__core__hal__intrin.html) for RVV, mainly including the following two points.

## Reduce the overhead of using wrapper class
There are unnecessary load & store instructions because of using wrapper class, which generates redundant instructions about data interaction between memory and registers. 

## Support for multiple VLENs
Each vector register in RVV has a fixed VLEN bit of state, but different RVV hardware devices can have different VLEN. Multiple(or scalable) VLENs is the one of the most different between RVV and existing SIMD Arch. But the current RVV UI implements only support VLEN=128.

## Here is a saxpy example on [godbolt](https://godbolt.org/z/EMc5373dW)
