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

//This file implements the functions:
//    - cv::gemm
//    - cv::scaleAdd
//    - cv::mulTransposed

#include "precomp.hpp"

#ifdef HAVE_IPP
#include "ippversion.h"
#endif

using namespace std;

namespace cv
{
// begin matmul.cpp
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

static void
GEMM_CopyBlock( const uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    int j;
    size.width *= (int)(pix_size / sizeof(int));

    for( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #if CV_ENABLE_UNROLLED
        for( ; j <= size.width - 4; j += 4 )
        {
            int t0 = ((const int*)src)[j];
            int t1 = ((const int*)src)[j+1];
            ((int*)dst)[j] = t0;
            ((int*)dst)[j+1] = t1;
            t0 = ((const int*)src)[j+2];
            t1 = ((const int*)src)[j+3];
            ((int*)dst)[j+2] = t0;
            ((int*)dst)[j+3] = t1;
        }
        #endif
        for( ; j < size.width; j++ )
            ((int*)dst)[j] = ((const int*)src)[j];
    }
}


static void
GEMM_TransposeBlock( const uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    int i, j;
    for( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        const uchar* _src = src;
        switch( pix_size )
        {
        case sizeof(int):
            for( j = 0; j < size.height; j++, _src += src_step )
                ((int*)dst)[j] = ((int*)_src)[0];
            break;
        case sizeof(int)*2:
            for( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
            }
            break;
        case sizeof(int)*4:
            for( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
                t0 = ((int*)_src)[2];
                t1 = ((int*)_src)[3];
                ((int*)dst)[j+2] = t0;
                ((int*)dst)[j+3] = t1;
            }
            break;
        default:
            AVR_ERROR(Cod::FunctionArgument, "pix_size value is invalid");
            return;
        }
    }
}


template<typename T, typename WT> static void
GEMMSingleMul( const T* a_data, size_t a_step,
               const T* b_data, size_t b_step,
               const T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               double alpha, double beta, int flags )
{
    int i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    const T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    c_step /= sizeof(c_data[0]);
    d_step /= sizeof(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    if( !c_data )
        c_step0 = c_step1 = 0;
    else if( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    if( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        if( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf;
        }
    }

    if( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        if( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf;
            for( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        if( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf;
            for( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        for( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            for( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                if( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                else
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    else if( flags & GEMM_2_T ) /* A * Bt */
    {
        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0), s1(0), s2(0), s3(0);
                k = 0;
                 #if CV_ENABLE_UNROLLED
                for( ; k <= n - 4; k += 4 )
                {
                    s0 += WT(a_data[k])*WT(b_data[k]);
                    s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                    s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                }
                #endif
                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = (s0+s1+s2+s3)*alpha;

                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    else if( d_size.width*sizeof(d_data[0]) <= 1600 )
    {
        for( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                for( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                if( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                else
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            for( ; j < m; j++, c_data += c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0);

                for( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    else
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf;

        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            for( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
                 #if CV_ENABLE_UNROLLED
                for(; j <= m - 4; j += 4 )
                {
                    WT t0 = d_buf[j] + WT(b_data[j])*al;
                    WT t1 = d_buf[j+1] + WT(b_data[j+1])*al;
                    d_buf[j] = t0;
                    d_buf[j+1] = t1;
                    t0 = d_buf[j+2] + WT(b_data[j+2])*al;
                    t1 = d_buf[j+3] + WT(b_data[j+3])*al;
                    d_buf[j+2] = t0;
                    d_buf[j+3] = t1;
                }
                #endif
                for( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            if( !c_data )
                for( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            else
                for( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}


template<typename T, typename WT> static void
GEMMBlockMul( const T* a_data, size_t a_step,
              const T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, int flags )
{
    int i, j, k, n = a_size.width, m = d_size.width;
    const T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    int do_acc = flags & 16;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    d_step /= sizeof(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    if( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf;
    }

    if( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j]:WT(0), s1(0);
                for( k = 0; k <= n - 2; k += 2 )
                {
                    s0 += WT(a_data[k])*WT(b_data[k]);
                    s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                }

                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0 + s1;
            }
        }
    }
    else
    {
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j <= m - 4; j += 4 )
            {
                WT s0, s1, s2, s3;
                const T* b = b_data + j;

                if( do_acc )
                {
                    s0 = d_data[j]; s1 = d_data[j+1];
                    s2 = d_data[j+2]; s3 = d_data[j+3];
                }
                else
                    s0 = s1 = s2 = s3 = WT(0);

                for( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                d_data[j] = s0; d_data[j+1] = s1;
                d_data[j+2] = s2; d_data[j+3] = s3;
            }

            for( ; j < m; j++ )
            {
                const T* b = b_data + j;
                WT s0 = do_acc ? d_data[j] : WT(0);

                for( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                d_data[j] = s0;
            }
        }
    }
}


template<typename T, typename WT> static void
GEMMStore( const T* c_data, size_t c_step,
           const WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           double alpha, double beta, int flags )
{
    const T* _c_data = c_data;
    int j;
    size_t c_step0, c_step1;

    c_step /= sizeof(c_data[0]);
    d_buf_step /= sizeof(d_buf[0]);
    d_step /= sizeof(d_data[0]);

    if( !c_data )
        c_step0 = c_step1 = 0;
    else if( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    for( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        if( _c_data )
        {
            c_data = _c_data;
            j=0;
             #if CV_ENABLE_UNROLLED
            for(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_data[0]);
                t1 += beta*WT(c_data[c_step1]);
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_data[c_step1*2]);
                t1 += beta*WT(c_data[c_step1*3]);
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        else
        {
            j = 0;
             #if CV_ENABLE_UNROLLED
            for( ; j <= d_size.width - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            for( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


typedef void (*GEMMSingleMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, const void* src3, size_t step3,
                   void* dst, size_t dststep, Size srcsize, Size dstsize,
                   double alpha, double beta, int flags );

typedef void (*GEMMBlockMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size srcsize, Size dstsize, int flags );

typedef void (*GEMMStoreFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size dstsize, double alpha, double beta, int flags );

static void GEMMSingleMul_32f( const float* a_data, size_t a_step,
              const float* b_data, size_t b_step,
              const float* c_data, size_t c_step,
              float* d_data, size_t d_step,
              Size a_size, Size d_size,
              double alpha, double beta, int flags )
{
    GEMMSingleMul<float,double>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64f( const double* a_data, size_t a_step,
                              const double* b_data, size_t b_step,
                              const double* c_data, size_t c_step,
                              double* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<double,double>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


static void GEMMSingleMul_32fc( const Complexf* a_data, size_t a_step,
                              const Complexf* b_data, size_t b_step,
                              const Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64fc( const Complexd* a_data, size_t a_step,
                              const Complexd* b_data, size_t b_step,
                              const Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

static void GEMMBlockMul_32f( const float* a_data, size_t a_step,
             const float* b_data, size_t b_step,
             double* d_data, size_t d_step,
             Size a_size, Size d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64f( const double* a_data, size_t a_step,
                             const double* b_data, size_t b_step,
                             double* d_data, size_t d_step,
                             Size a_size, Size d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_32fc( const Complexf* a_data, size_t a_step,
                             const Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64fc( const Complexd* a_data, size_t a_step,
                             const Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMStore_32f( const float* c_data, size_t c_step,
          const double* d_buf, size_t d_buf_step,
          float* d_data, size_t d_step, Size d_size,
          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64f( const double* c_data, size_t c_step,
                      const double* d_buf, size_t d_buf_step,
                      double* d_data, size_t d_step, Size d_size,
                      double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_32fc( const Complexf* c_data, size_t c_step,
                          const Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64fc( const Complexd* c_data, size_t c_step,
                          const Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

}

void cv::gemm( InputArray matA, InputArray matB, double alpha,
           InputArray matC, double beta, OutputArray _matD, int flags )
{
    const int block_lin_size = 128;
    const int block_size = block_lin_size * block_lin_size;

    static double zero[] = {0,0,0,0};
    static float zerof[] = {0,0,0,0};

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0 ? matC.getMat() : Mat();
    Size a_size = A.size(), d_size;
    int i, len = 0, type = A.type();

    AVR_ASSERT( type == B.type() && (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    switch( flags & (GEMM_1_T|GEMM_2_T) )
    {
    case 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        AVR_ASSERT( a_size.width == len );
        break;
    case 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        AVR_ASSERT( a_size.height == len );
        break;
    case 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        AVR_ASSERT( a_size.width == len );
        break;
    case 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        AVR_ASSERT( a_size.height == len );
        break;
    }

    if( C.data )
    {
        AVR_ASSERT( C.type() == type &&
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    if( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    if( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        if( type == CV_32F )
        {
            float* d = (float*)D.data;
            const float *a = (const float*)A.data,
                        *b = (const float*)B.data,
                        *c = (const float*)C.data;
            size_t d_step = D.step/sizeof(d[0]),
                a_step = A.step/sizeof(a[0]),
                b_step = B.step/sizeof(b[0]),
                c_step = C.data ? C.step/sizeof(c[0]) : 0;

            if( !c )
                c = zerof;

            switch( len )
            {
            case 2:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                    }
                }
                else
                    break;
                return;
            case 3:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                else
                    break;
                return;
            case 4:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        float t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                        d[3] = (float)(t3*alpha + c[3]*beta);
                    }
                }
                else if( len <= 16 && a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        float t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (float)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                else
                    break;
                return;
            }
        }

        if( type == CV_64F )
        {
            double* d = (double*)D.data;
            const double *a = (const double*)A.data,
                         *b = (const double*)B.data,
                         *c = (const double*)C.data;
            size_t d_step = D.step/sizeof(d[0]),
                a_step = A.step/sizeof(a[0]),
                b_step = B.step/sizeof(b[0]),
                c_step = C.data ? C.step/sizeof(c[0]) : 0;
            if( !c )
                c = zero;

            switch( len )
            {
            case 2:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                else
                    break;
                return;
            case 3:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                else
                    break;
                return;
            case 4:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        double t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                else if( d_size.width <= 16 && a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        double t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                else
                    break;
                return;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D, tmat;
    int tmat_size = 0;
    const uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    if( type == CV_32FC1 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_32f;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_32f;
        storeFunc = (GEMMStoreFunc)GEMMStore_32f;
    }
    else if( type == CV_64FC1 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_64f;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_64f;
        storeFunc = (GEMMStoreFunc)GEMMStore_64f;
    }
    else if( type == CV_32FC2 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_32fc;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_32fc;
        storeFunc = (GEMMStoreFunc)GEMMStore_32fc;
    }
    else
    {
        AVR_ASSERT( type == CV_64FC2 );
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_64fc;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_64fc;
        storeFunc = (GEMMStoreFunc)GEMMStore_64fc;
    }

    if( D.data == A.data || D.data == B.data )
    {
        tmat_size = d_size.width*d_size.height*CV_ELEM_SIZE(type);
        // Allocate tmat later, once the size of buf is known
        matD = &tmat;
    }

    if( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_step = d_size.width == 1 ? 0 : CV_ELEM_SIZE(type);
        flags |= GEMM_2_T;
    }

    /*if( (d_size.width | d_size.height | len) >= 16 && icvBLAS_GEMM_32f_p != 0 )
    {
        blas_func = type == CV_32FC1 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_32f_p :
                    type == CV_64FC1 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_64f_p :
                    type == CV_32FC2 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_32fc_p :
                    type == CV_64FC2 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_64fc_p : 0;
    }

    if( blas_func )
    {
        const char* transa = flags & GEMM_1_T ? "t" : "n";
        const char* transb = flags & GEMM_2_T ? "t" : "n";
        int lda, ldb, ldd;

        if( C->data.ptr )
        {
            if( C->data.ptr != D->data.ptr )
            {
                if( !(flags & GEMM_3_T) )
                    cvCopy( C, D );
                else
                    cvTranspose( C, D );
            }
        }

        if( CV_MAT_DEPTH(type) == CV_32F )
        {
            Complex32f _alpha, _beta;

            lda = A->step/sizeof(float);
            ldb = b_step/sizeof(float);
            ldd = D->step/sizeof(float);
            _alpha.re = (float)alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? (float)beta : 0;
            _beta.im = 0;
            if( CV_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
        else
        {
            CvComplex64f _alpha, _beta;

            lda = A->step/sizeof(double);
            ldb = b_step/sizeof(double);
            ldd = D->step/sizeof(double);
            _alpha.re = alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? beta : 0;
            _beta.im = 0;
            if( CV_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
    }
    else*/ if( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        if( tmat_size > 0 ) {
            buf.allocate(tmat_size);
            tmat = Mat(d_size.height, d_size.width, type, (uchar*)buf );
        }
        singleMulFunc( A.data, A.step, B.data, b_step, Cdata, Cstep,
                       matD->data, matD->step, a_size, d_size, alpha, beta, flags );
    }
    else
    {
        int is_a_t = flags & GEMM_1_T;
        int is_b_t = flags & GEMM_2_T;
        int elem_size = CV_ELEM_SIZE(type);
        int dk0_1, dk0_2;
        int a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        int j, k, di = 0, dj = 0, dk = 0;
        int dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        int work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        if( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        else
            a_step0 = elem_size, a_step1 = A.step;

        if( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        else
            b_step0 = elem_size, b_step1 = b_step;

        if( !C.data )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        else if( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        else
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        if( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        if( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (dk0+dk0/8+1)*dk0_1*work_elem_size;

        if( is_a_t )
        {
            a_buf_size = (dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size + tmat_size);
        d_buf = (uchar*)buf;
        b_buf = d_buf + d_buf_size;

        if( is_a_t )
            a_buf = b_buf + b_buf_size;
        if( tmat_size > 0 )
            tmat = Mat(d_size.height, d_size.width, type, b_buf + b_buf_size + a_buf_size );

        for( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            if( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            for( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->data + i*matD->step + j*elem_size;
                const uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                if( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                if( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                for( k = 0; k < len; k += dk )
                {
                    const uchar* _a = A.data + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    const uchar* _b = B.data + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    if( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    if( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    else
                        a_bl_size.width = di, a_bl_size.height = dk;

                    if( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    if( dj < d_size.width )
                    {
                        Size b_size;
                        if( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        else
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    if( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    else
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                if( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->data + i*matD->step + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }

    if( matD != &D )
        matD->copyTo(D);
    }
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

namespace cv
{

static void scaleAdd_32f(const float* src1, const float* src2, float* dst,
                         int len, float* _alpha)
{
    float alpha = *_alpha;
    int i = 0;
#if CV_SSE2
    if( USE_SSE2 )
    {
        __m128 a4 = _mm_set1_ps(alpha);
        if( (((size_t)src1|(size_t)src2|(size_t)dst) & 15) == 0 )
            for( ; i <= len - 8; i += 8 )
            {
                __m128 x0, x1, y0, y1, t0, t1;
                x0 = _mm_load_ps(src1 + i); x1 = _mm_load_ps(src1 + i + 4);
                y0 = _mm_load_ps(src2 + i); y1 = _mm_load_ps(src2 + i + 4);
                t0 = _mm_add_ps(_mm_mul_ps(x0, a4), y0);
                t1 = _mm_add_ps(_mm_mul_ps(x1, a4), y1);
                _mm_store_ps(dst + i, t0);
                _mm_store_ps(dst + i + 4, t1);
            }
        else
            for( ; i <= len - 8; i += 8 )
            {
                __m128 x0, x1, y0, y1, t0, t1;
                x0 = _mm_loadu_ps(src1 + i); x1 = _mm_loadu_ps(src1 + i + 4);
                y0 = _mm_loadu_ps(src2 + i); y1 = _mm_loadu_ps(src2 + i + 4);
                t0 = _mm_add_ps(_mm_mul_ps(x0, a4), y0);
                t1 = _mm_add_ps(_mm_mul_ps(x1, a4), y1);
                _mm_storeu_ps(dst + i, t0);
                _mm_storeu_ps(dst + i + 4, t1);
            }
    }
    else
#endif
    //vz why do we need unroll here?
    for( ; i <= len - 4; i += 4 )
    {
        float t0, t1;
        t0 = src1[i]*alpha + src2[i];
        t1 = src1[i+1]*alpha + src2[i+1];
        dst[i] = t0; dst[i+1] = t1;
        t0 = src1[i+2]*alpha + src2[i+2];
        t1 = src1[i+3]*alpha + src2[i+3];
        dst[i+2] = t0; dst[i+3] = t1;
    }
    for(; i < len; i++ )
        dst[i] = src1[i]*alpha + src2[i];
}


static void scaleAdd_64f(const double* src1, const double* src2, double* dst,
                         int len, double* _alpha)
{
    double alpha = *_alpha;
    int i = 0;
#if CV_SSE2
    if( USE_SSE2 && (((size_t)src1|(size_t)src2|(size_t)dst) & 15) == 0 )
    {
        __m128d a2 = _mm_set1_pd(alpha);
        for( ; i <= len - 4; i += 4 )
        {
            __m128d x0, x1, y0, y1, t0, t1;
            x0 = _mm_load_pd(src1 + i); x1 = _mm_load_pd(src1 + i + 2);
            y0 = _mm_load_pd(src2 + i); y1 = _mm_load_pd(src2 + i + 2);
            t0 = _mm_add_pd(_mm_mul_pd(x0, a2), y0);
            t1 = _mm_add_pd(_mm_mul_pd(x1, a2), y1);
            _mm_store_pd(dst + i, t0);
            _mm_store_pd(dst + i + 2, t1);
        }
    }
    else
#endif
     //vz why do we need unroll here?
    for( ; i <= len - 4; i += 4 )
    {
        double t0, t1;
        t0 = src1[i]*alpha + src2[i];
        t1 = src1[i+1]*alpha + src2[i+1];
        dst[i] = t0; dst[i+1] = t1;
        t0 = src1[i+2]*alpha + src2[i+2];
        t1 = src1[i+3]*alpha + src2[i+3];
        dst[i+2] = t0; dst[i+3] = t1;
    }
    for(; i < len; i++ )
        dst[i] = src1[i]*alpha + src2[i];
}

typedef void (*ScaleAddFunc)(const uchar* src1, const uchar* src2, uchar* dst, int len, const void* alpha);

} // namespace cv

void cv::scaleAdd( InputArray _src1, double alpha, InputArray _src2, OutputArray _dst )
{
    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    int depth = src1.depth(), cn = src1.channels();

    AVR_ASSERT( src1.type() == src2.type() );
    if( depth < CV_32F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        return;
    }

    _dst.create(src1.dims, src1.size, src1.type());
    Mat dst = _dst.getMat();

    float falpha = (float)alpha;
    void* palpha = depth == CV_32F ? (void*)&falpha : (void*)&alpha;

    ScaleAddFunc func = depth == CV_32F ? (ScaleAddFunc)scaleAdd_32f : (ScaleAddFunc)scaleAdd_64f;

    if( src1.isContinuous() && src2.isContinuous() && dst.isContinuous() )
    {
        size_t len = src1.total()*cn;
        func(src1.data, src2.data, dst.data, (int)len, palpha);
        return;
    }

    const Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3];
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (int)len, palpha );
}

/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

namespace cv
{

template<typename sT, typename dT> static void
MulTransposedR( const Mat& srcmat, Mat& dstmat, const Mat& deltamat, double scale )
{
    int i, j, k;
    const sT* src = (const sT*)srcmat.data;
    dT* dst = (dT*)dstmat.data;
    const dT* delta = (const dT*)deltamat.data;
    size_t srcstep = srcmat.step/sizeof(src[0]);
    size_t dststep = dstmat.step/sizeof(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/sizeof(delta[0]) : 0;
    int delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    int buf_size = size.height*sizeof(dT);
    AutoBuffer<uchar> buf;

    if( delta && delta_cols < size.width )
    {
        assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)(uchar*)buf;

    if( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        for( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

    if( !delta )
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            for( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            for( j = i; j <= size.width - 4; j += 4 )
            {
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                const sT *tsrc = src + j;

                for( k = 0; k < size.height; k++, tsrc += srcstep )
                {
                    double a = col_buf[k];
                    s0 += a * tsrc[0];
                    s1 += a * tsrc[1];
                    s2 += a * tsrc[2];
                    s3 += a * tsrc[3];
                }

                tdst[j] = (dT)(s0*scale);
                tdst[j+1] = (dT)(s1*scale);
                tdst[j+2] = (dT)(s2*scale);
                tdst[j+3] = (dT)(s3*scale);
            }

            for( ; j < size.width; j++ )
            {
                double s0 = 0;
                const sT *tsrc = src + j;

                for( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (double)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    else
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            if( !delta_buf )
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            else
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            for( j = i; j <= size.width - 4; j += 4 )
            {
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                const sT *tsrc = src + j;
                const dT *d = delta_buf ? delta_buf : delta + j;

                for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                {
                    double a = col_buf[k];
                    s0 += a * (tsrc[0] - d[0]);
                    s1 += a * (tsrc[1] - d[1]);
                    s2 += a * (tsrc[2] - d[2]);
                    s3 += a * (tsrc[3] - d[3]);
                }

                tdst[j] = (dT)(s0*scale);
                tdst[j+1] = (dT)(s1*scale);
                tdst[j+2] = (dT)(s2*scale);
                tdst[j+3] = (dT)(s3*scale);
            }

            for( ; j < size.width; j++ )
            {
                double s0 = 0;
                const sT *tsrc = src + j;
                const dT *d = delta_buf ? delta_buf : delta + j;

                for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (double)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> static void
MulTransposedL( const Mat& srcmat, Mat& dstmat, const Mat& deltamat, double scale )
{
    int i, j, k;
    const sT* src = (const sT*)srcmat.data;
    dT* dst = (dT*)dstmat.data;
    const dT* delta = (const dT*)deltamat.data;
    size_t srcstep = srcmat.step/sizeof(src[0]);
    size_t dststep = dstmat.step/sizeof(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/sizeof(delta[0]) : 0;
    int delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    if( !delta )
        for( i = 0; i < size.height; i++, tdst += dststep )
            for( j = i; j < size.height; j++ )
            {
                double s = 0;
                const sT *tsrc1 = src + i*srcstep;
                const sT *tsrc2 = src + j*srcstep;

                for( k = 0; k <= size.width - 4; k += 4 )
                    s += (double)tsrc1[k]*tsrc2[k] + (double)tsrc1[k+1]*tsrc2[k+1] +
                         (double)tsrc1[k+2]*tsrc2[k+2] + (double)tsrc1[k+3]*tsrc2[k+3];
                for( ; k < size.width; k++ )
                    s += (double)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    else
    {
        dT delta_buf[4];
        int delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*sizeof(dT));
        dT* row_buf = (dT*)(uchar*)buf;

        for( i = 0; i < size.height; i++, tdst += dststep )
        {
            const sT *tsrc1 = src + i*srcstep;
            const dT *tdelta1 = delta + i*deltastep;

            if( delta_cols < size.width )
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            else
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            for( j = i; j < size.height; j++ )
            {
                double s = 0;
                const sT *tsrc2 = src + j*srcstep;
                const dT *tdelta2 = delta + j*deltastep;
                if( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
                for( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                    s += (double)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                         (double)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                         (double)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                         (double)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                for( ; k < size.width; k++, tdelta2++ )
                    s += (double)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

typedef void (*MulTransposedFunc)(const Mat& src, Mat& dst, const Mat& delta, double scale);

}

void cv::mulTransposed( InputArray _src, OutputArray _dst, bool ata, InputArray _delta, double scale, int dtype )
{
    Mat src = _src.getMat(), delta = _delta.getMat();
    const int gemm_level = 100; // boundary above which GEMM is faster.
    int stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    AVR_ASSERT( src.channels() == 1 );

    if( delta.data )
    {
        AVR_ASSERT( delta.channels() == 1 &&
            (delta.rows == src.rows || delta.rows == 1) &&
            (delta.cols == src.cols || delta.cols == 1));
        if( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    int dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    if( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        const Mat* tsrc = &src;
        if( delta.data )
        {
            if( delta.size() == src.size() )
                subtract( src, delta, src2 );
            else
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    else
    {
        MulTransposedFunc func = 0;
        if(stype == CV_8U && dtype == CV_32F)
        {
            if(ata)
                func = MulTransposedR<uchar,float>;
            else
                func = MulTransposedL<uchar,float>;
        }
        else if(stype == CV_8U && dtype == CV_64F)
        {
            if(ata)
                func = MulTransposedR<uchar,double>;
            else
                func = MulTransposedL<uchar,double>;
        }
        else if(stype == CV_16U && dtype == CV_32F)
        {
            if(ata)
                func = MulTransposedR<ushort,float>;
            else
                func = MulTransposedL<ushort,float>;
        }
        else if(stype == CV_16U && dtype == CV_64F)
        {
            if(ata)
                func = MulTransposedR<ushort,double>;
            else
                func = MulTransposedL<ushort,double>;
        }
        else if(stype == CV_16S && dtype == CV_32F)
        {
            if(ata)
                func = MulTransposedR<short,float>;
            else
                func = MulTransposedL<short,float>;
        }
        else if(stype == CV_16S && dtype == CV_64F)
        {
            if(ata)
                func = MulTransposedR<short,double>;
            else
                func = MulTransposedL<short,double>;
        }
        else if(stype == CV_32F && dtype == CV_32F)
        {
            if(ata)
                func = MulTransposedR<float,float>;
            else
                func = MulTransposedL<float,float>;
        }
        else if(stype == CV_32F && dtype == CV_64F)
        {
            if(ata)
                func = MulTransposedR<float,double>;
            else
                func = MulTransposedL<float,double>;
        }
        else if(stype == CV_64F && dtype == CV_64F)
        {
            if(ata)
                func = MulTransposedR<double,double>;
            else
                func = MulTransposedL<double,double>;
        }
        if( !func )
            AVR_ERROR(Cod::MatrixFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, false );
    }
// end matmul.cpp

} // namespace cv

/* End of file. */
