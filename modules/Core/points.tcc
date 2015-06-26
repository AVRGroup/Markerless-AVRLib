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

#if !defined(AVR_CORE_POINTS_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_POINTS_TCC

#ifdef __cplusplus

namespace cv {

//////////////////////////////// 2D Point ////////////////////////////////

template<typename _Tp> inline Point_<_Tp>::Point_() : x(0), y(0) {}
template<typename _Tp> inline Point_<_Tp>::Point_(_Tp _x, _Tp _y) : x(_x), y(_y) {}
template<typename _Tp> inline Point_<_Tp>::Point_(const Point_& pt) : x(pt.x), y(pt.y) {}
template<typename _Tp> inline Point_<_Tp>::Point_(const Size_<_Tp>& sz) : x(sz.width), y(sz.height) {}
template<typename _Tp> inline Point_<_Tp>::Point_(const Vec<_Tp,2>& v) : x(v[0]), y(v[1]) {}
template<typename _Tp> inline Point_<_Tp>& Point_<_Tp>::operator = (const Point_& pt)
{ x = pt.x; y = pt.y; return *this; }

template<typename _Tp> template<typename _Tp2> inline Point_<_Tp>::operator Point_<_Tp2>() const
{ return Point_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y)); }
template<typename _Tp> inline Point_<_Tp>::operator Vec<_Tp, 2>() const
{ return Vec<_Tp, 2>(x, y); }

template<typename _Tp> inline _Tp Point_<_Tp>::dot(const Point_& pt) const
{ return saturate_cast<_Tp>(x*pt.x + y*pt.y); }
template<typename _Tp> inline double Point_<_Tp>::ddot(const Point_& pt) const
{ return (double)x*pt.x + (double)y*pt.y; }

template<typename _Tp> inline double Point_<_Tp>::cross(const Point_& pt) const
{ return (double)x*pt.y - (double)y*pt.x; }

template<typename _Tp> inline bool Point_<_Tp>::inside( const Rect_<_Tp>& r ) const
{ return r.contains(*this); }

template<typename _Tp> static inline Point_<_Tp>&
operator += (Point_<_Tp>& a, const Point_<_Tp>& b)
{
    a.x = saturate_cast<_Tp>(a.x + b.x);
    a.y = saturate_cast<_Tp>(a.y + b.y);
    return a;
}

template<typename _Tp> static inline Point_<_Tp>&
operator -= (Point_<_Tp>& a, const Point_<_Tp>& b)
{
    a.x = saturate_cast<_Tp>(a.x - b.x);
    a.y = saturate_cast<_Tp>(a.y - b.y);
    return a;
}

template<typename _Tp> static inline Point_<_Tp>&
operator *= (Point_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    return a;
}

template<typename _Tp> static inline Point_<_Tp>&
operator *= (Point_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    return a;
}

template<typename _Tp> static inline Point_<_Tp>&
operator *= (Point_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    return a;
}

template<typename _Tp> static inline double norm(const Point_<_Tp>& pt)
{ return std::sqrt((double)pt.x*pt.x + (double)pt.y*pt.y); }

template<typename _Tp> static inline bool operator == (const Point_<_Tp>& a, const Point_<_Tp>& b)
{ return a.x == b.x && a.y == b.y; }

template<typename _Tp> static inline bool operator != (const Point_<_Tp>& a, const Point_<_Tp>& b)
{ return a.x != b.x || a.y != b.y; }

template<typename _Tp> static inline Point_<_Tp> operator + (const Point_<_Tp>& a, const Point_<_Tp>& b)
{ return Point_<_Tp>( saturate_cast<_Tp>(a.x + b.x), saturate_cast<_Tp>(a.y + b.y) ); }

template<typename _Tp> static inline Point_<_Tp> operator - (const Point_<_Tp>& a, const Point_<_Tp>& b)
{ return Point_<_Tp>( saturate_cast<_Tp>(a.x - b.x), saturate_cast<_Tp>(a.y - b.y) ); }

template<typename _Tp> static inline Point_<_Tp> operator - (const Point_<_Tp>& a)
{ return Point_<_Tp>( saturate_cast<_Tp>(-a.x), saturate_cast<_Tp>(-a.y) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (const Point_<_Tp>& a, int b)
{ return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (int a, const Point_<_Tp>& b)
{ return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (const Point_<_Tp>& a, float b)
{ return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (float a, const Point_<_Tp>& b)
{ return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (const Point_<_Tp>& a, double b)
{ return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) ); }

template<typename _Tp> static inline Point_<_Tp> operator * (double a, const Point_<_Tp>& b)
{ return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) ); }

template<typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Point_<_Tp>& b)
{ return (out << "(" << b.x << ", " << b.y << ")"); }
template<typename _Tp> static inline std::istream& operator >> (std::istream& in, Point_<_Tp>& b)
{ return (in >> b.x >> b.y); }

//////////////////////////////// 3D Point ////////////////////////////////

template<typename _Tp> inline Point3_<_Tp>::Point3_() : x(0), y(0), z(0) {}
template<typename _Tp> inline Point3_<_Tp>::Point3_(_Tp _x, _Tp _y, _Tp _z) : x(_x), y(_y), z(_z) {}
template<typename _Tp> inline Point3_<_Tp>::Point3_(const Point3_& pt) : x(pt.x), y(pt.y), z(pt.z) {}
template<typename _Tp> inline Point3_<_Tp>::Point3_(const Point_<_Tp>& pt) : x(pt.x), y(pt.y), z(_Tp()) {}
template<typename _Tp> inline Point3_<_Tp>::Point3_(const Vec<_Tp, 3>& v) : x(v[0]), y(v[1]), z(v[2]) {}

template<typename _Tp> template<typename _Tp2> inline Point3_<_Tp>::operator Point3_<_Tp2>() const
{ return Point3_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y), saturate_cast<_Tp2>(z)); }

