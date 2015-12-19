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

#include "precomp.hpp"
#include <climits>

namespace cv
{
// begin stat.cpp
/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

float normL2Sqr_(const float* a, const float* b, int n)
{
    int j = 0; float d = 0.f;
#if CV_SSE
    if( USE_SSE2 )
    {
        float CV_DECL_ALIGNED(16) buf[4];
        __m128 d0 = _mm_setzero_ps(), d1 = _mm_setzero_ps();

        for( ; j <= n - 8; j += 8 )
        {
            __m128 t0 = _mm_sub_ps(_mm_loadu_ps(a + j), _mm_loadu_ps(b + j));
            __m128 t1 = _mm_sub_ps(_mm_loadu_ps(a + j + 4), _mm_loadu_ps(b + j + 4));
            d0 = _mm_add_ps(d0, _mm_mul_ps(t0, t0));
            d1 = _mm_add_ps(d1, _mm_mul_ps(t1, t1));
        }
        _mm_store_ps(buf, _mm_add_ps(d0, d1));
        d = buf[0] + buf[1] + buf[2] + buf[3];
    }
    else
#endif
    {
        for( ; j <= n - 4; j += 4 )
        {
            float t0 = a[j] - b[j], t1 = a[j+1] - b[j+1], t2 = a[j+2] - b[j+2], t3 = a[j+3] - b[j+3];
            d += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        }
    }

    for( ; j < n; j++ )
    {
        float t = a[j] - b[j];
        d += t*t;
    }
    return d;
}


float normL1_(const float* a, const float* b, int n)
{
    int j = 0; float d = 0.f;
#if CV_SSE
    if( USE_SSE2 )
    {
        float CV_DECL_ALIGNED(16) buf[4];
        static const int CV_DECL_ALIGNED(16) absbuf[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};
        __m128 d0 = _mm_setzero_ps(), d1 = _mm_setzero_ps();
        __m128 absmask = _mm_load_ps((const float*)absbuf);

        for( ; j <= n - 8; j += 8 )
        {
            __m128 t0 = _mm_sub_ps(_mm_loadu_ps(a + j), _mm_loadu_ps(b + j));
            __m128 t1 = _mm_sub_ps(_mm_loadu_ps(a + j + 4), _mm_loadu_ps(b + j + 4));
            d0 = _mm_add_ps(d0, _mm_and_ps(t0, absmask));
            d1 = _mm_add_ps(d1, _mm_and_ps(t1, absmask));
        }
        _mm_store_ps(buf, _mm_add_ps(d0, d1));
        d = buf[0] + buf[1] + buf[2] + buf[3];
    }
    else
#endif
    {
        for( ; j <= n - 4; j += 4 )
        {
            d += std::abs(a[j] - b[j]) + std::abs(a[j+1] - b[j+1]) +
                    std::abs(a[j+2] - b[j+2]) + std::abs(a[j+3] - b[j+3]);
        }
    }

    for( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    return d;
}

int normL1_(const uchar* a, const uchar* b, int n)
{
    int j = 0, d = 0;
#if CV_SSE
    if( USE_SSE2 )
    {
        __m128i d0 = _mm_setzero_si128();

        for( ; j <= n - 16; j += 16 )
        {
            __m128i t0 = _mm_loadu_si128((const __m128i*)(a + j));
            __m128i t1 = _mm_loadu_si128((const __m128i*)(b + j));

            d0 = _mm_add_epi32(d0, _mm_sad_epu8(t0, t1));
        }

        for( ; j <= n - 4; j += 4 )
        {
            __m128i t0 = _mm_cvtsi32_si128(*(const int*)(a + j));
            __m128i t1 = _mm_cvtsi32_si128(*(const int*)(b + j));

            d0 = _mm_add_epi32(d0, _mm_sad_epu8(t0, t1));
        }
        d = _mm_cvtsi128_si32(_mm_add_epi32(d0, _mm_unpackhi_epi64(d0, d0)));
    }
    else
#endif
    {
        for( ; j <= n - 4; j += 4 )
        {
            d += std::abs(a[j] - b[j]) + std::abs(a[j+1] - b[j+1]) +
                    std::abs(a[j+2] - b[j+2]) + std::abs(a[j+3] - b[j+3]);
        }
    }
    for( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    return d;
}

static const uchar popCountTable[] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

static const uchar popCountTable2[] =
{
    0, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4
};

static const uchar popCountTable4[] =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

static int normHamming(const uchar* a, int n)
{
    int i = 0, result = 0;
#if CV_NEON
    uint32x4_t bits = vmovq_n_u32(0);
    for (; i <= n - 16; i += 16) {
        uint8x16_t A_vec = vld1q_u8 (a + i);
        uint8x16_t bitsSet = vcntq_u8 (A_vec);
        uint16x8_t bitSet8 = vpaddlq_u8 (bitsSet);
        uint32x4_t bitSet4 = vpaddlq_u16 (bitSet8);
        bits = vaddq_u32(bits, bitSet4);
    }
    uint64x2_t bitSet2 = vpaddlq_u32 (bits);
    result = vgetq_lane_s32 (vreinterpretq_s32_u64(bitSet2),0);
    result += vgetq_lane_s32 (vreinterpretq_s32_u64(bitSet2),2);
#else
    for( ; i <= n - 4; i += 4 )
            result += popCountTable[a[i]] + popCountTable[a[i+1]] +
            popCountTable[a[i+2]] + popCountTable[a[i+3]];
#endif
    for( ; i < n; i++ )
        result += popCountTable[a[i]];
    return result;
}

int normHamming(const uchar* a, const uchar* b, int n)
{
    int i = 0, result = 0;
#if CV_NEON
    uint32x4_t bits = vmovq_n_u32(0);
    for (; i <= n - 16; i += 16) {
        uint8x16_t A_vec = vld1q_u8 (a + i);
        uint8x16_t B_vec = vld1q_u8 (b + i);
        uint8x16_t AxorB = veorq_u8 (A_vec, B_vec);
        uint8x16_t bitsSet = vcntq_u8 (AxorB);
        uint16x8_t bitSet8 = vpaddlq_u8 (bitsSet);
        uint32x4_t bitSet4 = vpaddlq_u16 (bitSet8);
        bits = vaddq_u32(bits, bitSet4);
    }
    uint64x2_t bitSet2 = vpaddlq_u32 (bits);
    result = vgetq_lane_s32 (vreinterpretq_s32_u64(bitSet2),0);
    result += vgetq_lane_s32 (vreinterpretq_s32_u64(bitSet2),2);
#else
    for( ; i <= n - 4; i += 4 )
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
#endif
    for( ; i < n; i++ )
        result += popCountTable[a[i] ^ b[i]];
    return result;
}

static int normHamming(const uchar* a, int n, int cellSize)
{
    if( cellSize == 1 )
        return normHamming(a, n);
    const uchar* tab = 0;
    if( cellSize == 2 )
        tab = popCountTable2;
    else if( cellSize == 4 )
        tab = popCountTable4;
    else
        AVR_ERROR(Cod::FunctionArgument, "bad cell size (not 1, 2 or 4) in normHamming" );
    int i = 0, result = 0;
#if CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    for( ; i < n; i++ )
        result += tab[a[i]];
    return result;
}

int normHamming(const uchar* a, const uchar* b, int n, int cellSize)
{
    if( cellSize == 1 )
        return normHamming(a, b, n);
    const uchar* tab = 0;
    if( cellSize == 2 )
        tab = popCountTable2;
    else if( cellSize == 4 )
        tab = popCountTable4;
    else
        AVR_ERROR(Cod::FunctionArgument, "bad cell size (not 1, 2 or 4) in normHamming" );
    int i = 0, result = 0;
    #if CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
    #endif
    for( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    return result;
}


template<typename T, typename ST> int
normInf_(const T* src, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result = std::max(result, normInf<T, ST>(src, len*cn));
    }
    else
    {
        for( int i = 0; i < len; i++, src += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                    result = std::max(result, ST(fast_abs(src[k])));
            }
    }
    *_result = result;
    return 0;
}

template<typename T, typename ST> int
normL1_(const T* src, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result += normL1<T, ST>(src, len*cn);
    }
    else
    {
        for( int i = 0; i < len; i++, src += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                    result += fast_abs(src[k]);
            }
    }
    *_result = result;
    return 0;
}

template<typename T, typename ST> int
normL2_(const T* src, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result += normL2Sqr<T, ST>(src, len*cn);
    }
    else
    {
        for( int i = 0; i < len; i++, src += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                {
                    T v = src[k];
                    result += (ST)v*v;
                }
            }
    }
    *_result = result;
    return 0;
}

template<typename T, typename ST> int
normDiffInf_(const T* src1, const T* src2, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result = std::max(result, normInf<T, ST>(src1, src2, len*cn));
    }
    else
    {
        for( int i = 0; i < len; i++, src1 += cn, src2 += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                    result = std::max(result, (ST)std::abs(src1[k] - src2[k]));
            }
    }
    *_result = result;
    return 0;
}

