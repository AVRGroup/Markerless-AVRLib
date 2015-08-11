#ifndef AVR_CORE_MODULE_HPP
#define AVR_CORE_MODULE_HPP

#include <opencv/core/core.hpp>
#include <avr/core/SafePointer.hpp>

#define TO_SUPER

namespace avr {

//! rename some opencv template classes
#if __cplusplus > 199711L // C++11
template <typename _Tp, int m, int n>   using Matx = cv::Matx<_Tp, m, n>;
template <typename _Tp, int cn>         using Vec = cv::Vec<_Tp, cn>;
template <typename _Tp>                 using Scalar = cv::Scalar_<_Tp>;
template <typename _Tp>                 using Complex = cv::Complex<_Tp>;
template <typename _Tp>                 using Point2 = cv::Point_<_Tp>;
template <typename _Tp>                 using Point3 = cv::Point3_<_Tp>;
template <typename _Tp>                 using Rect = cv::Rect_<_Tp>;
template <typename _Tp>                 using Size = cv::Size_<_Tp>;
#else
template <typename _Tp, int m, int n>   class Matx : public cv::Matx<_Tp, m, n> { };
template <typename _Tp, int cn>         class Vec : public cv::Vec<_Tp, cn> { };
template <typename _Tp>                 class Scalar : public cv::Scalar_<_Tp> { };
template <typename _Tp>                 class Complex : public cv::Complex<_Tp> { };
template <typename _Tp>                 class Point2 : public cv::Point_<_Tp> { };
template <typename _Tp>                 class Point3 : public cv::Point3_<_Tp> { };
template <typename _Tp>                 class Rect : public cv::Rect_<_Tp> { };
template <typename _Tp>                 class Size : public cv::Size_<_Tp> { };
#endif // __cplusplus

//! rename some opencv normal classes
typedef cv::Mat             Mat;
typedef cv::Range           Range;
typedef cv::RotatedRect     RotatedRect;

//! @typedef Shorter aliases for the most popular specializations of the core classes
typedef Matx<int, 2, 2> Matx22i;    typedef Matx<float, 2, 2>   Matx22f;    typedef Matx<double, 2, 2>  Matx22d;
typedef Matx<int, 3, 3> Matx33i;    typedef Matx<float, 3, 3>   Matx33f;    typedef Matx<double, 3, 3>  Matx33d;
typedef Matx<int, 3, 4> Matx34i;    typedef Matx<float, 3, 4>   Matx34f;    typedef Matx<double, 3, 4>  Matx34d;
typedef Matx<int, 4, 4> Matx44i;    typedef Matx<float, 4, 4>   Matx44f;    typedef Matx<double, 4, 4>  Matx44d;
typedef Vec<int, 2>     Vec2i;      typedef Vec<float, 2>       Vec2f;      typedef Vec<double, 2>      Vec2d;
typedef Vec<int, 3>     Vec3i;      typedef Vec<float, 3>       Vec3f;      typedef Vec<double, 3>      Vec3d;
typedef Vec<int, 4>     Vec4i;      typedef Vec<float, 4>       Vec4f;      typedef Vec<double, 4>      Vec4d;
typedef Scalar<int>     Scalari;    typedef Scalar<float>       Scalarf;    typedef Scalar<double>      Scalard;
typedef Complex<int>    Complexi;   typedef Complex<float>      Complexf;   typedef Complex<double>     Complexd;
typedef Point2<int>     Point;
typedef Point2<int>     Point2i;    typedef Point2<float>       Point2f;    typedef Point2<double>      Point2d;
typedef Point3<int>     Point3i;    typedef Point3<float>       Point3f;    typedef Point3<double>      Point3d;
typedef Size<int>       Sizei;      typedef Size<float>         Sizef;      typedef Size<double>        Sized;
// Arrays
typedef const cv::_InputArray&  InputArray;
typedef const cv::_OutputArray& OutputArray;
typedef const cv::_OutputArray& InputOutputArray;

//! opencv functions
using cv::noArray;
using cv::norm;
using cv::normalize;
using cv::determinant;
using cv::invert;
using cv::solve;
using cv::eigen;
using cv::transpose;
using cv::trace;
using cv::swap;

///////////////////////////////// M-AVRLib extended features /////////////////////////////////
//! some constants
const double _PI     = 4.0 * std::atan(1.0);
const double _2PI    = 2.0 * _PI;
const double _PI2    = _PI / 2.0;
const double _PI4    = _PI / 4.0;
const double _TORAD  = _PI / 180.0;
const double _TODEG  = 180.0 / _PI;

//! converts an angle in radians to degrees
inline double degrees(double rad) { return rad * _TODEG; }
//! converts an angle in degrees to radians
inline double radians(double deg) { return deg * _TORAD; }

//! more classes
template <typename Tp>
class BoundingRect;
class Quaternion;
class TMatx;

template <typename Tp>
class BoundingRect {
private:
   Rect<Tp> data;
public:
   //! various constructors
   BoundingRect(Tp x, Tp y, Tp w, Tp h);
   BoundingRect(const Point2<Tp>& p, const Size<Tp>& s);
   BoundingRect(const Point2<Tp>& p1, const Point2<Tp>& p2);
   BoundingRect(const BoundingRect&);
   BoundingRect(const Rect<Tp>&);

   template <typename _Tp> explicit BoundingRect(const BoundingRect<_Tp>&);
   template <typename _Tp> explicit BoundingRect(const Rect<_Tp>&);

   BoundingRect();

   //! Getters
   Tp GetArea() const;
   Point2f GetCenter() const;
   Size<Tp> GetSize() const;

