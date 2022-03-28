#ifndef __INTRIN_RVV_HPP__
#define __INTRIN_RVV_HPP__

#include <riscv_vector.h>

struct v_float32x4
{
    typedef float lane_type;
    enum { nlanes = 4 };

    v_float32x4() {}
    explicit v_float32x4(vfloat32m1_t v)
    {
        vse32_v_f32m1(val, v, nlanes);
    }
    v_float32x4(float v0, float v1, float v2, float v3)
    {
        float v[] = {v0, v1, v2, v3};
        for (int i = 0; i < nlanes; ++i)
        {
            val[i] = v[i];
        }
    }
    operator vfloat32m1_t() const
    {
        return vle32_v_f32m1(val, nlanes);
    }
    float get0() const
    {
        return val[0];
    }
    float val[4];
};

struct v_int16x8
{
    typedef int lane_type;
    enum { nlanes = 8 };

    v_int16x8() {}
    explicit v_int16x8(vint16m1_t v)
    {
        vse16_v_i16m1(val, v, nlanes);
    }
    v_int16x8(short v0, short v1, short v2, short v3)
    {
        short v[] = {v0, v1, v2, v3};
        for (int i = 0; i < nlanes; ++i)
        {
            val[i] = v[i];
        }
    }
    operator vint16m1_t() const
    {
        return vle16_v_i16m1(val, nlanes);
    }
    short get0() const
    {
        return val[0];
    }
    short val[8];
};

using v_float32 = v_float32x4;
using v_int16 = v_int16x8;

template <class T>
struct VTraits;

template <>
struct VTraits<v_float32> 
{
    static unsigned int nlanes; 
    using lane_type = float;
};

template <>
struct VTraits<v_int16> 
{
    static unsigned int nlanes; 
    using lane_type = short;
};

unsigned int VTraits<v_float32>::nlanes = 4;
unsigned int VTraits<v_int16>::nlanes = 8;

#define OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(_Tpvec, _Tp, vl, width, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ \
    return _Tpvec(vle##width##_v_##suffix##m1(ptr, vl)); \
} \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ \
    vse##width##_v_##suffix##m1(ptr, a, vl); \
} \

OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(v_int16x8, short, 8, 16, i16)
OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(v_float32x4, float, 4, 32, f32)

inline v_int16x8 v_setall_i16(float v) \
{ \
    return v_int16x8(vmv_v_x_i16m1(v, 4)); \
}
inline v_float32x4 v_setall_f32(float v) \
{ \
    return v_float32x4(vfmv_v_f_f32m1(v, 4)); \
}

inline v_float32x4 v_fma(const v_float32x4& a, const v_float32x4& b, const v_float32x4& c)
{
    return v_float32x4(vfmacc_vv_f32m1(c, a, b, 4));
}

inline v_int16x8 v_fma(const v_int16x8& a, const v_int16x8& b, const v_int16x8& c)
{
    return v_int16x8(vmacc_vv_i16m1(c, a, b, 8));
}

inline v_float32x4 v_fma(const float a, const v_float32x4& b, const v_float32x4& c)
{
    return v_float32x4(vfmacc_vf_f32m1(c, a, b, 4));
}

inline v_int16x8 v_fma(const short a, const v_int16x8& b, const v_int16x8& c)
{
    return v_int16x8(vmacc_vx_i16m1(c, a, b, 8));
}

#define OPENCV_HAL_IMPL_RVV_BIN_OP(bin_op, _Tpvec, intrin, vl) \
inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b) \
{ \
    return _Tpvec(intrin(a, b, vl)); \
} \
inline _Tpvec& operator bin_op##= (_Tpvec& a, const _Tpvec& b) \
{ \
    a = _Tpvec(intrin(a, b, vl)); \
    return a; \
}

OPENCV_HAL_IMPL_RVV_BIN_OP(+, v_int16x8, vadd_vv_i16m1, 8)
OPENCV_HAL_IMPL_RVV_BIN_OP(-, v_int16x8, vsub_vv_i16m1, 8)
OPENCV_HAL_IMPL_RVV_BIN_OP(*, v_int16x8, vmul_vv_i16m1, 8)
OPENCV_HAL_IMPL_RVV_BIN_OP(/, v_int16x8, vdiv_vv_i16m1, 8)
OPENCV_HAL_IMPL_RVV_BIN_OP(+, v_float32x4, vfadd_vv_f32m1, 4)
OPENCV_HAL_IMPL_RVV_BIN_OP(-, v_float32x4, vfsub_vv_f32m1, 4)
OPENCV_HAL_IMPL_RVV_BIN_OP(*, v_float32x4, vfmul_vv_f32m1, 4)
OPENCV_HAL_IMPL_RVV_BIN_OP(/, v_float32x4, vfdiv_vv_f32m1, 4)

#endif //__INTRIN_RVV_HPP__