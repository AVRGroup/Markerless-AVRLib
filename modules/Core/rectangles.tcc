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

#if !defined(AVR_CORE_RECTANGLES_TCC) && !defined(__OPENCV_CORE_OPERATIONS_HPP__)
#define AVR_CORE_RECTANGLES_TCC

#ifdef __cplusplus

namespace cv {

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

template<typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Rect_<_Tp>& b)
{
    out <<  "Rect_ Information\n" <<
            "  min  = " << Point_<_Tp>(b.x, b.y) << "\n" <<
            "  max  = " << Point_<_Tp>(b.x + b.width, b.y + b.height) << "\n" <<
            "  size = " << b.size() << "\n";
    return out;
}
template<typename _Tp> static inline std::istream& operator >> (std::istream& in, Rect_<_Tp>& b)
{ return (in >> b.x >> b.y >> b.width >> b.height); }

//////////////////////////////// RotatedRect ////////////////////////////////

inline RotatedRect::RotatedRect() { angle = 0; }
inline RotatedRect::RotatedRect(const Point2f& _center, const Size2f& _size, float _angle)
    : center(_center), size(_size), angle(_angle) {}

inline void RotatedRect::points(Point2f pts[]) const {
   float w2 = size.width / 2.0f;
   float h2 = size.height / 2.0f;
   float theta = radians(angle);

   Matx<float, 2, 2> rot(std::cos(theta), -std::sin(theta), std::sin(theta), std::cos(theta));

   pts[0] = center + rot * Point2f(+w2, +h2);
   pts[1] = center + rot * Point2f(-w2, +h2);
   pts[2] = center + rot * Point2f(-w2, -h2);
   pts[3] = center + rot * Point2f(+w2, -h2);
}
inline Rect_<float> RotatedRect::boundingRect() const {
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

static inline std::ostream& operator << (std::ostream& out, const RotatedRect& b)
{
    out <<  "RotatedRect Information\n" <<
            "  center = " << b.center << "\n" <<
            "  angle  = " << b.angle << "\n" <<
            "  size = " << b.size << "\n";
    return out;
}
static inline std::istream& operator >> (std::istream& in, RotatedRect& b)
{ return (in >> b.center >> b.size >> b.angle); }

} // namespace cv

#endif // __cplusplus

#endif
