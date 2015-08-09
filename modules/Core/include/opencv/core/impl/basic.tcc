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

//  This file implements the classes
//    - Size_<>
//    - Range
//    - AutoBuffer<>
//    - RNG

#if !defined(AVR_CORE_BASIC_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_BASIC_TCC

#ifdef __cplusplus

#ifndef size_t
    typedef unsigned int size_t;
#endif // size_t

namespace cv {
// begin operations.hpp
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

// begin avr
template <typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Size_<_Tp>& b)
{ return (out << "[" << b.width << "x" << b.height << "]"); }
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Size_<_Tp>& b)
{ return (in >> b.width >> b.height); }
// end avr

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

// begin avr
static inline std::ostream& operator << (std::ostream& out, const Range& r)
{ return (out << "[" << r.start << ":" << r.end << "]"); }
static inline std::istream& operator >> (std::istream& in, Range& r)
{ return (in >> r.start >> r.end); }
// end avr

/////////////////////////////// AutoBuffer ////////////////////////////////////////

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer()
{
    ptr = buf;
    size = fixed_size;
}

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer(size_t _size)
{
    ptr = buf;
    size = fixed_size;
    allocate(_size);
}

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::~AutoBuffer()
{ deallocate(); }

template<typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::allocate(size_t _size)
{
    if(_size <= size)
        return;
    deallocate();
    if(_size > fixed_size)
    {
        ptr = cv::allocate<_Tp>(_size);
        size = _size;
    }
}

template<typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::deallocate()
{
    if( ptr != buf )
    {
        cv::deallocate<_Tp>(ptr, size);
        ptr = buf;
        size = fixed_size;
    }
}

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator _Tp* ()
{ return ptr; }

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator const _Tp* () const
{ return ptr; }

////////////////////////////////////// Multiply-with-Carry RNG //////////////////////////////////////////////

inline RNG::RNG() { state = 0xffffffff; }
inline RNG::RNG(uint64 _state) { state = _state ? _state : 0xffffffff; }
inline unsigned RNG::next()
{
    state = (uint64)(unsigned)state*CV_RNG_COEFF + (unsigned)(state >> 32);
    return (unsigned)state;
}

inline RNG::operator uchar() { return (uchar)next(); }
inline RNG::operator schar() { return (schar)next(); }
inline RNG::operator ushort() { return (ushort)next(); }
inline RNG::operator short() { return (short)next(); }
inline RNG::operator unsigned() { return next(); }
inline unsigned RNG::operator ()(unsigned N) {return (unsigned)uniform(0,N);}
inline unsigned RNG::operator ()() {return next();}
inline RNG::operator int() { return (int)next(); }
// * (2^32-1)^-1
inline RNG::operator float() { return next()*2.3283064365386962890625e-10f; }
inline RNG::operator double()
{
    unsigned t = next();
    return (((uint64)t << 32) | next())*5.4210108624275221700372640043497e-20;
}
inline int RNG::uniform(int a, int b) { return a == b ? a : (int)(next()%(b - a) + a); }
inline float RNG::uniform(float a, float b) { return ((float)*this)*(b - a) + a; }
inline double RNG::uniform(double a, double b) { return ((double)*this)*(b - a) + a; }
// end operations.hpp

} // namespace cv

#endif // __cplusplus

#endif
