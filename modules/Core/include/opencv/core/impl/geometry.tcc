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
//    - Point_<>
//    - Point3_<>
//    - Rect_<>
//    - RotatedRect

#if !defined(AVR_CORE_GEOMETRY_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_GEOMETRY_TCC

#ifdef __cplusplus

namespace cv {
// begin operations.hpp
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

/** Writes a point to an output stream in Matlab notation
 */
template<typename _Tp> inline std::ostream& operator<<(std::ostream& out, const Point_<_Tp>& p)
{
    out << "[" << p.x << ", " << p.y << "]";
    return out;
}

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

/** Writes a point to an output stream in Matlab notation
 */
template<typename _Tp> inline std::ostream& operator<<(std::ostream& out, const Point3_<_Tp>& p)
{
    out << "[" << p.x << ", " << p.y << ", " << p.z << "]";
    return out;
}

//////////////////////////////// Rect ////////////////////////////////

template<typename _Tp> inline Rect_<_Tp>::Rect_() : x(0), y(0), width(0), height(0) {}
template<typename _Tp> inline Rect_<_Tp>::Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height) : x(_x), y(_y), width(_width), height(_height) {}
template<typename _Tp> inline Rect_<_Tp>::Rect_(const Rect_<_Tp>& r) : x(r.x), y(r.y), width(r.width), height(r.height) {}
template<typename _Tp> inline Rect_<_Tp>::Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz) :
    x(org.x), y(org.y), width(sz.width), height(sz.height) {}
template<typename _Tp> inline Rect_<_Tp>::Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2)
{
    x = std::min(pt1.x, pt2.x); y = std::min(pt1.y, pt2.y);
    width = std::max(pt1.x, pt2.x) - x; height = std::max(pt1.y, pt2.y) - y;
}
template<typename _Tp> inline Rect_<_Tp>& Rect_<_Tp>::operator = ( const Rect_<_Tp>& r )
{ x = r.x; y = r.y; width = r.width; height = r.height; return *this; }

template<typename _Tp> inline Point_<_Tp> Rect_<_Tp>::tl() const { return Point_<_Tp>(x,y); }
template<typename _Tp> inline Point_<_Tp> Rect_<_Tp>::br() const { return Point_<_Tp>(x+width, y+height); }
template<typename _Tp> inline Size_<_Tp> Rect_<_Tp>::size() const { return Size_<_Tp>(width, height); }
template<typename _Tp> inline _Tp Rect_<_Tp>::area() const { return width*height; }

template<typename _Tp> static inline Rect_<_Tp>& operator += ( Rect_<_Tp>& a, const Point_<_Tp>& b )
{ a.x += b.x; a.y += b.y; return a; }
template<typename _Tp> static inline Rect_<_Tp>& operator -= ( Rect_<_Tp>& a, const Point_<_Tp>& b )
{ a.x -= b.x; a.y -= b.y; return a; }

template<typename _Tp> static inline Rect_<_Tp>& operator += ( Rect_<_Tp>& a, const Size_<_Tp>& b )
{ a.width += b.width; a.height += b.height; return a; }

template<typename _Tp> static inline Rect_<_Tp>& operator -= ( Rect_<_Tp>& a, const Size_<_Tp>& b )
{ a.width -= b.width; a.height -= b.height; return a; }

template<typename _Tp> static inline Rect_<_Tp>& operator &= ( Rect_<_Tp>& a, const Rect_<_Tp>& b )
{
    _Tp x1 = std::max(a.x, b.x), y1 = std::max(a.y, b.y);
    a.width = std::min(a.x + a.width, b.x + b.width) - x1;
    a.height = std::min(a.y + a.height, b.y + b.height) - y1;
    a.x = x1; a.y = y1;
    if( a.width <= 0 || a.height <= 0 )
        a = Rect_<int>();
    return a;
}

template<typename _Tp> static inline Rect_<_Tp>& operator |= ( Rect_<_Tp>& a, const Rect_<_Tp>& b )
{
    _Tp x1 = std::min(a.x, b.x), y1 = std::min(a.y, b.y);
    a.width = std::max(a.x + a.width, b.x + b.width) - x1;
    a.height = std::max(a.y + a.height, b.y + b.height) - y1;
    a.x = x1; a.y = y1;
    return a;
}

template<typename _Tp> template<typename _Tp2> inline Rect_<_Tp>::operator Rect_<_Tp2>() const
{ return Rect_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y),
                     saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height)); }

template<typename _Tp> inline bool Rect_<_Tp>::contains(const Point_<_Tp>& pt) const
{ return x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height; }

template<typename _Tp> static inline bool operator == (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}
template<typename _Tp> static inline bool operator != (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    return a.x != b.x || a.y != b.y || a.width != b.width || a.height != b.height;
}

template<typename _Tp> static inline Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Point_<_Tp>& b)
{
    return Rect_<_Tp>( a.x + b.x, a.y + b.y, a.width, a.height );
}
template<typename _Tp> static inline Rect_<_Tp> operator - (const Rect_<_Tp>& a, const Point_<_Tp>& b)
{
    return Rect_<_Tp>( a.x - b.x, a.y - b.y, a.width, a.height );
}

template<typename _Tp> static inline Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Size_<_Tp>& b)
{
    return Rect_<_Tp>( a.x, a.y, a.width + b.width, a.height + b.height );
}

template<typename _Tp> static inline Rect_<_Tp> operator & (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    Rect_<_Tp> c = a;
    return c &= b;
}
template<typename _Tp> static inline Rect_<_Tp> operator | (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    Rect_<_Tp> c = a;
    return c |= b;
}

/** Writes a Rect_ to an output stream. Format example : [640 x 480 from (10, 20)]
 */
template<typename _Tp> inline std::ostream& operator<<(std::ostream& out, const Rect_<_Tp>& rect)
{
    out << "[" << rect.width << " x " << rect.height << " from (" << rect.x << ", " << rect.y << ")]";
    return out;
}

//////////////////////////////// RotatedRect ////////////////////////////////

inline RotatedRect::RotatedRect() { angle = 0; }
inline RotatedRect::RotatedRect(const Point2f& _center, const Size2f& _size, float _angle)
    : center(_center), size(_size), angle(_angle) {}
// end operations.hpp

// begin matrix.cpp
void inline RotatedRect::points(Point2f pts[]) const {
   float w2 = size.width / 2.0f;
   float h2 = size.height / 2.0f;
   float theta = radians(angle);

   Matx<float, 2, 2> rot(std::cos(theta), -std::sin(theta), std::sin(theta), std::cos(theta));

   pts[1] = center + rot * Point2f(-w2, +h2);
   pts[2] = center + rot * Point2f(-w2, -h2);
   pts[3] = center + rot * Point2f(+w2, -h2);
   pts[0] = center + rot * Point2f(+w2, +h2);
}

Rect_<float> inline RotatedRect::boundingRect() const {
   Point2f pts[4];
   this->points(pts);

   Point2f max = pts[0], min = pts[0];
   for(int i = 1; i < 4; i++) {
      if(min.x > pts[i].x) min.x = pts[i].x;
      if(min.y > pts[i].y) min.y = pts[i].y;
      if(max.x < pts[i].x) max.x = pts[i].x;
      if(max.y < pts[i].y) max.y = pts[i].y;
   }

   return Rect_<float>(min, max);
}
// begin matrix.cpp

} // namespace cv

#endif // __cplusplus

#endif


