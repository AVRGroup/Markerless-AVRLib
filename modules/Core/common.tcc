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

#if !defined(AVR_CORE_COMMON_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_COMMON_TCC

#ifndef SKIP_INCLUDES
   #include <string.h>
   #include <limits.h>
#endif // SKIP_INCLUDES

#ifdef __cplusplus

#include <cmath>
#include <limits>

namespace avr {
   const double _PI     = 4.0 * std::atan(1.0);
   const double _2PI    = 2.0 * _PI;
   const double _PI2    = _PI / 2.0;
   const double _PI4    = _PI / 4.0;
   const double _TORAD  = _PI / 180.0;
   const double _TODEG  = 180.0 / _PI;

   #if __cplusplus > 199711L // C++11
      inline constexpr double degrees(double rad) { return rad * _TODEG; }
      inline constexpr double radians(double deg) { return deg * _TORAD; }
   #else
      inline double degrees(double rad) { return rad * _TODEG; }
      inline double radians(double deg) { return deg * _TORAD; }
   #endif // __cplusplus
}

namespace cv {

using avr::_PI;
using avr::_2PI;
using avr::_PI2;
using avr::_PI4;
using avr::degrees;
using avr::radians;

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

//////////////////////////////// Size ////////////////////////////////

template<typename _Tp> inline Size_<_Tp>::Size_() : width(0), height(0) {}
template<typename _Tp> inline Size_<_Tp>::Size_(_Tp _width, _Tp _height) : width(_width), height(_height) {}
template<typename _Tp> inline Size_<_Tp>::Size_(const Size_& sz) : width(sz.width), height(sz.height) {}
template<typename _Tp> inline Size_<_Tp>::Size_(const Point_<_Tp>& pt) : width(pt.x), height(pt.y) {}

template<typename _Tp> inline _Tp Size_<_Tp>::area() const { return width*height; }

template<typename _Tp> template<typename _Tp2> inline Size_<_Tp>::operator Size_<_Tp2>() const
{ return Size_<_Tp2>(saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height)); }

template<typename _Tp> inline Size_<_Tp>& Size_<_Tp>::operator = (const Size_<_Tp>& sz)
{ width = sz.width; height = sz.height; return *this; }
template<typename _Tp> static inline Size_<_Tp>& operator += (Size_<_Tp>& a, const Size_<_Tp>& b)
{ a.width += b.width; a.height += b.height; return a; }
template<typename _Tp> static inline Size_<_Tp>& operator -= (Size_<_Tp>& a, const Size_<_Tp>& b)
{ a.width -= b.width; a.height -= b.height; return a; }

template<typename _Tp> static inline Size_<_Tp> operator + (const Size_<_Tp>& a, const Size_<_Tp>& b)
{ return Size_<_Tp>(a.width + b.width, a.height + b.height); }
template<typename _Tp> static inline Size_<_Tp> operator - (const Size_<_Tp>& a, const Size_<_Tp>& b)
{ return Size_<_Tp>(a.width - b.width, a.height - b.height); }

template<typename _Tp> static inline Size_<_Tp> operator * (const Size_<_Tp>& a, _Tp b)
{ return Size_<_Tp>(a.width * b, a.height * b); }
template<typename _Tp> static inline Size_<_Tp> operator * (_Tp b, const Size_<_Tp>& a)
{ return Size_<_Tp>(a.width * b, a.height * b); }

template<typename _Tp> static inline bool operator == (const Size_<_Tp>& a, const Size_<_Tp>& b)
{ return a.width == b.width && a.height == b.height; }
template<typename _Tp> static inline bool operator != (const Size_<_Tp>& a, const Size_<_Tp>& b)
{ return a.width != b.width || a.height != b.height; }

template <typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Size_<_Tp>& b)
{ return (out << "[" << b.width << "x" << b.height << "]"); }
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Size_<_Tp>& b)
{ return (in >> b.width >> b.height); }

//////////////////////////////// Range /////////////////////////////////

inline Range::Range() : start(0), end(0) {}
inline Range::Range(int _start, int _end) : start(_start), end(_end) {}

inline size_t Range::size() const { return end - start; }
inline bool Range::empty() const { return start == end; }
inline Range Range::all() { return Range(INT_MIN, INT_MAX); }

static inline bool operator == (const Range& r1, const Range& r2)
{ return r1.start == r2.start && r1.end == r2.end; }
static inline bool operator != (const Range& r1, const Range& r2)
{ return !(r1 == r2); }
static inline bool operator !(const Range& r)
{ return r.start == r.end; }

static inline Range operator & (const Range& r1, const Range& r2) {
    Range r(std::max(r1.start, r2.start), std::min(r1.end, r2.end));
    r.end = std::max(r.end, r.start);
    return r;
}

static inline Range& operator &= (Range& r1, const Range& r2) {
    r1 = r1 & r2;
    return r1;
}

static inline Range operator + (const Range& r1, int delta)
{ return Range(r1.start + delta, r1.end + delta); }
static inline Range operator + (int delta, const Range& r1)
{ return Range(r1.start + delta, r1.end + delta); }
static inline Range operator - (const Range& r1, int delta)
{ return r1 + (-delta); }

static inline std::ostream& operator << (std::ostream& out, const Range& r)
{ return (out << "[" << r.start << ":" << r.end << "]"); }
static inline std::istream& operator >> (std::istream& in, Range& r)
{ return (in >> r.start >> r.end); }

} // namespace cv

#endif // __cplusplus

#endif
