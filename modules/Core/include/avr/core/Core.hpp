#ifndef AVR_CORE_MODULE_HPP
#define AVR_CORE_MODULE_HPP

#ifdef __cplusplus

#if __cplusplus > 199711L // C++11
   #include <initializer_list>
#endif // __cplusplus

#include <opencv/core/core.hpp>
#include <avr/core/SafePointer.hpp>

namespace avr {

//! Import some opencv template classes
using cv::Matx;
using cv::Vec;
using cv::Scalar_;
using cv::Complex;
using cv::Point_;
using cv::Point3_;
using cv::Rect_;
using cv::Size_;

//! Rename some opencv normal classes
typedef cv::Mat         Mat;
typedef cv::Range       Range;
typedef cv::RotatedRect RotatedRect;

//! @typedef More shorter aliases for C++ interger primitive types
typedef int8_t          byte;
typedef uint8_t         ubyte;
typedef uint32_t        uint;

//! @typedef Shorter aliases for the most popular specializations of the core classes
typedef Matx<int, 2, 2> Matx22i;    typedef Matx<float, 2, 2>   Matx22f;    typedef Matx<double, 2, 2>  Matx22d;
typedef Matx<int, 3, 3> Matx33i;    typedef Matx<float, 3, 3>   Matx33f;    typedef Matx<double, 3, 3>  Matx33d;
typedef Matx<int, 3, 4> Matx34i;    typedef Matx<float, 3, 4>   Matx34f;    typedef Matx<double, 3, 4>  Matx34d;
typedef Matx<int, 4, 4> Matx44i;    typedef Matx<float, 4, 4>   Matx44f;    typedef Matx<double, 4, 4>  Matx44d;
typedef Vec<int, 2>     Vec2i;      typedef Vec<float, 2>       Vec2f;      typedef Vec<double, 2>      Vec2d;
typedef Vec<int, 3>     Vec3i;      typedef Vec<float, 3>       Vec3f;      typedef Vec<double, 3>      Vec3d;
typedef Vec<int, 4>     Vec4i;      typedef Vec<float, 4>       Vec4f;      typedef Vec<double, 4>      Vec4d;
typedef Scalar_<int>    Scalari;    typedef Scalar_<float>      Scalarf;    typedef Scalar_<double>     Scalard;
typedef Complex<int>    Complexi;   typedef Complex<float>      Complexf;   typedef Complex<double>     Complexd;
typedef Point_<int>     Point;
typedef Point_<int>     Point2i;    typedef Point_<float>       Point2f;    typedef Point_<double>      Point2d;
typedef Point3_<int>    Point3i;    typedef Point3_<float>      Point3f;    typedef Point3_<double>     Point3d;
typedef Rect_<int>      Rect2i;     typedef Rect_<float>        Rect2f;     typedef Rect_<double>       Rect2d;
typedef Size_<int>      Size2i;     typedef Size_<float>        Size2f;     typedef Size_<double>       Size2d;
// Arrays
typedef const cv::_InputArray&  InputArray;
typedef const cv::_OutputArray& OutputArray;
typedef const cv::_OutputArray& InputOutputArray;
// Collection of abstract class references
template <class T>
using Set = std::vector<SPtr<T> >;

//! Import some opencv functions
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

/*-------------------------- Markerless AVRLib extended core classes --------------------------*/
template <typename Tp> class BRect;
template <class T> class Builder;
class Quaternion;
class TMatx;
class Deg;
class Rad;

////////////////////////////////// Builder Pattern Interface //////////////////////////////////
/**
 * @class Builder<> Core.hpp <avr/core/Core.hpp>
 * @brief Interface to implement the Builder design pattern
 */
template <class T>
class Builder {
public:
   virtual ~Builder() {/* dtor */}
   virtual SPtr<T> build() const = 0;
};

////////////////////////////////// Degrees and Radians //////////////////////////////////
/**
 * @class Deg Core.hpp <avr/core/Core.hpp>
 * @brief A simple class to represent angles in degrees
 */
class Deg {
public:
   double value;
   // conversion constructors
   explicit Deg(const double& v);
   explicit Deg(const Rad& rad);
   Deg(/* default */);
   //! Gets the angle value
   double Get() const;
   //! Converts angle to radians
   Rad ToRad() const;
   // Operators overload
   Deg operator * (const double& v) const { return Deg(this->value * v); }
   Deg operator / (const double& v) const { return Deg(this->value / v); }
   friend Deg operator * (const double& v, const Deg& a) { return Deg(v * a.value); }
   friend Deg operator / (const double& v, const Deg& a) { return Deg(v / a.value); }
};

/**
 * @class Rad Core.hpp <avr/core/Core.hpp>
 * @brief A simple class to represent angles in radians
 */
class Rad {
public:
   double value;
   // conversion constructors
   explicit Rad(const double& v);
   explicit Rad(const Deg& deg);
   Rad(/* default */);
   //! Gets the angle value
   double Get() const;
   //! Converts angle to degrees
   Deg ToDeg() const;
   // Operators overload
   Rad operator * (const double& v) const { return Rad(this->value * v); }
   Rad operator / (const double& v) const { return Rad(this->value / v); }
   friend Rad operator * (const double& v, const Rad& a) { return Rad(v * a.value); }
   friend Rad operator / (const double& v, const Rad& a) { return Rad(v / a.value); }
};

// some constant angles
const Rad PI      = Rad(4.0 * std::atan(1.0));
const Rad PI2     = Rad(PI / 2.0);
const Rad _2PI    = Rad(2.0 * PI);
//! @var Rad4Deg Radians per degree constant value
const Rad Rad4Deg = Rad(PI.Get() / 180.0);
//! @var Deg4Rad Degress per radian constant value
const Deg Deg4Rad = Deg(180.0 / PI.Get());

// some trigonometric functions
double sin(const Deg&);
double cos(const Deg&);
double tan(const Deg&);
double sin(const Rad&);
double cos(const Rad&);
double tan(const Rad&);
Rad asin(const float&);
Rad acos(const float&);
Rad atan(const float&);
Rad asin(const double&);
Rad acos(const double&);
Rad atan(const double&);

///////////////////////////////// Bounding Rectangle /////////////////////////////////
/**
 * @class BRect<> Core.hpp <avr/core/Core.hpp>
 * @brief This class represents a Bounding Rectangle, it is an interface for cv::Rect
 *
 * @note This class performs a safe comparation between two rectangles with real coordinates
 */
template <typename Tp>
class BRect {
public:
   // Various constructors
   BRect(Tp w, Tp h, Tp x = Tp(0), Tp y = Tp(0));
   BRect(const Point_<Tp>& p, const Size_<Tp>& s);
   BRect(const Point_<Tp>& p1, const Point_<Tp>& p2);
   BRect(const Rect_<Tp>&);
   // Explicit conversions
   template <typename _Tp> explicit BRect(const BRect<_Tp>&);
   template <typename _Tp> explicit BRect(const Rect_<_Tp>&);
   // Copy and default
   BRect(const BRect&);
   BRect(/* default */);

