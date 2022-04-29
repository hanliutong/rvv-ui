#ifndef __INTRIN_RVV_HPP__
#define __INTRIN_RVV_HPP__

#ifndef RVV_MAX_VLEN
#define RVV_MAX_VLEN 1024
#endif

#define CV_SIMD_SCALABLE 1
#define CV_SIMD_SCALABLE_64F 1

#define SCALABLE_HAL_BEGIN RVV_COMPATIBLE_BEGIN
#define SCALABLE_HAL_END RVV_COMPATIBLE_END

#define RVV_COMPATIBLE_BEGIN {\
using rvv::v_float32; \
using rvv::VTraits; \
using rvv::v_setall_f32; \
using rvv::v_load; \
using rvv::v_fma; \
using rvv::v_store; \
using rvv::v_mul; \
using rvv::v_add;

#define RVV_COMPATIBLE_END }


namespace cv
{
namespace rvv {

using v_float32 = vfloat32m1_t;

template <class T>
struct VTraits;

template <>
struct VTraits<v_float32> 
{
    static unsigned int nlanes; 
    using lane_type = float;
    static const unsigned int max_nlanes = RVV_MAX_VLEN/sizeof(lane_type);
};

unsigned int VTraits<v_float32>::nlanes = vsetvlmax_e32m1();

#define OPENCV_HAL_IMPL_RVV_LOADSTORE_OP(_Tpvec, _Tp, vl, width, suffix) \
inline _Tpvec v_load(const _Tp* ptr) \
{ \
    return _Tpvec(vle##width##_v_##suffix##m1(ptr, vl)); \
} \
inline void v_store(_Tp* ptr, const _Tpvec& a) \
{ \
    vse##width(ptr, a, vl); \
} \

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

inline v_float32 v_add(v_float32 f1, v_float32 f2) {
    return vfadd(f1, f2, VTraits<v_float32>::nlanes);
}

template<typename... Args>
inline v_float32 v_add(v_float32 f1, v_float32 f2, Args... vf) {
    return v_add(vfadd(f1, f2, VTraits<v_float32>::nlanes), vf...);
}

inline v_float32 v_mul(v_float32 f1, v_float32 f2) {
    return vfmul(f1, f2, VTraits<v_float32>::nlanes);
}

template<typename... Args>
inline v_float32 v_mul(v_float32 f1, v_float32 f2, Args... vf) {
    return v_mul(vfmul(f1, f2, VTraits<v_float32>::nlanes), vf...);
}

inline vfloat32m1_t v_setall_f32(const float v) {
    return vfmv_v_f_f32m1(v, VTraits<v_float32>::nlanes);
}

#define OPENCV_HAL_IMPL_RVV_FMA(_Tpvec, _Tp, rvv_Intrin) \
inline _Tpvec v_fma(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c) { \
    return rvv_Intrin(c, a, b, VTraits<_Tpvec>::nlanes); \
}

OPENCV_HAL_IMPL_RVV_FMA(vfloat32m1_t, float, vfmacc)

} //namespace rvv
} //namespace cv
#endif //__INTRIN_RVV_HPP__