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

//This file implements the classes:
//    - Complex<>
//    - Matx_<>
//    - Vec_<>
//    - Scalar_<>

#if !defined(AVR_CORE_ALGEBRA_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_ALGEBRA_TCC

#ifdef __cplusplus

namespace cv {
// begin operations.hpp
//////////////////////////////// Complex //////////////////////////////

template<typename _Tp> inline Complex<_Tp>::Complex() : re(0), im(0) {}
template<typename _Tp> inline Complex<_Tp>::Complex( _Tp _re, _Tp _im ) : re(_re), im(_im) {}
template<typename _Tp> template<typename T2> inline Complex<_Tp>::operator Complex<T2>() const
{ return Complex<T2>(saturate_cast<T2>(re), saturate_cast<T2>(im)); }
template<typename _Tp> inline Complex<_Tp> Complex<_Tp>::conj() const
{ return Complex<_Tp>(re, -im); }

template<typename _Tp> static inline
bool operator == (const Complex<_Tp>& a, const Complex<_Tp>& b)
{ return a.re == b.re && a.im == b.im; }

template<typename _Tp> static inline
bool operator != (const Complex<_Tp>& a, const Complex<_Tp>& b)
{ return a.re != b.re || a.im != b.im; }

template<typename _Tp> static inline
Complex<_Tp> operator + (const Complex<_Tp>& a, const Complex<_Tp>& b)
{ return Complex<_Tp>( a.re + b.re, a.im + b.im ); }

template<typename _Tp> static inline
Complex<_Tp>& operator += (Complex<_Tp>& a, const Complex<_Tp>& b)
{ a.re += b.re; a.im += b.im; return a; }

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a, const Complex<_Tp>& b)
{ return Complex<_Tp>( a.re - b.re, a.im - b.im ); }

template<typename _Tp> static inline
Complex<_Tp>& operator -= (Complex<_Tp>& a, const Complex<_Tp>& b)
{ a.re -= b.re; a.im -= b.im; return a; }

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a)
{ return Complex<_Tp>(-a.re, -a.im); }

template<typename _Tp> static inline
Complex<_Tp> operator * (const Complex<_Tp>& a, const Complex<_Tp>& b)
{ return Complex<_Tp>( a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re ); }

template<typename _Tp> static inline
Complex<_Tp> operator * (const Complex<_Tp>& a, _Tp b)
{ return Complex<_Tp>( a.re*b, a.im*b ); }

template<typename _Tp> static inline
Complex<_Tp> operator * (_Tp b, const Complex<_Tp>& a)
{ return Complex<_Tp>( a.re*b, a.im*b ); }

template<typename _Tp> static inline
Complex<_Tp> operator + (const Complex<_Tp>& a, _Tp b)
{ return Complex<_Tp>( a.re + b, a.im ); }

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a, _Tp b)
{ return Complex<_Tp>( a.re - b, a.im ); }

template<typename _Tp> static inline
Complex<_Tp> operator + (_Tp b, const Complex<_Tp>& a)
{ return Complex<_Tp>( a.re + b, a.im ); }

template<typename _Tp> static inline
Complex<_Tp> operator - (_Tp b, const Complex<_Tp>& a)
{ return Complex<_Tp>( b - a.re, -a.im ); }

template<typename _Tp> static inline
Complex<_Tp>& operator += (Complex<_Tp>& a, _Tp b)
{ a.re += b; return a; }

template<typename _Tp> static inline
Complex<_Tp>& operator -= (Complex<_Tp>& a, _Tp b)
{ a.re -= b; return a; }

template<typename _Tp> static inline
Complex<_Tp>& operator *= (Complex<_Tp>& a, _Tp b)
{ a.re *= b; a.im *= b; return a; }

template<typename _Tp> static inline
double abs(const Complex<_Tp>& a)
{ return std::sqrt( (double)a.re*a.re + (double)a.im*a.im); }

template<typename _Tp> static inline
Complex<_Tp> operator / (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    double t = 1./((double)b.re*b.re + (double)b.im*b.im);
    return Complex<_Tp>( (_Tp)((a.re*b.re + a.im*b.im)*t),
                        (_Tp)((-a.re*b.im + a.im*b.re)*t) );
}

template<typename _Tp> static inline
Complex<_Tp>& operator /= (Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return (a = a / b);
}

