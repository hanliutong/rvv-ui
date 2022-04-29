#ifndef OPENCV_HAL_INTRIN_HPP
#define OPENCV_HAL_INTRIN_HPP

#if defined (__SSE2__)
#  include <emmintrin.h>
#  define CV_SSE2 1
#elif defined (__riscv_vector)
#include <riscv_vector.h>
#  define CV_RVV 1
#endif

#if CV_SSE2
#include "intrin_sse.hpp"
#elif CV_RVV
#include "intrin_cpp.hpp"
#include "intrin_rvv.hpp"
#else
#include "intrin_cpp.hpp"
#endif

namespace cv {

#if CV_SIMD128 || CV_SIMD128_CPP
#if defined CV_SIMD128_CPP
#define CV__SIMD_NAMESPACE simd128_cpp
#else
#define CV__SIMD_NAMESPACE simd128
#endif
namespace CV__SIMD_NAMESPACE {
    #define CV_SIMD CV_SIMD128
    #define CV_SIMD_64F CV_SIMD128_64F
    #define CV_SIMD_WIDTH 16

    typedef v_float32x4 v_float32;

    #define VXPREFIX(func) v##func
} // namespace
using namespace CV__SIMD_NAMESPACE;
#else
#define CV__SIMD_NAMESPACE simd
namespace CV__SIMD_NAMESPACE {
    #define CV_SIMD 0
    #define CV_SIMD_WIDTH 0

    #define VXPREFIX(func) v##func
} // namespace
using namespace CV__SIMD_NAMESPACE;
#endif

#if CV_SIMD
    
    #define CV_SIMD_SCALABLE 0
    #define CV_SIMD_SCALABLE_64F 0
    #define SCALABLE_HAL_BEGIN 
    #define SCALABLE_HAL_END 

    template <class T>
    struct VTraits;

    template <>
    struct VTraits<v_float32> 
    {
        const static int nlanes = v_float32::nlanes; 
        using lane_type = float;
    };

    #define OPENCV_HAL_IMPL_RVV_BIN_OP_FP(_Tpvec, vl) \
    inline _Tpvec v_add(const _Tpvec& a, const _Tpvec& b) \
    { \
        return a + b; \
    } \
    inline _Tpvec v_mul(const _Tpvec& a, const _Tpvec& b) \
    { \
        return a * b; \
    } \
    template<typename... Args> \
    inline _Tpvec v_add(_Tpvec f1, _Tpvec f2, Args... vf) { \
        return v_add(f1 + f2, vf...); \
    } \
    template<typename... Args> \
    inline _Tpvec v_mul(_Tpvec f1, _Tpvec f2, Args... vf) { \
        return v_mul(f1 * f2, vf...); \
    }

    OPENCV_HAL_IMPL_RVV_BIN_OP_FP( v_float32, VTraits<v_float32>::nlanes)

#endif

} //namespace cv
#endif