template<typename T, typename ST> int
normDiffL1_(const T* src1, const T* src2, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result += normL1<T, ST>(src1, src2, len*cn);
    }
    else
    {
        for( int i = 0; i < len; i++, src1 += cn, src2 += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                    result += std::abs(src1[k] - src2[k]);
            }
    }
    *_result = result;
    return 0;
}

template<typename T, typename ST> int
normDiffL2_(const T* src1, const T* src2, const uchar* mask, ST* _result, int len, int cn)
{
    ST result = *_result;
    if( !mask )
    {
        result += normL2Sqr<T, ST>(src1, src2, len*cn);
    }
    else
    {
        for( int i = 0; i < len; i++, src1 += cn, src2 += cn )
            if( mask[i] )
            {
                for( int k = 0; k < cn; k++ )
                {
                    ST v = src1[k] - src2[k];
                    result += v*v;
                }
            }
    }
    *_result = result;
    return 0;
}


#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    static int norm##L##_##suffix(const type* src, const uchar* mask, ntype* r, int len, int cn) \
{ return norm##L##_(src, mask, r, len, cn); } \
    static int normDiff##L##_##suffix(const type* src1, const type* src2, \
    const uchar* mask, ntype* r, int len, int cn) \
{ return normDiff##L##_(src1, src2, mask, r, (int)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, int, int, int)
CV_DEF_NORM_ALL(8s, schar, int, int, int)
CV_DEF_NORM_ALL(16u, ushort, int, int, double)
CV_DEF_NORM_ALL(16s, short, int, int, double)
CV_DEF_NORM_ALL(32s, int, int, double, double)
CV_DEF_NORM_ALL(32f, float, float, double, double)
CV_DEF_NORM_ALL(64f, double, double, double, double)


typedef int (*NormFunc)(const uchar*, const uchar*, uchar*, int, int);
typedef int (*NormDiffFunc)(const uchar*, const uchar*, const uchar*, uchar*, int, int);

static NormFunc getNormFunc(int normType, int depth)
{
    static NormFunc normTab[3][8] =
    {
        {
            (NormFunc)GET_OPTIMIZED(normInf_8u), (NormFunc)GET_OPTIMIZED(normInf_8s), (NormFunc)GET_OPTIMIZED(normInf_16u), (NormFunc)GET_OPTIMIZED(normInf_16s),
            (NormFunc)GET_OPTIMIZED(normInf_32s), (NormFunc)GET_OPTIMIZED(normInf_32f), (NormFunc)normInf_64f, 0
        },
        {
            (NormFunc)GET_OPTIMIZED(normL1_8u), (NormFunc)GET_OPTIMIZED(normL1_8s), (NormFunc)GET_OPTIMIZED(normL1_16u), (NormFunc)GET_OPTIMIZED(normL1_16s),
            (NormFunc)GET_OPTIMIZED(normL1_32s), (NormFunc)GET_OPTIMIZED(normL1_32f), (NormFunc)normL1_64f, 0
        },
        {
            (NormFunc)GET_OPTIMIZED(normL2_8u), (NormFunc)GET_OPTIMIZED(normL2_8s), (NormFunc)GET_OPTIMIZED(normL2_16u), (NormFunc)GET_OPTIMIZED(normL2_16s),
            (NormFunc)GET_OPTIMIZED(normL2_32s), (NormFunc)GET_OPTIMIZED(normL2_32f), (NormFunc)normL2_64f, 0
        }
    };

    return normTab[normType][depth];
}

static NormDiffFunc getNormDiffFunc(int normType, int depth)
{
    static NormDiffFunc normDiffTab[3][8] =
    {
        {
            (NormDiffFunc)GET_OPTIMIZED(normDiffInf_8u), (NormDiffFunc)normDiffInf_8s,
            (NormDiffFunc)normDiffInf_16u, (NormDiffFunc)normDiffInf_16s,
            (NormDiffFunc)normDiffInf_32s, (NormDiffFunc)GET_OPTIMIZED(normDiffInf_32f),
            (NormDiffFunc)normDiffInf_64f, 0
        },
        {
            (NormDiffFunc)GET_OPTIMIZED(normDiffL1_8u), (NormDiffFunc)normDiffL1_8s,
            (NormDiffFunc)normDiffL1_16u, (NormDiffFunc)normDiffL1_16s,
            (NormDiffFunc)normDiffL1_32s, (NormDiffFunc)GET_OPTIMIZED(normDiffL1_32f),
            (NormDiffFunc)normDiffL1_64f, 0
        },
        {
            (NormDiffFunc)GET_OPTIMIZED(normDiffL2_8u), (NormDiffFunc)normDiffL2_8s,
            (NormDiffFunc)normDiffL2_16u, (NormDiffFunc)normDiffL2_16s,
            (NormDiffFunc)normDiffL2_32s, (NormDiffFunc)GET_OPTIMIZED(normDiffL2_32f),
            (NormDiffFunc)normDiffL2_64f, 0
        }
    };

    return normDiffTab[normType][depth];
}

} // namespace cv

