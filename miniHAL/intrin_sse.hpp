
#ifndef OPENCV_HAL_SSE_HPP
#define OPENCV_HAL_SSE_HPP

#include <emmintrin.h>

#define CV_SIMD128 1

namespace cv
{

struct v_float32x4
{
    typedef float lane_type;
    typedef __m128 vector_type;
    enum { nlanes = 4 };

    /* coverity[uninit_ctor]: suppress warning */
    v_float32x4() {}
    explicit v_float32x4(__m128 v) : val(v) {}
    v_float32x4(float v0, float v1, float v2, float v3)
    {
        val = _mm_setr_ps(v0, v1, v2, v3);
    }

    float get0() const
    {
        return _mm_cvtss_f32(val);
    }

    __m128 val;
};

#define OPENCV_HAL_IMPL_SSE_LOADSTORE_FLT_OP(_Tpvec, _Tp, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ return _Tpvec(_mm_loadu_##suffix(ptr)); } \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ _mm_storeu_##suffix(ptr, a.val); }

OPENCV_HAL_IMPL_SSE_LOADSTORE_FLT_OP(v_float32x4, float, ps)

inline v_float32x4 v_setall_f32(float v) { return v_float32x4(_mm_set1_ps((float)v)); }

#define OPENCV_HAL_IMPL_SSE_BIN_OP(bin_op, _Tpvec, intrin) \
    inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b) \
    { \
        return _Tpvec(intrin(a.val, b.val)); \
    } \
    inline _Tpvec& operator bin_op##= (_Tpvec& a, const _Tpvec& b) \
    { \
        a.val = intrin(a.val, b.val); \
        return a; \
    }

OPENCV_HAL_IMPL_SSE_BIN_OP(+, v_float32x4, _mm_add_ps)
OPENCV_HAL_IMPL_SSE_BIN_OP(-, v_float32x4, _mm_sub_ps)
OPENCV_HAL_IMPL_SSE_BIN_OP(*, v_float32x4, _mm_mul_ps)
OPENCV_HAL_IMPL_SSE_BIN_OP(/, v_float32x4, _mm_div_ps)

inline v_float32x4 v_fma(const v_float32x4& a, const v_float32x4& b, const v_float32x4& c)
{
    return v_float32x4(_mm_add_ps(_mm_mul_ps(a.val, b.val), c.val));
}

}

#endif