   //! Gets the object data reference
   const Rect_<Tp>& Get() const { return this->data; }
   //! @return The rectangle area (w x h)
   Tp GetArea() const;
   //! @return The central point of rectangle
   Point2f GetCenter() const;
   //! @return The width and height of rectangle
   Size_<Tp> GetSize() const;

   //! @return true if the point is inside the rectangle or false otherwise
   bool Contains(const Point_<Tp>&) const;
   //! @return true if rectangles intersect or false otherwise @note edges are open
   bool Intersects(const BRect&) const;

   // Comparison operations
   bool operator == (const BRect&) const;
   bool operator != (const BRect&) const;
   //! @return true if the rectangle do not exists or false otherwise
   bool operator ! () const;

   //! Updates the object data
   BRect& operator ()(Tp x, Tp y, Tp w, Tp h);
   //! Updates the object data
   BRect& operator ()(const Point_<Tp>& p, const Size_<Tp>& s);
   //! Updates the object data
   BRect& operator ()(const Point_<Tp>& p1, const Point_<Tp>& p2);

   // All classes of this template are friends with each other
   template <typename _Tp> friend class BRect;

private:
   Rect_<Tp> data;

};

///////////////////////////////// Transformation Matrix /////////////////////////////////
/**
 * @class TMatx Core.hpp <avr/core/Core.hpp>
 * @brief The Transformation Matrix class represents a rigid transformation for the camera and virtual objects.
 *
 * The internal representation is a 4x4 matrix linearized by line in a 16x1 array.
 */
class TMatx {
public:
   // Various constructors
   template <typename Tp> TMatx(const Matx<Tp, 3, 3>& rotation, const Vec<Tp, 3>& translation);
   template <typename Tp> TMatx(const Matx<Tp, 3, 3>& rotation);
   template <typename Tp> TMatx(const Matx<Tp, 3, 4>& transf);
   template <typename Tp> TMatx(const Matx<Tp, 4, 4>& transf);
   #if __cplusplus > 199711L // C++11
      template <typename Tp> TMatx(std::initializer_list<Tp>);
   #endif // __cplusplus
   // Explicit conversions
   explicit TMatx(const Quaternion&, const Point3d& = Point3d(0, 0, 0));
   explicit TMatx(const double(&)[16], bool byRow = false);
   // Copy and default
   TMatx(const TMatx&);
   TMatx(/* default */);

