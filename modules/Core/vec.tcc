/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#if !defined(AVR_CORE_STATIC_VEC_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_STATIC_VEC_TCC

#ifdef __cplusplus

namespace cv {

/////////////////////////// short vector (Vec<>) /////////////////////////////

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec() {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0)
    : Matx<_Tp, cn, 1>(v0) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1)
    : Matx<_Tp, cn, 1>(v0, v1) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2)
    : Matx<_Tp, cn, 1>(v0, v1, v2) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                                                        _Tp v8)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7, v8) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                                                        _Tp v8, _Tp v9)
    : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(const _Tp* values)
    : Matx<_Tp, cn, 1>(values) {}


template<typename _Tp, int cn> inline Vec<_Tp, cn>::Vec(const Vec<_Tp, cn>& m)
    : Matx<_Tp, cn, 1>(m.val) {}

template<typename _Tp, int cn> inline Vec<_Tp, cn> Vec<_Tp, cn>::all(_Tp alpha)
{
    Vec v;
    for( int i = 0; i < cn; i++ ) v.val[i] = alpha;
    return v;
}

template<typename _Tp, int cn> inline Vec<_Tp, cn> Vec<_Tp, cn>::mul(const Vec<_Tp, cn>& v) const
{
    Vec<_Tp, cn> w;
    for( int i = 0; i < cn; i++ ) w.val[i] = saturate_cast<_Tp>(this->val[i]*v.val[i]);
    return w;
}

template<typename _Tp> inline Vec<_Tp, 2> conjugate(const Vec<_Tp, 2>& v)
{
    return Vec<_Tp, 2>(v[0], -v[1]);
}

template<typename _Tp> inline Vec<_Tp, 4> conjugate(const Vec<_Tp, 4>& v)
{
    return Vec<_Tp, 4>(v[0], -v[1], -v[2], -v[3]);
}

template<typename _Tp, int cn> inline Vec<_Tp, cn> Vec<_Tp, cn>::conj() const
{
   if(cn == 4 || cn == 2) return conjugate(*this);
   AVR_ERROR(Cod::TemplateArgument, "for arbitrary-size vector there is no conjugation defined");
   return Vec<_Tp, cn>();
}
template<> inline Vec<int, 2> Vec<int, 2>::conj() const { return conjugate(*this); }
template<> inline Vec<float, 2> Vec<float, 2>::conj() const { return conjugate(*this); }
template<> inline Vec<double, 2> Vec<double, 2>::conj() const { return conjugate(*this); }
template<> inline Vec<int, 4> Vec<int, 4>::conj() const { return conjugate(*this); }
template<> inline Vec<float, 4> Vec<float, 4>::conj() const { return conjugate(*this); }
template<> inline Vec<double, 4> Vec<double, 4>::conj() const { return conjugate(*this); }

template<typename _Tp, int cn> inline Vec<_Tp, cn> Vec<_Tp, cn>::cross(const Vec<_Tp, cn>&) const
{
    AVR_ERROR(Cod::TemplateArgument, "for arbitrary-size vector there is no cross-product defined");
    return Vec<_Tp, cn>();
}

template<> inline Vec<float, 3> Vec<float, 3>::cross(const Vec<float, 3>& v) const
{
    return Vec<float,3>(val[1]*v.val[2] - val[2]*v.val[1],
                     val[2]*v.val[0] - val[0]*v.val[2],
                     val[0]*v.val[1] - val[1]*v.val[0]);
}

template<> inline Vec<double, 3> Vec<double, 3>::cross(const Vec<double, 3>& v) const
{
    return Vec<double,3>(val[1]*v.val[2] - val[2]*v.val[1],
                     val[2]*v.val[0] - val[0]*v.val[2],
                     val[0]*v.val[1] - val[1]*v.val[0]);
}

template<typename _Tp, int cn> template<typename T2>
inline Vec<_Tp, cn>::operator Vec<T2, cn>() const
{
    Vec<T2, cn> v;
    for( int i = 0; i < cn; i++ ) v.val[i] = saturate_cast<T2>(this->val[i]);
    return v;
}

template<typename _Tp, int cn> inline const _Tp& Vec<_Tp, cn>::operator [](int i) const
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline _Tp& Vec<_Tp, cn>::operator [](int i)
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline const _Tp& Vec<_Tp, cn>::operator ()(int i) const
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline _Tp& Vec<_Tp, cn>::operator ()(int i)
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