template<typename _Tp> inline Point3_<_Tp>::operator Vec<_Tp, 3>() const
{ return Vec<_Tp, 3>(x, y, z); }

template<typename _Tp> inline Point3_<_Tp>& Point3_<_Tp>::operator = (const Point3_& pt)
{ x = pt.x; y = pt.y; z = pt.z; return *this; }

template<typename _Tp> inline _Tp Point3_<_Tp>::dot(const Point3_& pt) const
{ return saturate_cast<_Tp>(x*pt.x + y*pt.y + z*pt.z); }
template<typename _Tp> inline double Point3_<_Tp>::ddot(const Point3_& pt) const
{ return (double)x*pt.x + (double)y*pt.y + (double)z*pt.z; }

template<typename _Tp> inline Point3_<_Tp> Point3_<_Tp>::cross(const Point3_<_Tp>& pt) const
{
    return Point3_<_Tp>(y*pt.z - z*pt.y, z*pt.x - x*pt.z, x*pt.y - y*pt.x);
}

template<typename _Tp> static inline Point3_<_Tp>&
operator += (Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    a.x = saturate_cast<_Tp>(a.x + b.x);
    a.y = saturate_cast<_Tp>(a.y + b.y);
    a.z = saturate_cast<_Tp>(a.z + b.z);
    return a;
}

template<typename _Tp> static inline Point3_<_Tp>&
operator -= (Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    a.x = saturate_cast<_Tp>(a.x - b.x);
    a.y = saturate_cast<_Tp>(a.y - b.y);
    a.z = saturate_cast<_Tp>(a.z - b.z);
    return a;
}

template<typename _Tp> static inline Point3_<_Tp>&
operator *= (Point3_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    a.z = saturate_cast<_Tp>(a.z*b);
    return a;
}

template<typename _Tp> static inline Point3_<_Tp>&
operator *= (Point3_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    a.z = saturate_cast<_Tp>(a.z*b);
    return a;
}

template<typename _Tp> static inline Point3_<_Tp>&
operator *= (Point3_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x*b);
    a.y = saturate_cast<_Tp>(a.y*b);
    a.z = saturate_cast<_Tp>(a.z*b);
    return a;
}

template<typename _Tp> static inline double norm(const Point3_<_Tp>& pt)
{ return std::sqrt((double)pt.x*pt.x + (double)pt.y*pt.y + (double)pt.z*pt.z); }

template<typename _Tp> static inline bool operator == (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{ return a.x == b.x && a.y == b.y && a.z == b.z; }

template<typename _Tp> static inline bool operator != (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{ return a.x != b.x || a.y != b.y || a.z != b.z; }

template<typename _Tp> static inline Point3_<_Tp> operator + (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(a.x + b.x),
                      saturate_cast<_Tp>(a.y + b.y),
                      saturate_cast<_Tp>(a.z + b.z)); }

template<typename _Tp> static inline Point3_<_Tp> operator - (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(a.x - b.x),
                        saturate_cast<_Tp>(a.y - b.y),
                        saturate_cast<_Tp>(a.z - b.z)); }

template<typename _Tp> static inline Point3_<_Tp> operator - (const Point3_<_Tp>& a)
{ return Point3_<_Tp>( saturate_cast<_Tp>(-a.x),
                      saturate_cast<_Tp>(-a.y),
                      saturate_cast<_Tp>(-a.z) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (const Point3_<_Tp>& a, int b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(a.x*b),
                      saturate_cast<_Tp>(a.y*b),
                      saturate_cast<_Tp>(a.z*b) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (int a, const Point3_<_Tp>& b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(b.x*a),
                      saturate_cast<_Tp>(b.y*a),
                      saturate_cast<_Tp>(b.z*a) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (const Point3_<_Tp>& a, float b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(a.x*b),
                      saturate_cast<_Tp>(a.y*b),
                      saturate_cast<_Tp>(a.z*b) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (float a, const Point3_<_Tp>& b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(b.x*a),
                      saturate_cast<_Tp>(b.y*a),
                      saturate_cast<_Tp>(b.z*a) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (const Point3_<_Tp>& a, double b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(a.x*b),
                      saturate_cast<_Tp>(a.y*b),
                      saturate_cast<_Tp>(a.z*b) ); }

template<typename _Tp> static inline Point3_<_Tp> operator * (double a, const Point3_<_Tp>& b)
{ return Point3_<_Tp>( saturate_cast<_Tp>(b.x*a),
                      saturate_cast<_Tp>(b.y*a),
                      saturate_cast<_Tp>(b.z*a) ); }

template<typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Point3_<_Tp>& b)
{ return (out << "(" << b.x << ", " << b.y << ", " << b.z << ")"); }
template<typename _Tp> static inline std::istream& operator >> (std::istream& in, Point3_<_Tp>& b)
{ return (in >> b.x >> b.y >> b.z); }

} // namespace cv

#endif // __cplusplus

#endif

