#ifndef AVR_POINT_HPP
   #include "Point.hpp"
#endif // AVR_POINT_HPP

#include <istream>
#include <ostream>
#include <cmath>

#ifdef __cplusplus

namespace avr {

//! Point2_
template <typename Tp> template <typename Tp2> Point2_<Tp>::Point2_(const Point2_<Tp2>& p) : x(static_cast<Tp>(p.GetX())), y(static_cast<Tp>(p.GetY())) { /* conversion ctor */ }
template <typename Tp> template <typename Tp2> Point2_<Tp>::Point2_(const Point3_<Tp2>& p) : x(static_cast<Tp>(p.GetX())), y(static_cast<Tp>(p.GetY())) { /* conversion ctor */ }
template <typename Tp> Point2_<Tp>::Point2_(const Point2_& p) : x(p.GetX()), y(p.GetY()) { /* copy ctor */ }
template <typename Tp> Point2_<Tp>::Point2_(const Tp& x, const Tp& y) : x(x), y(y) { /* init ctor */ }
template <typename Tp> Point2_<Tp>::Point2_() : x(Tp(0)), y(Tp(0)) { /* default ctor */ }
template <typename Tp> Point2_<Tp>::~Point2_() { /* dtor */ }

// Assignment operators
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator = (const Point2_<Tp2>& p) {
   x = static_cast<Tp>(p.GetX()); y = static_cast<Tp>(p.GetY());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator += (const Point2_<Tp2>& p) {
   x += static_cast<Tp>(p.GetX()); y += static_cast<Tp>(p.GetY());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator -= (const Point2_<Tp2>& p) {
   x -= static_cast<Tp>(p.GetX()); y -= static_cast<Tp>(p.GetY());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator *= (const Tp2& scalar) {
   x *= scalar; y *= scalar;
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator /= (const Tp2& scalar) {
   x /= scalar; y /= scalar;
   return * this;
}

// Arithmetic operators
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator + (const Point2_<Tp2>& p) const {
   return * new Point2_<Tp>(x + static_cast<Tp>(p.GetX()), y + static_cast<Tp>(p.GetY()));
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator - (const Point2_<Tp2>& p) const {
   return * new Point2_<Tp>(x - static_cast<Tp>(p.GetX()), y - static_cast<Tp>(p.GetY()));
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator * (const Tp2& scalar) const {
   return * new Point2_<Tp>(x * static_cast<Tp>(scalar), y * static_cast<Tp>(scalar));
}
template <typename Tp> template<typename Tp2> inline Point2_<Tp>& Point2_<Tp>::operator / (const Tp2& scalar) const {
   return * new Point2_<Tp>(x / static_cast<Tp>(scalar), y / static_cast<Tp>(scalar));
}

// Unary operators
template <typename Tp> inline Point2_<Tp>& Point2_<Tp>::operator - () const { return * new Point2_<Tp>(-x, -y); }
template <typename Tp> inline bool Point2_<Tp>::operator ! () const { return (!x and !y); }

// Logical operators
template <typename Tp> template<typename Tp2> inline bool Point2_<Tp>::operator == (const Point2_<Tp2>& p) const {
   return (x == static_cast<Tp>(p.GetX()) and y == static_cast<Tp>(p.GetY()));
}
template <typename Tp> template<typename Tp2> inline bool Point2_<Tp>::operator != (const Point2_<Tp2>& p) const {
   return (x != static_cast<Tp>(p.GetX()) or y != static_cast<Tp>(p.GetY()));
}

// IO operators
template <typename Tp> static inline std::ostream& operator << (std::ostream& out, const Point2_<Tp>& p) {
   out << "(" << p.GetX() << ", " << p.GetY() << ")";
   return out;
}
template <typename Tp> static inline std::istream& operator >> (std::istream& in, Point2_<Tp>& p) {
   Tp x, y; in >> x >> y;
   p = Point2_<Tp>(x, y);
   return in;
}

//! Point3_
template <typename Tp> template <typename Tp2> Point3_<Tp>::Point3_(const Point3_<Tp2>& p) : x(static_cast<Tp>(p.GetX())), y(static_cast<Tp>(p.GetY())), z(static_cast<Tp>(p.GetZ())) { /* conversion ctor */ }
template <typename Tp> template <typename Tp2> Point3_<Tp>::Point3_(const Point2_<Tp2>& p) : x(static_cast<Tp>(p.GetX())), y(static_cast<Tp>(p.GetY())), z(Tp(0)) { /* conversion ctor */ }
template <typename Tp> Point3_<Tp>::Point3_(const Tp& x, const Tp& y, const Tp& z) : x(x), y(y), z(z) { /* init ctor */ }
template <typename Tp> Point3_<Tp>::Point3_(const Point3_& p) : x(p.GetX()), y(p.GetY()), z(p.GetZ()) { /* copy ctor */ }
template <typename Tp> Point3_<Tp>::Point3_() : x(Tp(0)), y(Tp(0)), z(Tp(0)) { /* default ctor */ }
template <typename Tp> Point3_<Tp>::~Point3_() { /* dtor */ }

// Assignment operators
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator = (const Point3_<Tp2>& p) {
   x = static_cast<Tp>(p.GetX()); y = static_cast<Tp>(p.GetY()); z = static_cast<Tp>(p.GetZ());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator += (const Point3_<Tp2>& p) {
   x += static_cast<Tp>(p.GetX()); y += static_cast<Tp>(p.GetY()); z += static_cast<Tp>(p.GetZ());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator -= (const Point3_<Tp2>& p) {
   x -= static_cast<Tp>(p.GetX()); y -= static_cast<Tp>(p.GetY()); z -= static_cast<Tp>(p.GetZ());
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator *= (const Tp2& scalar) {
   x *= scalar; y *= scalar; z *= scalar;
   return * this;
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator /= (const Tp2& scalar) {
   x /= scalar; y /= scalar; z /= scalar;
   return * this;
}

// Arithmetic operators
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator + (const Point3_<Tp2>& p) const {
   return * new Point3_<Tp>(x + static_cast<Tp>(p.GetX()), y + static_cast<Tp>(p.GetY()), z + static_cast<Tp>(p.GetZ()));
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator - (const Point3_<Tp2>& p) const {
   return * new Point3_<Tp>(x - static_cast<Tp>(p.GetX()), y - static_cast<Tp>(p.GetY()), z - static_cast<Tp>(p.GetZ()));
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator * (const Tp2& scalar) const {
   return * new Point3_<Tp>(x * static_cast<Tp>(scalar), y * static_cast<Tp>(scalar), z * static_cast<Tp>(scalar));
}
template <typename Tp> template<typename Tp2> inline Point3_<Tp>& Point3_<Tp>::operator / (const Tp2& scalar) const {
   return * new Point3_<Tp>(x / static_cast<Tp>(scalar), y / static_cast<Tp>(scalar), z / static_cast<Tp>(scalar));
}

// Unary operators
template <typename Tp> inline bool Point3_<Tp>::operator ! () const { return (!x and !y and !z); }
template<typename Tp> inline Point3_<Tp>& Point3_<Tp>::operator - () const { return * new Point3_<Tp>(-x, -y, -z); }

// Logical operators
template <typename Tp> template<typename Tp2> inline bool Point3_<Tp>::operator == (const Point3_<Tp2>& p) const {
   return (x == static_cast<Tp>(p.GetX()) and y == static_cast<Tp>(p.GetY()) and z == static_cast<Tp>(p.GetZ()));
}
template <typename Tp> template<typename Tp2> inline bool Point3_<Tp>::operator != (const Point3_<Tp2>& p) const {
   return (x != static_cast<Tp>(p.GetX()) or y != static_cast<Tp>(p.GetY()) or z != static_cast<Tp>(p.GetZ()));
}

// IO operators
template <typename Tp> static inline std::ostream& operator << (std::ostream& out, const Point3_<Tp>& p) {
   out << "(" << p.GetX() << ", " << p.GetY() << ", " << p.GetZ() << ")";
   return out;
}
template <typename Tp> static inline std::istream& operator >> (std::istream& in, Point3_<Tp>& p) {
   Tp x, y, z; in >> x >> y >> z;
   p = Point3_<Tp>(x, y, z);
   return in;
}

} // namespace avr

#endif // __cplusplus