/// Adition
template<typename _Tp1, typename _Tp2, int cn> static inline Vec<_Tp1, cn>&
operator += (Vec<_Tp1, cn>& a, const Vec<_Tp2, cn>& b)
{
    for( int i = 0; i < cn; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] + b.val[i]);
    return a;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator + (const Vec<_Tp, cn>& a, const Vec<_Tp, cn>& b)
{
    Vec<_Tp, cn> sum;
    for(int i = 0; i < cn; i++)
        sum[i] = a[i] + b[i];
    return sum;
}

/// Subtraction
template<typename _Tp1, typename _Tp2, int cn> static inline Vec<_Tp1, cn>&
operator -= (Vec<_Tp1, cn>& a, const Vec<_Tp2, cn>& b)
{
    for( int i = 0; i < cn; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] - b.val[i]);
    return a;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator - (const Vec<_Tp, cn>& a, const Vec<_Tp, cn>& b)
{
    Vec<_Tp, cn> sub;
    for(int i = 0; i < cn; i++)
        sub[i] = a[i] - b[i];
    return sub;
}

/// Multiplication by scalar
template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, int alpha)
{
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * alpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, float alpha)
{
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * alpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, double alpha)
{
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * alpha);
    return a;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (const Vec<_Tp, cn>& a, int alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (int alpha, const Vec<_Tp, cn>& a)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (const Vec<_Tp, cn>& a, float alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (float alpha, const Vec<_Tp, cn>& a)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (const Vec<_Tp, cn>& a, double alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator * (double alpha, const Vec<_Tp, cn>& a)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * alpha;
    return mul;
}

/// Division by scalar
template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, int alpha)
{
    double ialpha = 1./alpha;
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * ialpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, float alpha)
{
    float ialpha = 1.f/alpha;
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * ialpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, double alpha)
{
    double ialpha = 1./alpha;
    for( int i = 0; i < cn; i++ )
        a[i] = saturate_cast<_Tp>(a[i] * ialpha);
    return a;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator / (const Vec<_Tp, cn>& a, int alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * (1./alpha);
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator / (const Vec<_Tp, cn>& a, float alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * (1.f/alpha);
    return mul;
}

template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator / (const Vec<_Tp, cn>& a, double alpha)
{
    Vec<_Tp, cn> mul;
    for(int i = 0; i < cn; i++)
        mul[i] = a[i] * (1./alpha);
    return mul;
}

/// Negation
template<typename _Tp, int cn> static inline Vec<_Tp, cn>
operator - (const Vec<_Tp, cn>& a)
{
    Vec<_Tp,cn> t;
    for( int i = 0; i < cn; i++ ) t.val[i] = saturate_cast<_Tp>(-a.val[i]);
    return t;
}

/// Quaternion multiplication
template<typename _Tp> inline Vec<_Tp, 4> operator * (const Vec<_Tp, 4>& v1, const Vec<_Tp, 4>& v2)
{
    return Vec<_Tp, 4>(saturate_cast<_Tp>(v1[0]*v2[0] - v1[1]*v2[1] - v1[2]*v2[2] - v1[3]*v2[3]),
                       saturate_cast<_Tp>(v1[0]*v2[1] + v1[1]*v2[0] + v1[2]*v2[3] - v1[3]*v2[2]),
                       saturate_cast<_Tp>(v1[0]*v2[2] - v1[1]*v2[3] + v1[2]*v2[0] + v1[3]*v2[1]),
                       saturate_cast<_Tp>(v1[0]*v2[3] + v1[1]*v2[2] - v1[2]*v2[1] + v1[3]*v2[0]));
}
template<typename _Tp> inline Vec<_Tp, 4>& operator *= (Vec<_Tp, 4>& v1, const Vec<_Tp, 4>& v2)
{
    v1 = v1 * v2;
    return v1;
}

template<typename _Tp, int cn> inline Vec<_Tp, cn> normalize(const Vec<_Tp, cn>& v)
{
    double nv = norm(v);
    return v * (nv ? 1./nv : 0.);
}

template<typename _Tp, int cn> static inline std::ostream& operator << (std::ostream& out, const Vec<_Tp, cn>& v)
{
    out << "[";
    for(int i = 0; i < cn; i++)
        out << v[i] << ", ";
    out << "\b\b]";
    return out;
}

} //namespace cv

#endif // __cplusplus

#endif