double cv::norm( InputArray _src, int normType, InputArray _mask )
{
    Mat src = _src.getMat(), mask = _mask.getMat();
    int depth = src.depth(), cn = src.channels();

    normType &= 7;
    AVR_ASSERT( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src.type() == CV_8U) );

#if defined (HAVE_IPP) && (IPP_VERSION_MAJOR >= 7)
    size_t total_size = src.total();
    int rows = src.size[0], cols = (int)(total_size/rows);
    if( (src.dims == 2 || (src.isContinuous() && mask.isContinuous()))
        && cols > 0 && (size_t)rows*cols == total_size
        && (normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) )
    {
        IppiSize sz = { cols, rows };
        int type = src.type();
        if( !mask.empty() )
        {
            typedef IppStatus (CV_STDCALL* ippiMaskNormFuncC1)(const void *, int, const void *, int, IppiSize, Ipp64f *);
            ippiMaskNormFuncC1 ippFuncC1 =
                normType == NORM_INF ?
                (type == CV_8UC1 ? (ippiMaskNormFuncC1)ippiNorm_Inf_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormFuncC1)ippiNorm_Inf_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormFuncC1)ippiNorm_Inf_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormFuncC1)ippiNorm_Inf_32f_C1MR :
                0) :
            normType == NORM_L1 ?
                (type == CV_8UC1 ? (ippiMaskNormFuncC1)ippiNorm_L1_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormFuncC1)ippiNorm_L1_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormFuncC1)ippiNorm_L1_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormFuncC1)ippiNorm_L1_32f_C1MR :
                0) :
            normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC1 ? (ippiMaskNormFuncC1)ippiNorm_L2_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormFuncC1)ippiNorm_L2_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormFuncC1)ippiNorm_L2_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormFuncC1)ippiNorm_L2_32f_C1MR :
                0) : 0;
            if( ippFuncC1 )
            {
                Ipp64f norm;
                if( ippFuncC1(src.data, (int)src.step[0], mask.data, (int)mask.step[0], sz, &norm) >= 0 )
                {
                    return normType == NORM_L2SQR ? (double)(norm * norm) : (double)norm;
                }
            }
            typedef IppStatus (CV_STDCALL* ippiMaskNormFuncC3)(const void *, int, const void *, int, IppiSize, int, Ipp64f *);
            ippiMaskNormFuncC3 ippFuncC3 =
                normType == NORM_INF ?
                (type == CV_8UC3 ? (ippiMaskNormFuncC3)ippiNorm_Inf_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormFuncC3)ippiNorm_Inf_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormFuncC3)ippiNorm_Inf_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormFuncC3)ippiNorm_Inf_32f_C3CMR :
                0) :
            normType == NORM_L1 ?
                (type == CV_8UC3 ? (ippiMaskNormFuncC3)ippiNorm_L1_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormFuncC3)ippiNorm_L1_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormFuncC3)ippiNorm_L1_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormFuncC3)ippiNorm_L1_32f_C3CMR :
                0) :
            normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC3 ? (ippiMaskNormFuncC3)ippiNorm_L2_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormFuncC3)ippiNorm_L2_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormFuncC3)ippiNorm_L2_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormFuncC3)ippiNorm_L2_32f_C3CMR :
                0) : 0;
            if( ippFuncC3 )
            {
                Ipp64f norm1, norm2, norm3;
                if( ippFuncC3(src.data, (int)src.step[0], mask.data, (int)mask.step[0], sz, 1, &norm1) >= 0 &&
                    ippFuncC3(src.data, (int)src.step[0], mask.data, (int)mask.step[0], sz, 2, &norm2) >= 0 &&
                    ippFuncC3(src.data, (int)src.step[0], mask.data, (int)mask.step[0], sz, 3, &norm3) >= 0)
                {
                    Ipp64f norm =
                        normType == NORM_INF ? std::max(std::max(norm1, norm2), norm3) :
                        normType == NORM_L1 ? norm1 + norm2 + norm3 :
                        normType == NORM_L2 || normType == NORM_L2SQR ? std::sqrt(norm1 * norm1 + norm2 * norm2 + norm3 * norm3) :
                        0;
                    return normType == NORM_L2SQR ? (double)(norm * norm) : (double)norm;
                }
            }
        }
        else
        {
            typedef IppStatus (CV_STDCALL* ippiNormFunc)(const void *, int, IppiSize, Ipp64f *, IppHintAlgorithm hint);
            ippiNormFunc ippFunc =
                normType == NORM_INF ?
                (type == CV_8UC1 ? (ippiNormFunc)ippiNorm_Inf_8u_C1R :
                type == CV_8UC3 ? (ippiNormFunc)ippiNorm_Inf_8u_C3R :
                type == CV_8UC4 ? (ippiNormFunc)ippiNorm_Inf_8u_C4R :
                type == CV_16UC1 ? (ippiNormFunc)ippiNorm_Inf_16u_C1R :
                type == CV_16UC3 ? (ippiNormFunc)ippiNorm_Inf_16u_C3R :
                type == CV_16UC4 ? (ippiNormFunc)ippiNorm_Inf_16u_C4R :
                type == CV_16SC1 ? (ippiNormFunc)ippiNorm_Inf_16s_C1R :
                //type == CV_16SC3 ? (ippiNormFunc)ippiNorm_Inf_16s_C3R : //Aug 2013: problem in IPP 7.1, 8.0 : -32768
                //type == CV_16SC4 ? (ippiNormFunc)ippiNorm_Inf_16s_C4R : //Aug 2013: problem in IPP 7.1, 8.0 : -32768
                type == CV_32FC1 ? (ippiNormFunc)ippiNorm_Inf_32f_C1R :
                type == CV_32FC3 ? (ippiNormFunc)ippiNorm_Inf_32f_C3R :
                type == CV_32FC4 ? (ippiNormFunc)ippiNorm_Inf_32f_C4R :
                0) :
                normType == NORM_L1 ?
                (type == CV_8UC1 ? (ippiNormFunc)ippiNorm_L1_8u_C1R :
                type == CV_8UC3 ? (ippiNormFunc)ippiNorm_L1_8u_C3R :
                type == CV_8UC4 ? (ippiNormFunc)ippiNorm_L1_8u_C4R :
                type == CV_16UC1 ? (ippiNormFunc)ippiNorm_L1_16u_C1R :
                type == CV_16UC3 ? (ippiNormFunc)ippiNorm_L1_16u_C3R :
                type == CV_16UC4 ? (ippiNormFunc)ippiNorm_L1_16u_C4R :
                type == CV_16SC1 ? (ippiNormFunc)ippiNorm_L1_16s_C1R :
                type == CV_16SC3 ? (ippiNormFunc)ippiNorm_L1_16s_C3R :
                type == CV_16SC4 ? (ippiNormFunc)ippiNorm_L1_16s_C4R :
                type == CV_32FC1 ? (ippiNormFunc)ippiNorm_L1_32f_C1R :
                type == CV_32FC3 ? (ippiNormFunc)ippiNorm_L1_32f_C3R :
                type == CV_32FC4 ? (ippiNormFunc)ippiNorm_L1_32f_C4R :
                0) :
                normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC1 ? (ippiNormFunc)ippiNorm_L2_8u_C1R :
                type == CV_8UC3 ? (ippiNormFunc)ippiNorm_L2_8u_C3R :
                type == CV_8UC4 ? (ippiNormFunc)ippiNorm_L2_8u_C4R :
                type == CV_16UC1 ? (ippiNormFunc)ippiNorm_L2_16u_C1R :
                type == CV_16UC3 ? (ippiNormFunc)ippiNorm_L2_16u_C3R :
                type == CV_16UC4 ? (ippiNormFunc)ippiNorm_L2_16u_C4R :
                type == CV_16SC1 ? (ippiNormFunc)ippiNorm_L2_16s_C1R :
                type == CV_16SC3 ? (ippiNormFunc)ippiNorm_L2_16s_C3R :
                type == CV_16SC4 ? (ippiNormFunc)ippiNorm_L2_16s_C4R :
                type == CV_32FC1 ? (ippiNormFunc)ippiNorm_L2_32f_C1R :
                type == CV_32FC3 ? (ippiNormFunc)ippiNorm_L2_32f_C3R :
                type == CV_32FC4 ? (ippiNormFunc)ippiNorm_L2_32f_C4R :
                0) : 0;
            if( ippFunc )
            {
                Ipp64f norm_array[4];
                if( ippFunc(src.data, (int)src.step[0], sz, norm_array, ippAlgHintAccurate) >= 0 )
                {
                    Ipp64f norm = (normType == NORM_L2 || normType == NORM_L2SQR) ? norm_array[0] * norm_array[0] : norm_array[0];
                    for( int i = 1; i < cn; i++ )
                    {
                        norm =
                            normType == NORM_INF ? std::max(norm, norm_array[i]) :
                            normType == NORM_L1 ? norm + norm_array[i] :
                            normType == NORM_L2 || normType == NORM_L2SQR ? norm + norm_array[i] * norm_array[i] :
                            0;
                    }
                    return normType == NORM_L2 ? (double)std::sqrt(norm) : (double)norm;
                }
            }
        }
    }
