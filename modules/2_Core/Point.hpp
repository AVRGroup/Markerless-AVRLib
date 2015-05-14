#ifndef AVR_POINT_HPP
#define AVR_POINT_HPP

#ifdef __cplusplus

namespace avr {

//! public types
template<typename Tp>       class Point2_;	// Generic 2D point
template<typename Tp>       class Point3_;	// Generic 3D point

typedef class Point2_<int>		Point2i;
typedef class Point2_<char>	Point2b;
typedef class Point2_<float>	Point2f;
typedef class Point2_<double>	Point2d;
typedef class Point3_<int>		Point3i;
typedef class Point3_<char>	Point3b;
typedef class Point3_<float>	Point3f;
typedef class Point3_<double>	Point3d;

class TransformationMatrix;

//! \class Point2_ Point.hpp "Point.hpp" \brief 2D points of generic coordinate type
template<typename Tp> class Point2_ {
   private:
      Tp x, y;

   public:
      explicit Point2_(const Tp& x, const Tp& y);
      Point2_(const Point2_&);

      template <typename Tp2> explicit Point2_(const Point2_<Tp2>&);
      template <typename Tp2> explicit Point2_(const Point3_<Tp2>&);

      Point2_();
      ~Point2_();

      // Gets
      inline Tp GetX() const { return this->x; }
      inline Tp GetY() const { return this->y; }

      // Assignment operators
      template<typename Tp2> Point2_<Tp>& operator =  (const Point2_<Tp2>& p);
      template<typename Tp2> Point2_<Tp>& operator += (const Point2_<Tp2>& p);
      template<typename Tp2> Point2_<Tp>& operator -= (const Point2_<Tp2>& p);
      template<typename Tp2> Point2_<Tp>& operator *= (const Tp2& scalar);
      template<typename Tp2> Point2_<Tp>& operator /= (const Tp2& scalar);

      // Arithmetic operators
      template<typename Tp2> Point2_<Tp>& operator + (const Point2_<Tp2>& p) const;
      template<typename Tp2> Point2_<Tp>& operator - (const Point2_<Tp2>& p) const;
      template<typename Tp2> Point2_<Tp>& operator * (const Tp2& scalar) const;
      template<typename Tp2> Point2_<Tp>& operator / (const Tp2& scalar) const;

      // Unary operators
      bool operator ! () const;
      Point2_<Tp>& operator - () const;

      // Logical operators
      template<typename Tp2> bool operator == (const Point2_<Tp2>& p) const;
      template<typename Tp2> bool operator != (const Point2_<Tp2>& p) const;
};
//! \class Point3_ Point.hpp "Point.hpp" \brief 3D points of generic coordinate type
template<typename Tp> class Point3_ {
   private:
      Tp x, y, z;

   public:
      explicit Point3_(const Tp& x, const Tp& y, const Tp& z);
      Point3_(const Point3_&);

      template <typename Tp2> explicit Point3_(const Point3_<Tp2>&);
      template <typename Tp2> explicit Point3_(const Point2_<Tp2>&);

      Point3_();
      ~Point3_();

      // Gets
      inline Tp GetX() const { return this->x; }
      inline Tp GetY() const { return this->y; }
      inline Tp GetZ() const { return this->z; }

      // Assignment operators
      template<typename Tp2> Point3_<Tp>& operator =  (const Point3_<Tp2>& p);
      template<typename Tp2> Point3_<Tp>& operator += (const Point3_<Tp2>& p);
      template<typename Tp2> Point3_<Tp>& operator -= (const Point3_<Tp2>& p);
      template<typename Tp2> Point3_<Tp>& operator *= (const Tp2& scalar);
      template<typename Tp2> Point3_<Tp>& operator /= (const Tp2& scalar);

      // Arithmetic operators
      template<typename Tp2> Point3_<Tp>& operator + (const Point3_<Tp2>& p) const;
      template<typename Tp2> Point3_<Tp>& operator - (const Point3_<Tp2>& p) const;
      template<typename Tp2> Point3_<Tp>& operator * (const Tp2& scalar) const;
      template<typename Tp2> Point3_<Tp>& operator / (const Tp2& scalar) const;

      // Unary operators
      bool operator ! () const;
      Point3_<Tp>& operator - () const;

      // Logical operators
      template<typename Tp2> bool operator == (const Point3_<Tp2>& p) const;
      template<typename Tp2> bool operator != (const Point3_<Tp2>& p) const;

      friend class TransformationMatrix;
};

} // namespace avr

#endif // __cplusplus

#include "Point.tcc"

#endif // AVR_POINT_HPP
