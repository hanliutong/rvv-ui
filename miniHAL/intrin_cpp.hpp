#ifndef OPENCV_HAL_INTRIN_CPP_HPP
#define OPENCV_HAL_INTRIN_CPP_HPP
// #include <cstddef>
// #include <algorithm>

#define CV_SIMD128_CPP 1
#if defined(CV_FORCE_SIMD128_CPP)
#define CV_SIMD128 1
#define CV_SIMD128_64F 1
#endif

namespace cv
{
template<typename _Tp, int n> struct v_reg
{
    typedef _Tp lane_type;
    enum { nlanes = n };

    explicit v_reg(const _Tp* ptr) { for( int i = 0; i < n; i++ ) s[i] = ptr[i]; }

    v_reg(_Tp s0, _Tp s1) { s[0] = s0; s[1] = s1; }

    v_reg(_Tp s0, _Tp s1, _Tp s2, _Tp s3) { s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3; }

    v_reg(_Tp s0, _Tp s1, _Tp s2, _Tp s3,
           _Tp s4, _Tp s5, _Tp s6, _Tp s7)
    {
        s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3;
        s[4] = s4; s[5] = s5; s[6] = s6; s[7] = s7;
    }

    v_reg(_Tp s0, _Tp s1, _Tp s2, _Tp s3,
           _Tp s4, _Tp s5, _Tp s6, _Tp s7,
           _Tp s8, _Tp s9, _Tp s10, _Tp s11,
           _Tp s12, _Tp s13, _Tp s14, _Tp s15)
    {
        s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3;
        s[4] = s4; s[5] = s5; s[6] = s6; s[7] = s7;
        s[8] = s8; s[9] = s9; s[10] = s10; s[11] = s11;
        s[12] = s12; s[13] = s13; s[14] = s14; s[15] = s15;
    }

    v_reg() {}


    v_reg(const v_reg<_Tp, n> & r)
    {
        for( int i = 0; i < n; i++ )
            s[i] = r.s[i];
    }

    static v_reg<_Tp, n> zero()
    {
        v_reg<_Tp, n> c;
        for( int i = 0; i < n; i++ )
            c.s[i] = (_Tp)0;
        return c;
    }

    static v_reg<_Tp, n> all(_Tp s)
    {
        v_reg<_Tp, n> c;
        for( int i = 0; i < n; i++ )
            c.s[i] = s;
        return c;
    }
    v_reg& operator=(const v_reg<_Tp, n> & r)
    {
        for( int i = 0; i < n; i++ )
            s[i] = r.s[i];
        return *this;
    }

    _Tp s[n];
};


typedef v_reg<short, 8> v_int16x8;

typedef v_reg<float, 4> v_float32x4;


enum {
    simd128_width = 16,
#if CV_SIMD256
    simd256_width = 32,
#endif
#if CV_SIMD512
    simd512_width = 64,
    simdmax_width = simd512_width
#elif CV_SIMD256
    simdmax_width = simd256_width
#else
    simdmax_width = simd128_width
#endif
};

template<typename _Tp>
inline v_reg<_Tp, simd128_width / sizeof(_Tp)> v_load(const _Tp* ptr)
{
    return v_reg<_Tp, simd128_width / sizeof(_Tp)>(ptr);
}

template<typename _Tp, int n>
inline void v_store(_Tp* ptr, const v_reg<_Tp, n>& a)
{
    for( int i = 0; i < n; i++ )
        ptr[i] = a.s[i];
}

#define OPENCV_HAL_IMPL_C_INIT_VAL(_Tpvec, _Tp, prefix, suffix) \
inline _Tpvec prefix##_setall_##suffix(_Tp val) { return _Tpvec::all(val); }

OPENCV_HAL_IMPL_C_INIT_VAL(v_int16x8, short, v, s16)
OPENCV_HAL_IMPL_C_INIT_VAL(v_float32x4, float, v, f32)

template<typename _Tp, int n>
inline v_reg<_Tp, n> v_fma(const v_reg<_Tp, n>& a, const v_reg<_Tp, n>& b,
                           const v_reg<_Tp, n>& c)
{
    v_reg<_Tp, n> d;
    for( int i = 0; i < n; i++ )
        d.s[i] = a.s[i]*b.s[i] + c.s[i];
    return d;
}

template<typename _Tp, int n> inline v_reg<_Tp, n> operator+(const v_reg<_Tp, n>& a, const v_reg<_Tp, n>& b);
template<typename _Tp, int n> inline v_reg<_Tp, n> operator*(const v_reg<_Tp, n>& a, const v_reg<_Tp, n>& b);


}
#endif