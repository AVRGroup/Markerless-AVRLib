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

#if !defined(AVR_CORE_FUNCTIONS_HPP) && !defined(__OPENCV_CORE_HPP__)
#define AVR_CORE_FUNCTIONS_HPP

#ifndef SKIP_INCLUDES
   #include <string.h>
   #include <limits.h>
#endif // SKIP_INCLUDES

#ifdef __cplusplus

#include <cmath>
#include <limits>

// avrlib's extended funcionalities
namespace avr {
   const double _PI     = 4.0 * std::atan(1.0);
   const double _2PI    = 2.0 * _PI;
   const double _PI2    = _PI / 2.0;
   const double _PI4    = _PI / 4.0;
   const double _TORAD  = _PI / 180.0;
   const double _TODEG  = 180.0 / _PI;

  inline double degrees(double rad) { return rad * _TODEG; }
  inline double radians(double deg) { return deg * _TORAD; }
}

namespace cv {

using avr::_PI;
using avr::_2PI;
using avr::_PI2;
using avr::_PI4;
using avr::degrees;
using avr::radians;

// File: alloc.cpp
/*!
  Allocates memory buffer

  This is specialized OpenCV memory allocation function that returns properly aligned memory buffers.
  The usage is identical to malloc(). The allocated buffers must be freed with cv::fastFree().
  If there is not enough memory, the function calls cv::error(), which raises an exception.

  \param bufSize buffer size in bytes
  \return the allocated memory buffer.
*/
void* fastMalloc( size_t size );
/*!
  Frees the memory allocated with cv::fastMalloc

  This is the corresponding deallocation function for cv::fastMalloc().
  When ptr==NULL, the function has no effect.
*/
void fastFree(void* ptr);

template<typename _Tp> static inline _Tp* allocate(size_t n)
{ return new _Tp[n]; }

template<typename _Tp> static inline void deallocate(_Tp* ptr, size_t)
{ delete[] ptr; }

/*!
  Aligns pointer by the certain number of bytes

  This small inline function aligns the pointer by the certian number of bytes by shifting
  it forward by 0 or a positive offset.
*/
template<typename _Tp> static inline _Tp* alignPtr(_Tp* ptr, int n=(int)sizeof(_Tp))
{ return (_Tp*)(((size_t)ptr + n-1) & -n); }

/*!
  Aligns buffer size by the certain number of bytes

  This small inline function aligns a buffer size by the certian number of bytes by enlarging it.
*/
static inline size_t alignSize(size_t sz, int n) {
    AVR_ASSERT((n & (n - 1)) == 0); // n is a power of 2
    return (sz + n-1) & -n;
}

// begin core.hpp
typedef void (*BinaryFunc)(const uchar* src1, size_t step1,
                           const uchar* src2, size_t step2,
                           uchar* dst, size_t step, Size sz,
                           void*);

BinaryFunc getConvertFunc(int sdepth, int ddepth);      // File: convert.cpp
BinaryFunc getConvertScaleFunc(int sdepth, int ddepth); // File: convert.cpp
BinaryFunc getCopyMaskFunc(size_t esz);                 // File: copy.cpp

// File: arithm.cpp
//! computes bitwise conjunction of the two arrays (dst = src1 & src2)
void bitwise_and(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray());
//! computes bitwise disjunction of the two arrays (dst = src1 | src2)
void bitwise_or(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray());
//! computes bitwise exclusive-or of the two arrays (dst = src1 ^ src2)
void bitwise_xor(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray());
//! inverts each bit of array (dst = ~src)
void bitwise_not(InputArray src, OutputArray dst, InputArray mask=noArray());
//! computes per-element minimum of two arrays (dst = min(src1, src2))
void min(InputArray src1, InputArray src2, OutputArray dst);
//! computes per-element maximum of two arrays (dst = max(src1, src2))
void max(InputArray src1, InputArray src2, OutputArray dst);
//! computes per-element minimum of two arrays (dst = min(src1, src2))
void min(const Mat& src1, const Mat& src2, Mat& dst);
//! computes per-element minimum of array and scalar (dst = min(src1, src2))
void min(const Mat& src1, double src2, Mat& dst);
//! computes per-element maximum of two arrays (dst = max(src1, src2))
void max(const Mat& src1, const Mat& src2, Mat& dst);
//! computes per-element maximum of array and scalar (dst = max(src1, src2))
void max(const Mat& src1, double src2, Mat& dst);
//! adds one matrix to another (dst = src1 + src2)
void add(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray(), int dtype=-1);
//! subtracts one matrix from another (dst = src1 - src2)
void subtract(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray(), int dtype=-1);
//! computes element-wise absolute difference of two arrays (dst = abs(src1 - src2))
void absdiff(InputArray src1, InputArray src2, OutputArray dst);
//! computes element-wise weighted product of the two arrays (dst = scale*src1*src2)
void multiply(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1);
//! computes element-wise weighted quotient of the two arrays (dst = scale*src1/src2)
void divide(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1);
//! computes element-wise weighted reciprocal of an array (dst = scale/src2)
void divide(double scale, InputArray src2, OutputArray dst, int dtype=-1);
//! computes weighted sum of two arrays (dst = alpha*src1 + beta*src2 + gamma)
void addWeighted(InputArray src1, double alpha, InputArray src2, double beta, double gamma, OutputArray dst, int dtype=-1);
//! compares elements of two arrays (dst = src1 <cmpop> src2)
void compare(InputArray src1, InputArray src2, OutputArray dst, int cmpop);

// File: copy.cpp
//! replicates the input matrix the specified number of times in the horizontal and/or vertical direction
void repeat(InputArray src, int ny, int nx, OutputArray dst);
Mat repeat(const Mat& src, int ny, int nx);

// File: lapack.cpp
//! computes determinant of a square matrix
double determinant(InputArray mtx);
//! computes inverse or pseudo-inverse matrix
double invert(InputArray src, OutputArray dst, int flags=DECOMP_LU);
//! solves linear system or a least-square problem
bool solve(InputArray src1, InputArray src2, OutputArray dst, int flags=DECOMP_LU);
//! finds eigenvalues of a symmetric matrix
bool eigen(InputArray src, OutputArray eigenvalues, int lowindex=-1, int highindex=-1);
//! finds eigenvalues and eigenvectors of a symmetric matrix
bool eigen(InputArray src, OutputArray eigenvalues, OutputArray eigenvectors, int lowindex=-1, int highindex=-1);
bool eigen(InputArray src, bool computeEigenvectors, OutputArray eigenvalues, OutputArray eigenvectors);

// File: matmul.cpp
//! implements generalized matrix product algorithm GEMM from BLAS
void gemm(InputArray src1, InputArray src2, double alpha, InputArray src3, double beta, OutputArray dst, int flags=0);
//! adds scaled array to another one (dst = alpha*src1 + src2)
void scaleAdd(InputArray src1, double alpha, InputArray src2, OutputArray dst);
//! multiplies matrix by its transposition from the left or from the right
void mulTransposed( InputArray src, OutputArray dst, bool aTa, InputArray delta=noArray(), double scale=1, int dtype=-1 );

// File: matrix.xpp
//! swaps two matrices
void swap(Mat& a, Mat& b);
//! converts Scalar object to c-array
void scalarToRawData(const Scalar& s, void* buf, int type, int unroll_to=0);
//! initializes scaled identity matrix
void setIdentity(InputOutputArray mtx, const Scalar& s=Scalar(1));
//! transposes the matrix
void transpose(InputArray src, OutputArray dst);
//! extends the symmetrical matrix from the lower half or from the upper half
void completeSymm(InputOutputArray mtx, bool lowerToUpper=false);
//! @enum sort flags
enum { SORT_EVERY_ROW=0, SORT_EVERY_COLUMN=1, SORT_ASCENDING=0, SORT_DESCENDING=16 };
//! sorts independently each matrix row or each matrix column
void sort(InputArray src, OutputArray dst, int flags);
//! sorts independently each matrix row or each matrix column
void sortIdx(InputArray src, OutputArray dst, int flags);

// File: norm.cpp
//! computes norm of the selected array part
double norm(InputArray src1, int normType=NORM_L2, InputArray mask=noArray());
//! computes norm of selected part of the difference between two arrays
double norm(InputArray src1, InputArray src2, int normType=NORM_L2, InputArray mask=noArray());
//! finds global minimum and maximum array elements and returns their values and their locations
void minMaxLoc(InputArray src, double* minVal, double* maxVal=0, Point* minLoc=0, Point* maxLoc=0, InputArray mask=noArray());
void minMaxIdx(InputArray src, double* minVal, double* maxVal, int* minIdx=0, int* maxIdx=0, InputArray mask=noArray());
//! scales and shifts array elements so that either the specified norm (alpha) or the minimum (alpha) and maximum (beta) array values get the specified values
void normalize( InputArray src, OutputArray dst, double alpha=1, double beta=0, int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray());

// end core.hpp

// begin operations.hpp
/////////////// saturate_cast (used in image & signal processing) ///////////////////
template<typename _Tp> static inline _Tp saturate_cast(unsigned char v)    { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(signed char v)      { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(unsigned short v)   { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(short v)            { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(unsigned v)         { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(int v)              { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(float v)            { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(double v)           { return _Tp(v); }

inline int fast_abs(unsigned char v)   { return v; }
inline int fast_abs(signed char v)     { return std::abs((int)v); }
inline int fast_abs(unsigned short v)  { return v; }
inline int fast_abs(short v)           { return std::abs((int)v); }
inline int fast_abs(int v)             { return std::abs(v); }
inline float fast_abs(float v)         { return std::abs(v); }
inline double fast_abs(double v)       { return std::abs(v); }

/* ----------- LU & Cholesky decomposition for small matrices ----------- */
int LU(float* A, size_t astep, int m, float* b, size_t bstep, int n);
int LU(double* A, size_t astep, int m, double* b, size_t bstep, int n);
bool Cholesky(float* A, size_t astep, int m, float* b, size_t bstep, int n);
bool Cholesky(double* A, size_t astep, int m, double* b, size_t bstep, int n);
// end operations.hpp

} // namespace cv

#endif // __cplusplus

#endif