#endif

    if( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        if( len == (size_t)(int)len )
        {
            if( depth == CV_32F )
            {
                const float* data = src.ptr<float>();

                if( normType == NORM_L2 )
                {
                    double result = 0;
                    GET_OPTIMIZED(normL2_32f)(data, 0, &result, (int)len, 1);
                    return std::sqrt(result);
                }
                if( normType == NORM_L2SQR )
                {
                    double result = 0;
                    GET_OPTIMIZED(normL2_32f)(data, 0, &result, (int)len, 1);
                    return result;
                }
                if( normType == NORM_L1 )
                {
                    double result = 0;
                    GET_OPTIMIZED(normL1_32f)(data, 0, &result, (int)len, 1);
                    return result;
                }
                if( normType == NORM_INF )
                {
                    float result = 0;
                    GET_OPTIMIZED(normInf_32f)(data, 0, &result, (int)len, 1);
                    return result;
                }
            }
            if( depth == CV_8U )
            {
                const uchar* data = src.ptr<uchar>();

                if( normType == NORM_HAMMING )
                    return normHamming(data, (int)len);

                if( normType == NORM_HAMMING2 )
                    return normHamming(data, (int)len, 2);
            }
        }
    }

    AVR_ASSERT( mask.empty() || mask.type() == CV_8U );

    if( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        if( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            return norm(temp, normType);
        }
        int cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src, 0};
        uchar* ptrs[1];
        NAryMatIterator it(arrays, ptrs);
        int total = (int)it.size;
        int result = 0;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            result += normHamming(ptrs[0], total, cellSize);

        return result;
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    AVR_ASSERT( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2];
    union
    {
        double d;
        int i;
        float f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    int j, total = (int)it.size, blockSize = total, intSumBlockSize = 0, count = 0;
    bool blockSum = (normType == NORM_L1 && depth <= CV_16S) ||
            ((normType == NORM_L2 || normType == NORM_L2SQR) && depth <= CV_8S);
    int isum = 0;
    int *ibuf = &result.i;
    size_t esz = 0;

    if( blockSum )
    {
        intSumBlockSize = (normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        blockSize = std::min(blockSize, intSumBlockSize);
        ibuf = &isum;
        esz = src.elemSize();
    }

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( j = 0; j < total; j += blockSize )
        {
            int bsz = std::min(total - j, blockSize);
            func( ptrs[0], ptrs[1], (uchar*)ibuf, bsz, cn );
            count += bsz;
            if( blockSum && (count + blockSize >= intSumBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                result.d += isum;
                isum = 0;
                count = 0;
            }
            ptrs[0] += bsz*esz;
            if( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    if( normType == NORM_INF )
    {
        if( depth == CV_64F )
            ;
        else if( depth == CV_32F )
            result.d = result.f;
        else
            result.d = result.i;
    }
    else if( normType == NORM_L2 )
        result.d = std::sqrt(result.d);

    return result.d;
}


double cv::norm( InputArray _src1, InputArray _src2, int normType, InputArray _mask )
{
    if( normType & CV_RELATIVE )
    {
#if defined (HAVE_IPP) && (IPP_VERSION_MAJOR >= 7)
        Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();

        AVR_ASSERT( src1.size == src2.size && src1.type() == src2.type() );

        normType &= 7;
        AVR_ASSERT( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR ||
                ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );
        size_t total_size = src1.total();
        int rows = src1.size[0], cols = (int)(total_size/rows);
        if( (src1.dims == 2 || (src1.isContinuous() && src2.isContinuous() && mask.isContinuous()))
            && cols > 0 && (size_t)rows*cols == total_size
            && (normType == NORM_INF || normType == NORM_L1 ||
                normType == NORM_L2 || normType == NORM_L2SQR) )
        {
            IppiSize sz = { cols, rows };
            int type = src1.type();
            if( !mask.empty() )
            {
                typedef IppStatus (CV_STDCALL* ippiMaskNormRelFuncC1)(const void *, int, const void *, int, const void *, int, IppiSize, Ipp64f *);
                ippiMaskNormRelFuncC1 ippFuncC1 =
                    normType == NORM_INF ?
                    (type == CV_8UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_Inf_8u_C1MR :
                    type == CV_8SC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_Inf_8s_C1MR :
                    type == CV_16UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_Inf_16u_C1MR :
                    type == CV_32FC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_Inf_32f_C1MR :
                    0) :
                    normType == NORM_L1 ?
                    (type == CV_8UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L1_8u_C1MR :
                    type == CV_8SC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L1_8s_C1MR :
                    type == CV_16UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L1_16u_C1MR :
                    type == CV_32FC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L1_32f_C1MR :
                    0) :
                    normType == NORM_L2 || normType == NORM_L2SQR ?
                    (type == CV_8UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L2_8u_C1MR :
                    type == CV_8SC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L2_8s_C1MR :
                    type == CV_16UC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L2_16u_C1MR :
                    type == CV_32FC1 ? (ippiMaskNormRelFuncC1)ippiNormRel_L2_32f_C1MR :
                    0) : 0;
                if( ippFuncC1 )
                {
                    Ipp64f norm;
                    if( ippFuncC1(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], mask.data, (int)mask.step[0], sz, &norm) >= 0 )
                        return normType == NORM_L2SQR ? (double)(norm * norm) : (double)norm;
                }
            }
            else
            {
                typedef IppStatus (CV_STDCALL* ippiNormRelFunc)(const void *, int, const void *, int, IppiSize, Ipp64f *, IppHintAlgorithm hint);
                ippiNormRelFunc ippFunc =
                    normType == NORM_INF ?
                    (type == CV_8UC1 ? (ippiNormRelFunc)ippiNormRel_Inf_8u_C1R :
                    type == CV_16UC1 ? (ippiNormRelFunc)ippiNormRel_Inf_16u_C1R :
                    type == CV_16SC1 ? (ippiNormRelFunc)ippiNormRel_Inf_16s_C1R :
                    type == CV_32FC1 ? (ippiNormRelFunc)ippiNormRel_Inf_32f_C1R :
                    0) :
                    normType == NORM_L1 ?
                    (type == CV_8UC1 ? (ippiNormRelFunc)ippiNormRel_L1_8u_C1R :
                    type == CV_16UC1 ? (ippiNormRelFunc)ippiNormRel_L1_16u_C1R :
                    type == CV_16SC1 ? (ippiNormRelFunc)ippiNormRel_L1_16s_C1R :
                    type == CV_32FC1 ? (ippiNormRelFunc)ippiNormRel_L1_32f_C1R :
                    0) :
                    normType == NORM_L2 || normType == NORM_L2SQR ?
                    (type == CV_8UC1 ? (ippiNormRelFunc)ippiNormRel_L2_8u_C1R :
                    type == CV_16UC1 ? (ippiNormRelFunc)ippiNormRel_L2_16u_C1R :
                    type == CV_16SC1 ? (ippiNormRelFunc)ippiNormRel_L2_16s_C1R :
                    type == CV_32FC1 ? (ippiNormRelFunc)ippiNormRel_L2_32f_C1R :
                    0) : 0;
                if( ippFunc )
                {
                    Ipp64f norm;
                    if( ippFunc(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], sz, &norm, ippAlgHintAccurate) >= 0 )
                        return (double)norm;
                }
            }
        }
#endif
        return norm(_src1, _src2, normType & ~CV_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    int depth = src1.depth(), cn = src1.channels();

    AVR_ASSERT( src1.size == src2.size && src1.type() == src2.type() );

    normType &= 7;
    AVR_ASSERT( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

#if defined (HAVE_IPP) && (IPP_VERSION_MAJOR >= 7)
    size_t total_size = src1.total();
    int rows = src1.size[0], cols = (int)(total_size/rows);
    if( (src1.dims == 2 || (src1.isContinuous() && src2.isContinuous() && mask.isContinuous()))
        && cols > 0 && (size_t)rows*cols == total_size
        && (normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) )
    {
        IppiSize sz = { cols, rows };
        int type = src1.type();
        if( !mask.empty() )
        {
            typedef IppStatus (CV_STDCALL* ippiMaskNormDiffFuncC1)(const void *, int, const void *, int, const void *, int, IppiSize, Ipp64f *);
            ippiMaskNormDiffFuncC1 ippFuncC1 =
                normType == NORM_INF ?
                (type == CV_8UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_Inf_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_Inf_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_Inf_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_Inf_32f_C1MR :
                0) :
                normType == NORM_L1 ?
                (type == CV_8UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L1_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L1_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L1_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L1_32f_C1MR :
                0) :
                normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L2_8u_C1MR :
                type == CV_8SC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L2_8s_C1MR :
                type == CV_16UC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L2_16u_C1MR :
                type == CV_32FC1 ? (ippiMaskNormDiffFuncC1)ippiNormDiff_L2_32f_C1MR :
                0) : 0;
            if( ippFuncC1 )
            {
                Ipp64f norm;
                if( ippFuncC1(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], mask.data, (int)mask.step[0], sz, &norm) >= 0 )
                    return normType == NORM_L2SQR ? (double)(norm * norm) : (double)norm;
            }
            typedef IppStatus (CV_STDCALL* ippiMaskNormDiffFuncC3)(const void *, int, const void *, int, const void *, int, IppiSize, int, Ipp64f *);
            ippiMaskNormDiffFuncC3 ippFuncC3 =
                normType == NORM_INF ?
                (type == CV_8UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_Inf_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_Inf_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_Inf_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_Inf_32f_C3CMR :
                0) :
                normType == NORM_L1 ?
                (type == CV_8UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L1_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L1_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L1_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L1_32f_C3CMR :
                0) :
                normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L2_8u_C3CMR :
                type == CV_8SC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L2_8s_C3CMR :
                type == CV_16UC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L2_16u_C3CMR :
                type == CV_32FC3 ? (ippiMaskNormDiffFuncC3)ippiNormDiff_L2_32f_C3CMR :
                0) : 0;
            if( ippFuncC3 )
            {
                Ipp64f norm1, norm2, norm3;
                if( ippFuncC3(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], mask.data, (int)mask.step[0], sz, 1, &norm1) >= 0 &&
                    ippFuncC3(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], mask.data, (int)mask.step[0], sz, 2, &norm2) >= 0 &&
                    ippFuncC3(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], mask.data, (int)mask.step[0], sz, 3, &norm3) >= 0)
                {
                    Ipp64f norm =
                        normType == NORM_INF ? std::max(std::max(norm1, norm2), norm3) :
                        normType == NORM_L1 ? norm1 + norm2 + norm3 :
                        normType == NORM_L2 || normType == NORM_L2SQR ? std::sqrt(norm1 * norm1 + norm2 * norm2 + norm3 * norm3) :
                        0;
                    return normType == NORM_L2SQR ? (double)(norm * norm) : (double)norm;
                }
            }
        }
        else
        {
            typedef IppStatus (CV_STDCALL* ippiNormDiffFunc)(const void *, int, const void *, int, IppiSize, Ipp64f *, IppHintAlgorithm hint);
            ippiNormDiffFunc ippFunc =
                normType == NORM_INF ?
                (type == CV_8UC1 ? (ippiNormDiffFunc)ippiNormDiff_Inf_8u_C1R :
                type == CV_8UC3 ? (ippiNormDiffFunc)ippiNormDiff_Inf_8u_C3R :
                type == CV_8UC4 ? (ippiNormDiffFunc)ippiNormDiff_Inf_8u_C4R :
                type == CV_16UC1 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16u_C1R :
                type == CV_16UC3 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16u_C3R :
                type == CV_16UC4 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16u_C4R :
                type == CV_16SC1 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16s_C1R :
                //type == CV_16SC3 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16s_C3R : //Aug 2013: problem in IPP 7.1, 8.0 : -32768
                //type == CV_16SC4 ? (ippiNormDiffFunc)ippiNormDiff_Inf_16s_C4R : //Aug 2013: problem in IPP 7.1, 8.0 : -32768
                type == CV_32FC1 ? (ippiNormDiffFunc)ippiNormDiff_Inf_32f_C1R :
                type == CV_32FC3 ? (ippiNormDiffFunc)ippiNormDiff_Inf_32f_C3R :
                type == CV_32FC4 ? (ippiNormDiffFunc)ippiNormDiff_Inf_32f_C4R :
                0) :
                normType == NORM_L1 ?
                (type == CV_8UC1 ? (ippiNormDiffFunc)ippiNormDiff_L1_8u_C1R :
                type == CV_8UC3 ? (ippiNormDiffFunc)ippiNormDiff_L1_8u_C3R :
                type == CV_8UC4 ? (ippiNormDiffFunc)ippiNormDiff_L1_8u_C4R :
                type == CV_16UC1 ? (ippiNormDiffFunc)ippiNormDiff_L1_16u_C1R :
                type == CV_16UC3 ? (ippiNormDiffFunc)ippiNormDiff_L1_16u_C3R :
                type == CV_16UC4 ? (ippiNormDiffFunc)ippiNormDiff_L1_16u_C4R :
                type == CV_16SC1 ? (ippiNormDiffFunc)ippiNormDiff_L1_16s_C1R :
                type == CV_16SC3 ? (ippiNormDiffFunc)ippiNormDiff_L1_16s_C3R :
                type == CV_16SC4 ? (ippiNormDiffFunc)ippiNormDiff_L1_16s_C4R :
                type == CV_32FC1 ? (ippiNormDiffFunc)ippiNormDiff_L1_32f_C1R :
                type == CV_32FC3 ? (ippiNormDiffFunc)ippiNormDiff_L1_32f_C3R :
                type == CV_32FC4 ? (ippiNormDiffFunc)ippiNormDiff_L1_32f_C4R :
                0) :
                normType == NORM_L2 || normType == NORM_L2SQR ?
                (type == CV_8UC1 ? (ippiNormDiffFunc)ippiNormDiff_L2_8u_C1R :
                type == CV_8UC3 ? (ippiNormDiffFunc)ippiNormDiff_L2_8u_C3R :
                type == CV_8UC4 ? (ippiNormDiffFunc)ippiNormDiff_L2_8u_C4R :
                type == CV_16UC1 ? (ippiNormDiffFunc)ippiNormDiff_L2_16u_C1R :
                type == CV_16UC3 ? (ippiNormDiffFunc)ippiNormDiff_L2_16u_C3R :
                type == CV_16UC4 ? (ippiNormDiffFunc)ippiNormDiff_L2_16u_C4R :
                type == CV_16SC1 ? (ippiNormDiffFunc)ippiNormDiff_L2_16s_C1R :
                type == CV_16SC3 ? (ippiNormDiffFunc)ippiNormDiff_L2_16s_C3R :
                type == CV_16SC4 ? (ippiNormDiffFunc)ippiNormDiff_L2_16s_C4R :
                type == CV_32FC1 ? (ippiNormDiffFunc)ippiNormDiff_L2_32f_C1R :
                type == CV_32FC3 ? (ippiNormDiffFunc)ippiNormDiff_L2_32f_C3R :
                type == CV_32FC4 ? (ippiNormDiffFunc)ippiNormDiff_L2_32f_C4R :
                0) : 0;
            if( ippFunc )
            {
                Ipp64f norm_array[4];
                if( ippFunc(src1.data, (int)src1.step[0], src2.data, (int)src2.step[0], sz, norm_array, ippAlgHintAccurate) >= 0 )
                {
                    Ipp64f norm = (normType == NORM_L2 || normType == NORM_L2SQR) ? norm_array[0] * norm_array[0] : norm_array[0];
                    for( int i = 1; i < src1.channels(); i++ )
                    {
                        norm =
                            normType == NORM_INF ? std::max(norm, norm_array[i]) :
                            normType == NORM_L1 ? norm + norm_array[i] :
                            normType == NORM_L2 || normType == NORM_L2SQR ? norm + norm_array[i] * norm_array[i] :
                            0;
                    }
                    return normType == NORM_L2 ? (double)std::sqrt(norm) : (double)norm;
                }
            }
        }
    }