   //! Gets the object data reference
   const Matx44d& Get() const { return this->data; }
   /**
    * Converts the object data in a c-array
    * @param [out] buf The output buffer
    * @param [in]  byRow Flag to sets linearization mode (row or collumn)
    */
   void        GetData(double buf[16], bool byRow = false) const;
   //! @return The converted translation submatrix to 3D Point
   Point3d     GetPosition() const;
   //! @return The converted rotation submatrix to Quaternion
   Quaternion  GetQuaternion() const;

   //! Calculates and returns the transpose transformation
   TMatx  T() const;
   //! Calculates and returns the inverse transformation
   TMatx  Inv() const;
   //! Calculates and returns the determinant value
   double Det() const;

   //! Access a particular matrix cell
   double& At(ubyte row, ubyte col);
   //! Access a particular matrix cell (const version)
   const double& At(ubyte row, ubyte col) const;
   //! Access a particular matrix cell
   double& operator ()(ubyte row, ubyte col);
   //! Access a particular matrix cell (const version)
   const double& operator ()(ubyte row, ubyte col) const;

   // Operations
   TMatx&   operator += (const TMatx&);
   TMatx&   operator -= (const TMatx&);
   TMatx&   operator *= (const TMatx&);
   TMatx    operator +  (const TMatx&) const;
   TMatx    operator -  (const TMatx&) const;
   TMatx    operator *  (const TMatx&) const;
   // Template operations
   template <typename Tp> TMatx&          operator *= (Tp scalar);
   template <typename Tp> TMatx           operator *  (Tp scalar) const;
   template <typename Tp> Vec<double, 4>  operator *  (const Vec<Tp, 4>&) const;
   template <typename Tp> Vec<double, 3>  operator *  (const Vec<Tp, 3>&) const;
   template <typename Tp> Point3_<double>  operator *  (const Point3_<Tp>&) const;

private:
   cv::Matx<double, 4, 4> data;

};

///////////////////////////////// Quaternion /////////////////////////////////
/**
 * @class Quaternion Core.hpp <avr/core/Core.hpp>
 * @brief This class implements the @em quaternion structure to represents 3D rotations
 *
 * <div style="box-shadow:5px 5px 5px black"><h3> Theory </h3>
 * Quaternion is a generalization of complex numbers to 3D space, proposed by Hamilton in 1843.
 * A quaternion is defined as q = [w, v] = [w + xi + yj + zk], where w is a real scalar
 * and v a 3D vector with three imaginary units (i, j and k). These units have importants
 * multiplicative properties:
 *    @li i² = j² = k² = -1
 *    @li ij = k; jk = i; ki = j
 *    @li ji = -k; kj = -i; ik = -j
 * Quaternions may be used to rotate points around an arbitrary axis and represents the
 * orientation of virtual objects in 3D space. Be @em theta the rotation angle and @em u
 * the rotation axis, then the rotation quaternion is q = [cos(theta/2), sin(theta/2)u].
 * A 3D point P(x, y, z) may be writed quaternion's form as follow: p = [0, x, y, z].
 * Then the rotation is applied by: q * p * q^-1
 * </div> <!-- End theory -->
 *
 * Any Point3_<T>, Scalar_<T>, Vec<T,3> and Vec<T,4> may be converted implicitly to Quaternion.
 * Also it is possible to convert the rotation part of a TMatx to Quaternion. Operators +, - and *
 * are overloaded for the sum, subtraction, multiplication by scalar and quaternion multiplication
 * operations. This class implements yet the norm, conjugate and inverse operations.
 *
 * Ex:
 *    @code
 *    Quaternion q(Deg(90), 0, 0, 1);     // rotation of 90º around z-axis
 *    Point3d p(1, 1, 1);                 // creates a 3D point of double
 *    cout << (q * p * q.Inv()) << endl;  // apply transformation
 *    // Output: [0, [-1, 1, 0]]
 *    @endcode
 */
class Quaternion {
public:
   // Various constructors
   Quaternion(const Deg& angle, double x, double y, double z);
   Quaternion(const Deg& angle, const Point3d& rotationPoint);
   Quaternion(const Deg& angle, const Vec3d& rotationAxis);
   // Copy and default
   Quaternion(const Quaternion&);
   Quaternion(/* default */);
   // Explicit conversions
   explicit Quaternion(const TMatx&);
   // Implicit conversions (copies per-element)
   template <typename Tp> Quaternion(const Point3_<Tp>&);
   template <typename Tp> Quaternion(const Scalar_<Tp>&);
   template <typename Tp> Quaternion(const Vec<Tp, 3>&);
   template <typename Tp> Quaternion(const Vec<Tp, 4>&);