template<typename _Tp> static inline
Complex<_Tp> operator / (const Complex<_Tp>& a, _Tp b)
{
    _Tp t = (_Tp)1/b;
    return Complex<_Tp>( a.re*t, a.im*t );
}

template<typename _Tp> static inline
Complex<_Tp> operator / (_Tp b, const Complex<_Tp>& a)
{
    return Complex<_Tp>(b)/a;
}

template<typename _Tp> static inline
Complex<_Tp> operator /= (const Complex<_Tp>& a, _Tp b)
{
    _Tp t = (_Tp)1/b;
    a.re *= t; a.im *= t; return a;
}

//////////////////////////////// Matx /////////////////////////////////

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx()
{ for(int i = 0; i < channels; i++) val[i] = _Tp(0); }

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0)
{
    val[0] = v0;
    for(int i = 1; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1)
{
    AVR_ASSERT(channels >= 2);
    val[0] = v0; val[1] = v1;
    for(int i = 2; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2)
{
    AVR_ASSERT(channels >= 3);
    val[0] = v0; val[1] = v1; val[2] = v2;
    for(int i = 3; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{
    AVR_ASSERT(channels >= 4);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    for(int i = 4; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
{
    AVR_ASSERT(channels >= 5);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    for(int i = 5; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5)
{
    AVR_ASSERT(channels >= 6);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5;
    for(int i = 6; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6)
{
    AVR_ASSERT(channels >= 7);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6;
    for(int i = 7; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7)
{
    AVR_ASSERT(channels >= 8);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    for(int i = 8; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                                                        _Tp v8)
{
    AVR_ASSERT(channels >= 9);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8;
    for(int i = 9; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                                                        _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                                                        _Tp v8, _Tp v9)
{
    AVR_ASSERT(channels >= 10);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9;
    for(int i = 10; i < channels; i++) val[i] = _Tp(0);
}


template<typename _Tp, int m, int n>
inline Matx<_Tp,m,n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                            _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                            _Tp v8, _Tp v9, _Tp v10, _Tp v11)
{
    AVR_ASSERT(channels == 12);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9; val[10] = v10; val[11] = v11;
}

template<typename _Tp, int m, int n>
inline Matx<_Tp,m,n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
                           _Tp v4, _Tp v5, _Tp v6, _Tp v7,
                           _Tp v8, _Tp v9, _Tp v10, _Tp v11,
                           _Tp v12, _Tp v13, _Tp v14, _Tp v15)
{
    AVR_ASSERT(channels == 16);
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9; val[10] = v10; val[11] = v11;
    val[12] = v12; val[13] = v13; val[14] = v14; val[15] = v15;
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx(const _Tp* values)
{
    for( int i = 0; i < channels; i++ ) val[i] = values[i];
}

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n> Matx<_Tp, m, n>::all(_Tp alpha)
{
    Matx<_Tp, m, n> M;
    for( int i = 0; i < m*n; i++ ) M.val[i] = alpha;
    return M;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::zeros()
{ return all(0); }

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::ones()
{ return all(1); }

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::eye()
{
    Matx<_Tp,m,n> M;
    for(int i = 0; i < std::min(m,n); i++)
        M(i,i) = 1;
    return M;
}

template<typename _Tp, int m, int n> inline _Tp Matx<_Tp, m, n>::dot(const Matx<_Tp, m, n>& M) const
{
    _Tp s = 0;
    for( int i = 0; i < m*n; i++ ) s += val[i]*M.val[i];
    return s;
}


template<typename _Tp, int m, int n> inline double Matx<_Tp, m, n>::ddot(const Matx<_Tp, m, n>& M) const
{
    double s = 0;
    for( int i = 0; i < m*n; i++ ) s += (double)val[i]*M.val[i];
    return s;
}



template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::diag(const typename Matx<_Tp,m,n>::diag_type& d)
{
    Matx<_Tp,m,n> M;
    for(int i = 0; i < std::min(m,n); i++)
        M(i,i) = d(i, 0);
    return M;
}

template<typename _Tp, int m, int n> template<typename T2>
inline Matx<_Tp, m, n>::operator Matx<T2, m, n>() const
{
    Matx<T2, m, n> M;
    for( int i = 0; i < m*n; i++ ) M.val[i] = saturate_cast<T2>(val[i]);
    return M;
}


template<typename _Tp, int m, int n> template<int m1, int n1> inline
Matx<_Tp, m1, n1> Matx<_Tp, m, n>::reshape() const
{
    AVR_DBG_ASSERT(m1 * n1 == m * n);
    return (const Matx<_Tp, m1, n1>&)*this;
}


template<typename _Tp, int m, int n>
template<int m1, int n1> inline
Matx<_Tp, m1, n1> Matx<_Tp, m, n>::get_minor(int i, int j) const
{
    AVR_DBG_ASSERT(0 <= i && i + m1 <= m && 0 <= j && j + n1 <= n);
    Matx<_Tp, m1, n1> s;
    for( int di = 0; di < m1; di++ )
        for( int dj = 0; dj < n1; dj++ )
            s(di, dj) = (*this)(i+di, j+dj);
    return s;
}


template<typename _Tp, int m, int n> inline
Matx<_Tp, 1, n> Matx<_Tp, m, n>::row(int i) const
{
    AVR_DBG_ASSERT((unsigned)i < (unsigned)m);
    return Matx<_Tp, 1, n>(&val[i*n]);
}


template<typename _Tp, int m, int n> inline
Matx<_Tp, m, 1> Matx<_Tp, m, n>::col(int j) const
{
    AVR_DBG_ASSERT((unsigned)j < (unsigned)n);
    Matx<_Tp, m, 1> v;
    for( int i = 0; i < m; i++ )
        v.val[i] = val[i*n + j];
    return v;
}


template<typename _Tp, int m, int n> inline
typename Matx<_Tp, m, n>::diag_type Matx<_Tp, m, n>::diag() const
{
    diag_type d;
    for( int i = 0; i < std::min(m, n); i++ )
        d.val[i] = val[i*n + i];
    return d;
}


template<typename _Tp, int m, int n> inline
const _Tp& Matx<_Tp, m, n>::operator ()(int i, int j) const
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)m && (unsigned)j < (unsigned)n );
    return this->val[i * n + j];
}


template<typename _Tp, int m, int n> inline
_Tp& Matx<_Tp, m, n>::operator ()(int i, int j)
{
    AVR_DBG_ASSERT( (unsigned)i < (unsigned)m && (unsigned)j < (unsigned)n );
    return val[i * n + j];
}


template<typename _Tp, int m, int n> inline
const _Tp& Matx<_Tp, m, n>::operator ()(int i) const
{
    AVR_DBG_ASSERT( (m == 1 || n == 1) && (unsigned)i < (unsigned)(m + n - 1) );
    return val[i];
}


template<typename _Tp, int m, int n> inline
_Tp& Matx<_Tp, m, n>::operator ()(int i)
{
    AVR_DBG_ASSERT( (m == 1 || n == 1) && (unsigned)i < (unsigned)(m + n - 1) );
    return val[i];
}

/// Adition
template<typename _Tp1, typename _Tp2, int m, int n> static inline
Matx<_Tp1, m, n>& operator += (Matx<_Tp1, m, n>& a, const Matx<_Tp2, m, n>& b)
{
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] + b.val[i]);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator + (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    Matx<_Tp, m, n> sum;
    for( int i = 0; i < m*n; i++ )
        sum.val[i] = saturate_cast<_Tp>(a.val[i] + b.val[i]);
    return sum;
}

/// Subtraction
template<typename _Tp1, typename _Tp2, int m, int n> static inline
Matx<_Tp1, m, n>& operator -= (Matx<_Tp1, m, n>& a, const Matx<_Tp2, m, n>& b)
{
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] - b.val[i]);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator - (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    Matx<_Tp, m, n> sub;
    for( int i = 0; i < m*n; i++ )
        sub.val[i] = saturate_cast<_Tp>(a.val[i] - b.val[i]);
    return sub;
}

/// Multiplication by scalar
template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n>& operator *= (Matx<_Tp, m, n>& a, int alpha)
{
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n>& operator *= (Matx<_Tp, m, n>& a, float alpha)
{
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n>& operator *= (Matx<_Tp, m, n>& a, double alpha)
{
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (const Matx<_Tp, m, n>& a, int alpha)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (const Matx<_Tp, m, n>& a, float alpha)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (const Matx<_Tp, m, n>& a, double alpha)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (int alpha, const Matx<_Tp, m, n>& a)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (float alpha, const Matx<_Tp, m, n>& a)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator * (double alpha, const Matx<_Tp, m, n>& a)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(a.val[i] * alpha);
    return mul;
}

/// Matrix Multiplication
template<typename _Tp, int m, int n, int l> static inline
Matx<_Tp, m, n> operator * (const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b)
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m; i++ ) {
        for( int j = 0; j < n; j++ ) {
            _Tp s = 0;
            for( int k = 0; k < l; k++ )
                s += a(i, k) * b(k, j);
            mul.val[i * n + j] = s;
        }
    }
    return mul;
}

/// Matrix-Vector Multiplication
template<typename _Tp, int m, int n> static inline
Vec<_Tp, m> operator * (const Matx<_Tp, m, n>& a, const Vec<_Tp, n>& b)
{
    Vec<_Tp, m> mul;
    for( int i = 0; i < m; i++ ) {
        _Tp s = 0;
        for( int j = 0; j < n; j++ )
            s += a(i, j) * b(j);
        mul.val[i] = s;
    }
    return mul;
}

/// Trasformation Matrix Multiplication
template<typename _Tp> static inline
Point_<_Tp> operator * (const Matx<_Tp, 2, 2>& a, const Point_<_Tp>& b)
{
    Vec<_Tp, 2> tmp = a * Vec<_Tp, 2>(b.x, b.y);
    return Point_<_Tp>(tmp.val[0], tmp.val[1]);
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Matx<_Tp, 3, 3>& a, const Point3_<_Tp>& b)
{
    Vec<_Tp, 3> tmp = a * Vec<_Tp, 3>(b.x, b.y, b.z);
    return Point3_<_Tp>(tmp.val[0], tmp.val[1], tmp.val[2]);
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Matx<_Tp, 3, 3>& a, const Point_<_Tp>& b)
{
    Vec<_Tp, 3> tmp = a * Vec<_Tp, 3>(b.x, b.y, 1);
    return Point3_<_Tp>(tmp.val[0], tmp.val[1], tmp.val[2]);
}

template<typename _Tp> static inline
Matx<_Tp, 4, 1> operator * (const Matx<_Tp, 4, 4>& a, const Point3_<_Tp>& b)
{
    return a * Matx<_Tp, 4, 1>(b.x, b.y, b.z, 1);
}

template<typename _Tp> static inline
Scalar operator * (const Matx<_Tp, 4, 4>& a, const Scalar& b)
{
    return static_cast<const Scalar&>(Matx<float, 4, 4>(a) * Matx<float, 4, 1>(b.val));
}

static inline
Scalar operator * (const Matx<float, 4, 4>& a, const Scalar& b)
{
    Matx<float, 4, 1> mul = a * Matx<float, 4, 1>(b.val);
    return Scalar(mul.val[0], mul.val[1], mul.val[2], mul.val[3]);
}

/// Negation
template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator - (const Matx<_Tp, m, n>& a)
{
    Matx<_Tp, m, n> neg;
    for( int i = 0; i < m*n; i++ )
        neg.val[i] = -saturate_cast<_Tp>(a.val[i]);
    return neg;
}

/// Per-element Matrix Multiplication
template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n> Matx<_Tp, m, n>::mul(const Matx<_Tp, m, n>& a) const
{
    Matx<_Tp, m, n> mul;
    for( int i = 0; i < m*n; i++ )
        mul.val[i] = saturate_cast<_Tp>(val[i] * a.val[i]);
    return mul;
}

template<typename _Tp, int m, int n> static inline
bool operator == (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    for( int i = 0; i < m*n; i++ )
        if( a.val[i] != b.val[i] ) return false;
    return true;
}

template<typename _Tp, int m, int n> static inline
bool operator != (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    return !(a == b);
}

/** Writes a Matx to an output stream.
 */
template <typename Tp, int r, int c> static inline std::ostream& operator << (std::ostream& out, Matx<Tp, r, c> m) {
   out << "[";
   for(int i = 0; i < r; i++) {
      if(i != 0) out << "\n";
      out << m(i, 0);
      for(int j = 1; j < c; j++)
         out << ", " << m(i, j);
   }
   return (out << "]");
}

template<typename _Tp, int m> struct Matx_DetOp
{
    double operator ()(const Matx<_Tp, m, m>& a) const
    {
        Matx<_Tp, m, m> temp = a;
        double p = LU(temp.val, m*sizeof(_Tp), m, 0, 0, 0);
        if( p == 0 )
            return p;
        for( int i = 0; i < m; i++ )
            p *= temp(i, i);
        return 1./p;
    }
};


template<typename _Tp> struct Matx_DetOp<_Tp, 1>
{
    double operator ()(const Matx<_Tp, 1, 1>& a) const
    {
        return a(0,0);
    }
};


template<typename _Tp> struct Matx_DetOp<_Tp, 2>
{
    double operator ()(const Matx<_Tp, 2, 2>& a) const
    {
        return a(0,0)*a(1,1) - a(0,1)*a(1,0);
    }
};


template<typename _Tp> struct Matx_DetOp<_Tp, 3>
{
    double operator ()(const Matx<_Tp, 3, 3>& a) const
    {
        return a(0,0)*(a(1,1)*a(2,2) - a(2,1)*a(1,2)) -
            a(0,1)*(a(1,0)*a(2,2) - a(2,0)*a(1,2)) +
            a(0,2)*(a(1,0)*a(2,1) - a(2,0)*a(1,1));
    }
};

template<typename _Tp, int m> static inline
double determinant(const Matx<_Tp, m, m>& a)
{
    return Matx_DetOp<_Tp, m>()(a);
}


template<typename _Tp, int m, int n> static inline
double trace(const Matx<_Tp, m, n>& a)
{
    _Tp s = 0;
    for( int i = 0; i < std::min(m, n); i++ )
        s += a(i,i);
    return s;
}


template<typename _Tp, int m, int n> inline
Matx<_Tp, n, m> Matx<_Tp, m, n>::t() const
{
    Matx<_Tp, n, m> transp;
    for( int i = 0; i < m; i++ )
        for( int j = 0; j < n; j++ )
            transp(j, i) = (*this)(i, j);
    return transp;
}


template<typename _Tp, int m> struct Matx_FastInvOp
{
    bool operator()(const Matx<_Tp, m, m>& a, Matx<_Tp, m, m>& b, int method) const
    {
        Matx<_Tp, m, m> temp = a;

        // assume that b is all 0's on input => make it a unity matrix
        for( int i = 0; i < m; i++ )
            b(i, i) = (_Tp)1;

        if( method == DECOMP_CHOLESKY )
            return cv::Cholesky(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m);

        return cv::LU(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m) != 0;
    }
};


template<typename _Tp> struct Matx_FastInvOp<_Tp, 2>
{
    bool operator()(const Matx<_Tp, 2, 2>& a, Matx<_Tp, 2, 2>& b, int) const
    {
        _Tp d = determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        b(1,1) = a(0,0)*d;
        b(0,0) = a(1,1)*d;
        b(0,1) = -a(0,1)*d;
        b(1,0) = -a(1,0)*d;
        return true;
    }
};


template<typename _Tp> struct Matx_FastInvOp<_Tp, 3>
{
    bool operator()(const Matx<_Tp, 3, 3>& a, Matx<_Tp, 3, 3>& b, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        b(0,0) = (a(1,1) * a(2,2) - a(1,2) * a(2,1)) * d;
        b(0,1) = (a(0,2) * a(2,1) - a(0,1) * a(2,2)) * d;
        b(0,2) = (a(0,1) * a(1,2) - a(0,2) * a(1,1)) * d;

        b(1,0) = (a(1,2) * a(2,0) - a(1,0) * a(2,2)) * d;
        b(1,1) = (a(0,0) * a(2,2) - a(0,2) * a(2,0)) * d;
        b(1,2) = (a(0,2) * a(1,0) - a(0,0) * a(1,2)) * d;

        b(2,0) = (a(1,0) * a(2,1) - a(1,1) * a(2,0)) * d;
        b(2,1) = (a(0,1) * a(2,0) - a(0,0) * a(2,1)) * d;
        b(2,2) = (a(0,0) * a(1,1) - a(0,1) * a(1,0)) * d;
        return true;
    }
};


template<typename _Tp, int m, int n> inline
Matx<_Tp, n, m> Matx<_Tp, m, n>::inv(int method) const
{
    Matx<_Tp, n, m> b;
    bool ok = false;
    if( method == DECOMP_LU || method == DECOMP_CHOLESKY )
        ok = Matx_FastInvOp<_Tp, m>()(*this, b, method);
    else
    {
        Mat A(*this, false), B(b, false);
        ok = (cv::invert(A, B, method) != 0);
    }
    return ok ? b : Matx<_Tp, n, m>::zeros();
}


template<typename _Tp, int m, int n> struct Matx_FastSolveOp
{
    bool operator()(const Matx<_Tp, m, m>& a, const Matx<_Tp, m, n>& b,
                    Matx<_Tp, m, n>& x, int method) const
    {
        Matx<_Tp, m, m> temp = a;
        x = b;
        if( method == DECOMP_CHOLESKY )
            return cv::Cholesky(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n);

        return cv::LU(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n) != 0;
    }
};


template<typename _Tp> struct Matx_FastSolveOp<_Tp, 2, 1>
{
    bool operator()(const Matx<_Tp, 2, 2>& a, const Matx<_Tp, 2, 1>& b,
                    Matx<_Tp, 2, 1>& x, int) const
    {
        _Tp d = determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        x(0) = (b(0)*a(1,1) - b(1)*a(0,1))*d;
        x(1) = (b(1)*a(0,0) - b(0)*a(1,0))*d;
        return true;
    }
};


template<typename _Tp> struct Matx_FastSolveOp<_Tp, 3, 1>
{
    bool operator()(const Matx<_Tp, 3, 3>& a, const Matx<_Tp, 3, 1>& b,
                    Matx<_Tp, 3, 1>& x, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        x(0) = d*(b(0)*(a(1,1)*a(2,2) - a(1,2)*a(2,1)) -
                a(0,1)*(b(1)*a(2,2) - a(1,2)*b(2)) +
                a(0,2)*(b(1)*a(2,1) - a(1,1)*b(2)));

        x(1) = d*(a(0,0)*(b(1)*a(2,2) - a(1,2)*b(2)) -
                b(0)*(a(1,0)*a(2,2) - a(1,2)*a(2,0)) +
                a(0,2)*(a(1,0)*b(2) - b(1)*a(2,0)));

        x(2) = d*(a(0,0)*(a(1,1)*b(2) - b(1)*a(2,1)) -
                a(0,1)*(a(1,0)*b(2) - b(1)*a(2,0)) +
                b(0)*(a(1,0)*a(2,1) - a(1,1)*a(2,0)));
        return true;
    }
};


template<typename _Tp, int m, int n> template<int l> inline
Matx<_Tp, n, l> Matx<_Tp, m, n>::solve(const Matx<_Tp, m, l>& rhs, int method) const
{
    Matx<_Tp, n, l> x;
    bool ok = false;
    if( method == DECOMP_LU || method == DECOMP_CHOLESKY )
        ok = Matx_FastSolveOp<_Tp, m, l>()(*this, rhs, x, method);
    else
    {
        Mat A(*this, false), B(rhs, false), X(x, false);
        ok = cv::solve(A, B, X, method);
    }

    return ok ? x : Matx<_Tp, n, l>::zeros();
}

template<typename _Tp, int m, int n> inline
Vec<_Tp, n> Matx<_Tp, m, n>::solve(const Vec<_Tp, m>& rhs, int method) const
{
    Matx<_Tp, n, 1> x = solve(reinterpret_cast<const Matx<_Tp, m, 1>&>(rhs), method);
    return reinterpret_cast<Vec<_Tp, n>&>(x);
}

/*-------------------- Norm function -----------------------*/
template<typename _Tp, int m, int n> static double norm(const Matx<_Tp, m, n>& M);
template<typename _Tp, int m, int n> static double norm(const Matx<_Tp, m, n>& M, int normType);

template<typename _Tp, typename _AccTp> static inline
_AccTp normL2Sqr(const _Tp* a, int n)
{
    _AccTp s = 0;
    int i=0;
 #if CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
    {
        _AccTp v0 = a[i], v1 = a[i+1], v2 = a[i+2], v3 = a[i+3];
        s += v0*v0 + v1*v1 + v2*v2 + v3*v3;
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = a[i];
        s += v*v;
    }
    return s;
}


template<typename _Tp, typename _AccTp> static inline
_AccTp normL1(const _Tp* a, int n)
{
    _AccTp s = 0;
    int i = 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        s += (_AccTp)fast_abs(a[i]) + (_AccTp)fast_abs(a[i+1]) +
            (_AccTp)fast_abs(a[i+2]) + (_AccTp)fast_abs(a[i+3]);
    }
#endif
    for( ; i < n; i++ )
        s += fast_abs(a[i]);
    return s;
}


template<typename _Tp, typename _AccTp> static inline
_AccTp normInf(const _Tp* a, int n)
{
    _AccTp s = 0;
    for( int i = 0; i < n; i++ )
        s = std::max(s, (_AccTp)fast_abs(a[i]));
    return s;
}


template<typename _Tp, typename _AccTp> static inline
_AccTp normL2Sqr(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    int i= 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        _AccTp v0 = _AccTp(a[i] - b[i]), v1 = _AccTp(a[i+1] - b[i+1]), v2 = _AccTp(a[i+2] - b[i+2]), v3 = _AccTp(a[i+3] - b[i+3]);
        s += v0*v0 + v1*v1 + v2*v2 + v3*v3;
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = _AccTp(a[i] - b[i]);
        s += v*v;
    }
    return s;
}

float normL2Sqr_(const float* a, const float* b, int n);
float normL1_(const float* a, const float* b, int n);
int normL1_(const uchar* a, const uchar* b, int n);
int normHamming(const uchar* a, const uchar* b, int n);
int normHamming(const uchar* a, const uchar* b, int n, int cellSize);

template<> inline float normL2Sqr(const float* a, const float* b, int n)
{
    if( n >= 8 )
        return normL2Sqr_(a, b, n);
    float s = 0;
    for( int i = 0; i < n; i++ )
    {
        float v = a[i] - b[i];
        s += v*v;
    }
    return s;
}


template<typename _Tp, typename _AccTp> static inline
_AccTp normL1(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    int i= 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        _AccTp v0 = _AccTp(a[i] - b[i]), v1 = _AccTp(a[i+1] - b[i+1]), v2 = _AccTp(a[i+2] - b[i+2]), v3 = _AccTp(a[i+3] - b[i+3]);
        s += std::abs(v0) + std::abs(v1) + std::abs(v2) + std::abs(v3);
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = _AccTp(a[i] - b[i]);
        s += std::abs(v);
    }
    return s;
}

template<> inline float normL1(const float* a, const float* b, int n)
{
    if( n >= 8 )
        return normL1_(a, b, n);
    float s = 0;
    for( int i = 0; i < n; i++ )
    {
        float v = a[i] - b[i];
        s += std::abs(v);
    }
    return s;
}

template<> inline int normL1(const uchar* a, const uchar* b, int n)
{
    return normL1_(a, b, n);
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normInf(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    for( int i = 0; i < n; i++ )
    {
        _AccTp v0 = a[i] - b[i];
        s = std::max(s, std::abs(v0));
    }
    return s;
}

} // namespace cv

template<typename _Tp, int m, int n> inline
double cv::norm(const Matx<_Tp, m, n>& M)
{
    return std::sqrt(normL2Sqr<_Tp, double>(M.val, m*n));
}


template<typename _Tp, int m, int n> inline
double cv::norm(const Matx<_Tp, m, n>& M, int normType)
{
    return normType == NORM_INF ? (double)normInf<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n) :
        normType == NORM_L1 ? (double)normL1<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n) :
        std::sqrt((double)normL2Sqr<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n));
}

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

/** Writes a Vec to an output stream. Format example : [10, 20, 30]
 */
template<typename _Tp, int n> inline std::ostream& operator<<(std::ostream& out, const Vec<_Tp, n>& vec)
{
    out << "[";

    if(Vec<_Tp, n>::depth < CV_32F)
    {
        for (int i = 0; i < n - 1; ++i) {
            out << (int)vec[i] << ", ";
        }
        out << (int)vec[n-1] << "]";
    }
    else
    {
        for (int i = 0; i < n - 1; ++i) {
            out << vec[i] << ", ";
        }
        out << vec[n-1] << "]";
    }

    return out;
}

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

// end operations.hpp

} // namespace cv

#endif // __cplusplus

#endif
