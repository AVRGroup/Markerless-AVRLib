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

#if !defined(AVR_CORE_SCALAR_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_SCALAR_TCC

#ifdef __cplusplus

namespace cv {

//////////////////////////////// Scalar_ ///////////////////////////////

template<typename _Tp> inline Scalar_<_Tp>::Scalar_()
{ this->val[0] = this->val[1] = this->val[2] = this->val[3] = 0; }
template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{ this->val[0] = v0; this->val[1] = v1; this->val[2] = v2; this->val[3] = v3; }
template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0)
{ this->val[0] = v0; this->val[1] = this->val[2] = this->val[3] = 0; }

template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::all(_Tp v0)
{ return Scalar_<_Tp>(v0, v0, v0, v0); }

template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::mul(const Scalar_<_Tp>& t, double scale ) const
{
    return Scalar_<_Tp>( saturate_cast<_Tp>(this->val[0]*t.val[0]*scale),
                       saturate_cast<_Tp>(this->val[1]*t.val[1]*scale),
                       saturate_cast<_Tp>(this->val[2]*t.val[2]*scale),
                       saturate_cast<_Tp>(this->val[3]*t.val[3]*scale));
}

template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::conj() const
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(this->val[0]),
                        saturate_cast<_Tp>(-this->val[1]),
                        saturate_cast<_Tp>(-this->val[2]),
                        saturate_cast<_Tp>(-this->val[3]));
}

template<typename _Tp> inline bool Scalar_<_Tp>::isReal() const
{ return this->val[1] == 0 && this->val[2] == 0 && this->val[3] == 0; }

template<typename _Tp> template<typename T2> inline Scalar_<_Tp>::operator Scalar_<T2>() const
{
    return Scalar_<T2>(saturate_cast<T2>(this->val[0]),
                  saturate_cast<T2>(this->val[1]),
                  saturate_cast<T2>(this->val[2]),
                  saturate_cast<T2>(this->val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp>& operator += (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a.val[0] = saturate_cast<_Tp>(a.val[0] + b.val[0]);
    a.val[1] = saturate_cast<_Tp>(a.val[1] + b.val[1]);
    a.val[2] = saturate_cast<_Tp>(a.val[2] + b.val[2]);
    a.val[3] = saturate_cast<_Tp>(a.val[3] + b.val[3]);
    return a;
}

template<typename _Tp> static inline Scalar_<_Tp>& operator -= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a.val[0] = saturate_cast<_Tp>(a.val[0] - b.val[0]);
    a.val[1] = saturate_cast<_Tp>(a.val[1] - b.val[1]);
    a.val[2] = saturate_cast<_Tp>(a.val[2] - b.val[2]);
    a.val[3] = saturate_cast<_Tp>(a.val[3] - b.val[3]);
    return a;
}

template<typename _Tp> static inline Scalar_<_Tp>& operator *= ( Scalar_<_Tp>& a, _Tp v )
{
    a.val[0] = saturate_cast<_Tp>(a.val[0] * v);
    a.val[1] = saturate_cast<_Tp>(a.val[1] * v);
    a.val[2] = saturate_cast<_Tp>(a.val[2] * v);
    a.val[3] = saturate_cast<_Tp>(a.val[3] * v);
    return a;
}

template<typename _Tp> static inline bool operator == ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
    return a.val[0] == b.val[0] && a.val[1] == b.val[1] &&
        a.val[2] == b.val[2] && a.val[3] == b.val[3];
}

template<typename _Tp> static inline bool operator != ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
    return a.val[0] != b.val[0] || a.val[1] != b.val[1] ||
        a.val[2] != b.val[2] || a.val[3] != b.val[3];
}

template<typename _Tp> static inline Scalar_<_Tp> operator + (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] + b.val[0]),
                      saturate_cast<_Tp>(a.val[1] + b.val[1]),
                      saturate_cast<_Tp>(a.val[2] + b.val[2]),
                      saturate_cast<_Tp>(a.val[3] + b.val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp> operator - (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] - b.val[0]),
                      saturate_cast<_Tp>(a.val[1] - b.val[1]),
                      saturate_cast<_Tp>(a.val[2] - b.val[2]),
                      saturate_cast<_Tp>(a.val[3] - b.val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp> operator * (const Scalar_<_Tp>& a, _Tp alpha)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] * alpha),
                      saturate_cast<_Tp>(a.val[1] * alpha),
                      saturate_cast<_Tp>(a.val[2] * alpha),
                      saturate_cast<_Tp>(a.val[3] * alpha));
}

template<typename _Tp> static inline Scalar_<_Tp> operator * (_Tp alpha, const Scalar_<_Tp>& a)
{
    return a*alpha;
}

template<typename _Tp> static inline Scalar_<_Tp> operator - (const Scalar_<_Tp>& a)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(-a.val[0]), saturate_cast<_Tp>(-a.val[1]),
                      saturate_cast<_Tp>(-a.val[2]), saturate_cast<_Tp>(-a.val[3]));
}


template<typename _Tp> static inline Scalar_<_Tp>
operator * (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3]),
                        saturate_cast<_Tp>(a[0]*b[1] + a[1]*b[0] + a[2]*b[3] - a[3]*b[2]),
                        saturate_cast<_Tp>(a[0]*b[2] - a[1]*b[3] + a[2]*b[0] + a[3]*b[1]),
                        saturate_cast<_Tp>(a[0]*b[3] + a[1]*b[2] - a[2]*b[1] + a[3]*b[0]));
}

template<typename _Tp> static inline Scalar_<_Tp>&
operator *= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a = a * b;
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (const Scalar_<_Tp>& a, _Tp alpha)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] / alpha),
                        saturate_cast<_Tp>(a.val[1] / alpha),
                        saturate_cast<_Tp>(a.val[2] / alpha),
                        saturate_cast<_Tp>(a.val[3] / alpha));
}

template<typename _Tp> static inline
Scalar_<float> operator / (const Scalar_<float>& a, float alpha)
{
    float s = 1.0f/alpha;
    return Scalar_<float>(a.val[0]*s, a.val[1]*s, a.val[2]*s, a.val[3]*s);
}

template<typename _Tp> static inline
Scalar_<double> operator / (const Scalar_<double>& a, double alpha)
{
    double s = 1.0/alpha;
    return Scalar_<double>(a.val[0]*s, a.val[1]*s, a.val[2]*s, a.val[3]*s);
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator /= (Scalar_<_Tp>& a, _Tp alpha)
{
    a = a/alpha;
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (_Tp a, const Scalar_<_Tp>& b)
{
    _Tp s = a/(b[0]*b[0] + b[1]*b[1] + b[2]*b[2] + b[3]*b[3]);
    return b.conj()*s;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return a*((_Tp)1.0/b);
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator /= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a = a/b;
    return a;
}

template<typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Scalar_<_Tp>& s)
{ return (out << "[" << s[0] << ", [" << s[1] << ", " << s[2] << ", " << s[3] << "]]"); }
template<typename _Tp> static inline std::istream& operator >> (std::istream& in, Scalar_<_Tp>& s)
{ return (in >> s[0] >> s[1] >> s[2] >> s[3]); }

} // namespace cv

#endif // __cplusplus

#endif
