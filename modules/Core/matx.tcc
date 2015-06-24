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

#if !defined(AVR_CORE_STATIC_MATX_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_STATIC_MATX_TCC

#include "matrix.hpp"

#ifdef __cplusplus

namespace cv {

//////////////////////////////// Matx /////////////////////////////////

template<typename _Tp, int m, int n> inline Matx<_Tp, m, n>::Matx()
{
    for(int i = 0; i < channels; i++) val[i] = _Tp(0);
}

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
{
    return all(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::ones()
{
    return all(1);
}

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
            return hal::Cholesky(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m);

        return hal::LU(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m) != 0;
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
//    else
//    {
//        Mat A(*this, false), B(b, false);
//        ok = (invert(A, B, method) != 0);
//    }
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
            return hal::Cholesky(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n);

        return hal::LU(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n) != 0;
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
//    else
//    {
//        Mat A(*this, false), B(rhs, false), X(x, false);
//        ok = cv::solve(A, B, X, method);
//    }

    return ok ? x : Matx<_Tp, n, l>::zeros();
}

template<typename _Tp, int m, int n> inline
Vec<_Tp, n> Matx<_Tp, m, n>::solve(const Vec<_Tp, m>& rhs, int method) const
{
    Matx<_Tp, n, 1> x = solve(reinterpret_cast<const Matx<_Tp, m, 1>&>(rhs), method);
    return reinterpret_cast<Vec<_Tp, n>&>(x);
}

} // namespace cv

#endif // __cplusplus

#endif