   // Getters
   double Qw() const;
   double Qx() const;
   double Qy() const;
   double Qz() const;

   //! Gets the object data reference
   const Vec4d& Get() const { return this->data; }
   //! @return The rotation angle in degrees
   Deg   GetAngle() const;
   //! @return The rotation axis
   Vec3d GetAxis() const;
   //! Converts the quaternion in a transformation matrix (no translation) and returns it
   TMatx GetMatrix() const;

   //! Calculates and returns the inverse quaternion (conj(q)/||q||)
   Quaternion Inv() const;
   //! Calculates and returns the quaternion's conjugate ([w, [-x, -y, -z]])
   Quaternion Conj() const;
   //! Calculates and returns the quaternion's L2-Norm
   double Norm() const;

   //! Access a quaternion's component by index @note Qw = 0; Qx = 1; Qy = 2; Qz = 3;
   double& operator [] (ubyte index);
   //! Access a quaternion's component by index (const version) @note Qw = 0; Qx = 1; Qy = 2; Qz = 3;
   const double& operator [] (ubyte index) const;

   // Operations
   Quaternion& operator += (const Quaternion&);
   Quaternion& operator -= (const Quaternion&);
   Quaternion& operator *= (const Quaternion&);
   Quaternion& operator *= (const double& scalar);
   Quaternion& operator /= (const double& scalar);
   Quaternion  operator +  (const Quaternion&) const;
   Quaternion  operator -  (const Quaternion&) const;
   Quaternion  operator *  (const Quaternion&) const;
   Quaternion  operator *  (const double& scalar) const;
   Quaternion  operator /  (const double& scalar) const;

private:
   cv::Vec<double, 4> data;

   Quaternion(double, double, double, double);

   typedef const double& Cdouble;
   void create(Cdouble, Cdouble, Cdouble, Cdouble);

};

} // namespace avr

#endif // __cplusplus

#include "impl/Core.tcc"

#endif // AVR_CORE_MODULE_HPP