#endif

    if( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        if( len == (size_t)(int)len )
        {
            if( src1.depth() == CV_32F )
            {
                const float* data1 = src1.ptr<float>();
                const float* data2 = src2.ptr<float>();

                if( normType == NORM_L2 )
                {
                    double result = 0;
                    GET_OPTIMIZED(normDiffL2_32f)(data1, data2, 0, &result, (int)len, 1);
                    return std::sqrt(result);
                }
                if( normType == NORM_L2SQR )
                {
                    double result = 0;
                    GET_OPTIMIZED(normDiffL2_32f)(data1, data2, 0, &result, (int)len, 1);
                    return result;
                }
                if( normType == NORM_L1 )
                {
                    double result = 0;
                    GET_OPTIMIZED(normDiffL1_32f)(data1, data2, 0, &result, (int)len, 1);
                    return result;
                }
                if( normType == NORM_INF )
                {
                    float result = 0;
                    GET_OPTIMIZED(normDiffInf_32f)(data1, data2, 0, &result, (int)len, 1);
                    return result;
                }
            }
        }
    }

    AVR_ASSERT( mask.empty() || mask.type() == CV_8U );

    if( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        if( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            return norm(temp, normType);
        }
        int cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs);
        int total = (int)it.size;
        int result = 0;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            result += normHamming(ptrs[0], ptrs[1], total, cellSize);

        return result;
    }

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    AVR_ASSERT( func != 0 );

    const Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3];
    union
    {
        double d;
        float f;
        int i;
        unsigned u;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    int j, total = (int)it.size, blockSize = total, intSumBlockSize = 0, count = 0;
    bool blockSum = (normType == NORM_L1 && depth <= CV_16S) ||
            ((normType == NORM_L2 || normType == NORM_L2SQR) && depth <= CV_8S);
    unsigned isum = 0;
    unsigned *ibuf = &result.u;
    size_t esz = 0;

    if( blockSum )
    {
        intSumBlockSize = normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, intSumBlockSize);
        ibuf = &isum;
        esz = src1.elemSize();
    }

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( j = 0; j < total; j += blockSize )
        {
            int bsz = std::min(total - j, blockSize);
            func( ptrs[0], ptrs[1], ptrs[2], (uchar*)ibuf, bsz, cn );
            count += bsz;
            if( blockSum && (count + blockSize >= intSumBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                result.d += isum;
                isum = 0;
                count = 0;
            }
            ptrs[0] += bsz*esz;
            ptrs[1] += bsz*esz;
            if( ptrs[2] )
                ptrs[2] += bsz;
        }
    }

    if( normType == NORM_INF )
    {
        if( depth == CV_64F )
            ;
        else if( depth == CV_32F )
            result.d = result.f;
        else
            result.d = result.u;
    }
    else if( normType == NORM_L2 )
        result.d = std::sqrt(result.d);

    return result.d;
}

