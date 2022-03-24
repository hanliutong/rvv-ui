#include <riscv_vector.h>

using v_float32 = vfloat32m1_t;
using v_int16 = vint16m1_t;

template <class T>
class VTraits
{
public:
    typedef float lane_type;
    static unsigned int nlanes; 
};
template <>
unsigned int VTraits<v_float32>::nlanes = vsetvlmax_e32m1();
template <>
unsigned int VTraits<v_int16>::nlanes = vsetvlmax_e32m1();

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


inline vfloat32m1_t v_setall_f32(const float v) {
    return vfmv_v_f_f32m1(v, VTraits<v_float32>::nlanes);
}
inline vint16m1_t v_setall_i16(const int v) {
    return vmv_v_x_i16m1(v, VTraits<v_int16>::nlanes);
}

#define OPENCV_HAL_IMPL_RVV_FMA(rvv_T, rvv_Intrin)\
inline rvv_T v_fma(const rvv_T& a, const rvv_T& b, const rvv_T& c) {\
    return rvv_Intrin(c, a, b, VTraits<rvv_T>::nlanes);\
}

OPENCV_HAL_IMPL_RVV_FMA(vfloat32m1_t, vfmacc)
OPENCV_HAL_IMPL_RVV_FMA(vint16m1_t, vmacc)

