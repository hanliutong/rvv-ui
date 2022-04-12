#ifndef __INTRIN_RVV_HPP_NEW__
#define __INTRIN_RVV_HPP_NEW__

#ifndef RVV_MAX_VLEN
#define RVV_MAX_VLEN 1024
#endif

#include <riscv_vector.h>

using v_float32 = vfloat32m1_t;
using v_int16 = vint16m1_t;

template <class T>
struct VTraits;

template <>
struct VTraits<v_float32> 
{
    static unsigned int nlanes; 
    using lane_type = float;
    static const unsigned int max_nlanes = RVV_MAX_VLEN/sizeof(lane_type);
};

template <>
struct VTraits<v_int16> 
{
    static unsigned int nlanes; 
    using lane_type = short;
    static const unsigned int max_nlanes = RVV_MAX_VLEN/sizeof(lane_type);
};

unsigned int VTraits<v_float32>::nlanes = vsetvlmax_e32m1();
unsigned int VTraits<v_int16>::nlanes = vsetvlmax_e16m1();

#define OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(_Tpvec, _Tp, vl, width, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ \
    return _Tpvec(vle##width##_v_##suffix##m1(ptr, vl)); \
} \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ \
    vse##width(ptr, a, vl); \
} \

OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(v_int16, short, VTraits<v_int16>::nlanes, 16, i16)
OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(v_float32, float, VTraits<v_float32>::nlanes, 32, f32)

#include <initializer_list>
#include <assert.h>

inline v_float32 v_load(std::initializer_list<float> nScalars)
{
    // assert( nScalars.size() <= VTraits<v_float32>::nlanes);
    return vle32_v_f32m1(nScalars.begin(), nScalars.size());
}
template<typename... Args>
inline v_float32 v_load(float v, Args... vec) {
    return v_load({v, vec...});
}

inline v_int16 v_load(std::initializer_list<short> nScalars)
{
    // assert( nScalars.size() <= VTraits<v_int16>::nlanes);
    return vle16_v_i16m1(nScalars.begin(), nScalars.size());
}
template<typename... Args>
inline v_int16 v_load(short v, Args... vec) {
    return v_load({v, vec...});
}


inline vfloat32m1_t v_setall_f32(const float v) {
    return vfmv_v_f_f32m1(v, VTraits<v_float32>::nlanes);
}
inline vint16m1_t v_setall_i16(const int v) {
    return vmv_v_x_i16m1(v, VTraits<v_int16>::nlanes);
}

#define OPENCV_HAL_IMPL_RVV_FMA(_Tpvec, _Tp, rvv_Intrin) \
inline _Tpvec v_fma(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c) { \
    return rvv_Intrin(c, a, b, VTraits<_Tpvec>::nlanes); \
} \
inline _Tpvec v_fma(const _Tp& a, const _Tpvec& b, const _Tpvec& c) { \
    return rvv_Intrin(c, a, b, VTraits<_Tpvec>::nlanes); \
}

OPENCV_HAL_IMPL_RVV_FMA(vfloat32m1_t, float, vfmacc)
OPENCV_HAL_IMPL_RVV_FMA(vint16m1_t, short, vmacc)

#endif //__INTRIN_RVV_HPP_NEW__