/****************************************************************************************\
*                                       minMaxLoc                                        *
\****************************************************************************************/

namespace cv
{

template<typename T, typename WT> static void
minMaxIdx_( const T* src, const uchar* mask, WT* _minVal, WT* _maxVal, size_t* _minIdx, size_t* _maxIdx, int len, size_t startIdx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;

    if( !mask )
    {
        for( int i = 0; i < len; i++ )
        {
            T val = src[i];
            if( val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            if( val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }
    else
    {
        for( int i = 0; i < len; i++ )
        {
            T val = src[i];
            if( mask[i] && val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            if( mask[i] && val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

static void minMaxIdx_8u(const uchar* src, const uchar* mask, int* minval, int* maxval,
                         size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_8s(const schar* src, const uchar* mask, int* minval, int* maxval,
                         size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_16u(const ushort* src, const uchar* mask, int* minval, int* maxval,
                          size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_16s(const short* src, const uchar* mask, int* minval, int* maxval,
                          size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_32s(const int* src, const uchar* mask, int* minval, int* maxval,
                          size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_32f(const float* src, const uchar* mask, float* minval, float* maxval,
                          size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

static void minMaxIdx_64f(const double* src, const uchar* mask, double* minval, double* maxval,
                          size_t* minidx, size_t* maxidx, int len, size_t startidx )
{ minMaxIdx_(src, mask, minval, maxval, minidx, maxidx, len, startidx ); }

typedef void (*MinMaxIdxFunc)(const uchar*, const uchar*, int*, int*, size_t*, size_t*, int, size_t);

static MinMaxIdxFunc getMinmaxTab(int depth)
{
    static MinMaxIdxFunc minmaxTab[] =
    {
        (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_8u), (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_8s),
        (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_16u), (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_16s),
        (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_32s),
        (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_32f), (MinMaxIdxFunc)GET_OPTIMIZED(minMaxIdx_64f),
        0
    };

    return minmaxTab[depth];
}

static void ofs2idx(const Mat& a, size_t ofs, int* idx)
{
    int i, d = a.dims;
    if( ofs > 0 )
    {
        ofs--;
        for( i = d-1; i >= 0; i-- )
        {
            int sz = a.size[i];
            idx[i] = (int)(ofs % sz);
            ofs /= sz;
        }
    }
    else
    {
        for( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

} // namespace cv

void cv::minMaxIdx(InputArray _src, double* minVal, double* maxVal, int* minIdx, int* maxIdx, InputArray _mask)
{
    Mat src = _src.getMat(), mask = _mask.getMat();
    int depth = src.depth(), cn = src.channels();

    AVR_ASSERT( (cn == 1 && (mask.empty() || mask.type() == CV_8U)) ||
               (cn >= 1 && mask.empty() && !minIdx && !maxIdx) );

#if defined (HAVE_IPP) && (IPP_VERSION_MAJOR >= 7)
    size_t total_size = src.total();
    int rows = src.size[0], cols = (int)(total_size/rows);
    if( cn == 1 && ( src.dims == 2 || (src.isContinuous() && mask.isContinuous() && cols > 0 && (size_t)rows*cols == total_size) ) )
    {
        IppiSize sz = { cols, rows };
        int type = src.type();
        if( !mask.empty() )
        {
            typedef IppStatus (CV_STDCALL* ippiMaskMinMaxIndxFuncC1)(const void *, int, const void *, int, IppiSize, Ipp32f *, Ipp32f *, IppiPoint *, IppiPoint *);
            ippiMaskMinMaxIndxFuncC1 ippFuncC1 =
            type == CV_8UC1 ? (ippiMaskMinMaxIndxFuncC1)ippiMinMaxIndx_8u_C1MR :
            type == CV_16UC1 ? (ippiMaskMinMaxIndxFuncC1)ippiMinMaxIndx_16u_C1MR :
            type == CV_32FC1 ? (ippiMaskMinMaxIndxFuncC1)ippiMinMaxIndx_32f_C1MR :
            0;
            if( ippFuncC1 )
            {
                Ipp32f min, max;
                IppiPoint minp, maxp;
                if( ippFuncC1(src.data, (int)src.step[0], mask.data, (int)mask.step[0], sz, &min, &max, &minp, &maxp) >= 0 )
                {
                    if( minVal )
                        *minVal = (double)min;
                    if( maxVal )
                        *maxVal = (double)max;
                    if( !minp.x && !minp.y && !maxp.x && !maxp.y && !mask.data[0] )
                        minp.x = maxp.x = -1;
                    if( minIdx )
                    {
                        size_t minidx = minp.y * cols + minp.x + 1;
                        ofs2idx(src, minidx, minIdx);
                    }
                    if( maxIdx )
                    {
                        size_t maxidx = maxp.y * cols + maxp.x + 1;
                        ofs2idx(src, maxidx, maxIdx);
                    }
                    return;
                }
            }
        }
        else
        {
            typedef IppStatus (CV_STDCALL* ippiMinMaxIndxFuncC1)(const void *, int, IppiSize, Ipp32f *, Ipp32f *, IppiPoint *, IppiPoint *);
            ippiMinMaxIndxFuncC1 ippFuncC1 =
                type == CV_8UC1 ? (ippiMinMaxIndxFuncC1)ippiMinMaxIndx_8u_C1R :
                type == CV_16UC1 ? (ippiMinMaxIndxFuncC1)ippiMinMaxIndx_16u_C1R :
                type == CV_32FC1 ? (ippiMinMaxIndxFuncC1)ippiMinMaxIndx_32f_C1R :
                0;
            if( ippFuncC1 )
            {
                Ipp32f min, max;
                IppiPoint minp, maxp;
                if( ippFuncC1(src.data, (int)src.step[0], sz, &min, &max, &minp, &maxp) >= 0 )
                {
                    if( minVal )
                        *minVal = (double)min;
                    if( maxVal )
                        *maxVal = (double)max;
                    if( minIdx )
                    {
                        size_t minidx = minp.y * cols + minp.x + 1;
                        ofs2idx(src, minidx, minIdx);
                    }
                    if( maxIdx )
                    {
                        size_t maxidx = maxp.y * cols + maxp.x + 1;
                        ofs2idx(src, maxidx, maxIdx);
                    }
                    return;
                }
            }
        }
    }
#endif

    MinMaxIdxFunc func = getMinmaxTab(depth);
    AVR_ASSERT( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    int iminval = INT_MAX, imaxval = INT_MIN;
    float fminval = FLT_MAX, fmaxval = -FLT_MAX;
    double dminval = DBL_MAX, dmaxval = -DBL_MAX;
    size_t startidx = 1;
    int *minval = &iminval, *maxval = &imaxval;
    int planeSize = (int)it.size*cn;

    if( depth == CV_32F )
        minval = (int*)&fminval, maxval = (int*)&fmaxval;
    else if( depth == CV_64F )
        minval = (int*)&dminval, maxval = (int*)&dmaxval;

    for( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], minval, maxval, &minidx, &maxidx, planeSize, startidx );

    if( minidx == 0 )
        dminval = dmaxval = 0;
    else if( depth == CV_32F )
        dminval = fminval, dmaxval = fmaxval;
    else if( depth <= CV_32S )
        dminval = iminval, dmaxval = imaxval;

    if( minVal )
        *minVal = dminval;
    if( maxVal )
        *maxVal = dmaxval;

    if( minIdx )
        ofs2idx(src, minidx, minIdx);
    if( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxLoc( InputArray _img, double* minVal, double* maxVal, Point* minLoc, Point* maxLoc, InputArray mask )
{
    Mat img = _img.getMat();
    AVR_ASSERT(img.dims <= 2);

    minMaxIdx(_img, minVal, maxVal, (int*)minLoc, (int*)maxLoc, mask);
    if( minLoc )
        std::swap(minLoc->x, minLoc->y);
    if( maxLoc )
        std::swap(maxLoc->x, maxLoc->y);
}
// end stat.cpp

// begin convert.cpp
void cv::normalize( InputArray _src, OutputArray _dst, double a, double b, int norm_type, int rtype, InputArray _mask )
{
    Mat src = _src.getMat(), mask = _mask.getMat();

    double scale = 1, shift = 0;
    if( norm_type == CV_MINMAX )
    {
        double smin = 0, smax = 0;
        double dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxLoc( _src, &smin, &smax, 0, 0, mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        shift = dmin - smin*scale;
    }
    else if( norm_type == CV_L2 || norm_type == CV_L1 || norm_type == CV_C )
    {
        scale = norm( src, norm_type, mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    else
        AVR_ERROR(Cod::FunctionArgument, "Unknown/unsupported norm type" );

    if( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : src.depth();

    _dst.create(src.dims, src.size, CV_MAKETYPE(rtype, src.channels()));
    Mat dst = _dst.getMat();

    if( !mask.data )
        src.convertTo( dst, rtype, scale, shift );
    else
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( dst, mask );
    }
}
// end convert.cpp

/* End of file. */