   //! @return true if the point is inside the rectangle or false otherwise
   bool Contains(const Point2<Tp>&) const;
   //! @return true if rectangles intersect or false otherwise @note edges are open
   bool Intersects(const BoundingRect&) const;

   //! assign operator
   BoundingRect& operator = (const BoundingRect&);

   //! compare operations
   bool operator == (const BoundingRect&);
   bool operator != (const BoundingRect&);

   //! Setters to update the object
   BoundingRect& operator ()(Tp x, Tp y, Tp w, Tp h);
   BoundingRect& operator ()(const Point2<Tp>& p, const Size<Tp>& s);
   BoundingRect& operator ()(const Point2<Tp>& p1, const Point2<Tp>& p2);

   //! explicit conversion operator to cv::Rect_<>
   inline const cv::Rect_<Tp>& operator ()() const { return this->data; }

   //! all classes of this template are friends with each other
   template <typename _Tp> friend class BoundingRect;
};

/**
 * The Transformation Matrix class (TMatx) represents a rigid tranformation for the camera and virtual objects.
 * Your internal representation is a 4x4 matrix linearized by line in a 16x1 array. Instances of this class
 * may be easily converted to cv::Matx44d by explicit call to operator (), in addition they are implicitly
 * converted to OpenGL format array (double[16] linearized by column)
 */
class TMatx {
private:
   cv::Matx<double, 4, 4> data;
public:
   //! various constructors
   template <typename Tp> TMatx(const Matx<Tp, 3, 3>& rotation, const Vec<Tp, 3>& translation);
   template <typename Tp> TMatx(const Matx<Tp, 3, 3>& rotation);
   template <typename Tp> TMatx(const Matx<Tp, 3, 4>& transf);
   template <typename Tp> TMatx(const Matx<Tp, 4, 4>& transf);

   explicit TMatx(const Quaternion&);
   explicit TMatx(const double (&v)[16]);

   TMatx(const TMatx&);
   TMatx();

   Quaternion  GetQuaternion() const;
   Matx44d     GetMatx() const;
   Point3d     GetPosition() const;
   void        GetGLData(double [16]) const;

   static TMatx  Invert(const TMatx&);
   static TMatx  Transpose(const TMatx&);
   static double Determinant(const TMatx&);

   //! Access a particular matrix cell
   double& At(uint8 row, uint8 col);
   double& operator ()(uint8 row, uint8 col);
   //! Access a particular matrix cell (const version)
   const double& At(uint8 row, uint8 col) const;
   const double& operator ()(uint8 row, uint8 col) const;

   TMatx&   operator = (const TMatx&);
   TMatx&   operator += (const TMatx&);
   TMatx&   operator -= (const TMatx&);
   TMatx&   operator *= (const TMatx&);

   TMatx    operator + (const TMatx&) const;
   TMatx    operator - (const TMatx&) const;
   TMatx    operator * (const TMatx&) const;

   template <typename Tp> TMatx&          operator*= (Tp scalar);
   template <typename Tp> TMatx           operator * (Tp scalar) const;
   template <typename Tp> Vec<double, 4>  operator * (const Vec<Tp, 4>&) const;
   template <typename Tp> Vec<double, 3>  operator * (const Vec<Tp, 3>&) const;
   template <typename Tp> Point3<double>  operator * (const Point3<Tp>&) const;

   //! implicit conversion operator to double * (OpenGL format)
   operator double*() const;

   //! explicit conversion operator to cv::Matx44d
   const cv::Matx<double, 4, 4>& operator ()() const;
};

class Quaternion {
private:
   cv::Vec<double, 4> data;
public:
   typedef double Degree;

   Quaternion();
   Quaternion(Degree angle, double x, double y, double z);
   Quaternion(Degree angle, const Point3d& rotationPoint);
   Quaternion(Degree angle, const Vec3d& rotationAxis);
   Quaternion(const Quaternion&);

   explicit Quaternion(const TMatx&);
   template <typename Tp> Quaternion(const Point3<Tp>&);
   template <typename Tp> Quaternion(const Scalar<Tp>&);
   template <typename Tp> Quaternion(const Vec<Tp, 3>&);
   template <typename Tp> Quaternion(const Vec<Tp, 4>&);

   double Qw() const;
   double Qx() const;
   double Qy() const;
   double Qz() const;

   TMatx  GetMatrix() const;
   Vec3d  GetAxis() const;
   double GetAngle() const;

   static double Norm(const Quaternion&);
   static Quaternion Conjugate(const Quaternion&);
   static Quaternion Inverse(const Quaternion&);

   //! Access a quaternion's component by index
   //! @note Qx = 1; Qy = 2; Qz = 3; Qw = 0;
   double& operator () (uint8 component);
   const double& operator () (uint8 component) const;

   Quaternion& operator =  (const Quaternion&);
   Quaternion& operator += (const Quaternion&);
   Quaternion& operator -= (const Quaternion&);
   Quaternion& operator *= (const Quaternion&);
   Quaternion& operator *= (double scalar);

   Quaternion operator + (const Quaternion&) const;
   Quaternion operator - (const Quaternion&) const;
   Quaternion operator * (const Quaternion&) const;
   Quaternion operator * (double scalar) const;

   //! explicit conversion operator to cv::Vec4d
   inline const cv::Vec<double, 4>& operator ()() const { return this->data; }

private:
   typedef const double& Double;
   void create(Double, Double, Double, Double);

};

} // namespace avr

#include "impl/Bounding.tcc"
#include "impl/Quaternion.tcc"
#include "impl/TMatx.tcc"

#endif // AVR_CORE_MODULE_HPP
