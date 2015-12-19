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

#if !defined(AVR_CORE_HPP) && !defined(__OPENCV_CORE_HPP__)
#define AVR_CORE_HPP

#include <algorithm>
#include <float.h>
#include <cstddef>
#include <complex>
#include <vector>

#include <avr/core/Handling.hpp>

#include "types.hpp"

#ifdef __cplusplus

namespace cv {

//! Template classes in this file
template<typename _Tp> class Size_;
template<typename _Tp> class Rect_;
template<typename _Tp> class Point_;
template<typename _Tp> class Point3_;
template<typename _Tp> class Scalar_;
template<typename _Tp> class Complex;
template<typename _Tp, int cn> class Vec;
template<typename _Tp, int m, int n> class Matx;
template<typename _Tp, size_t fixed_size> class AutoBuffer;
//! Classes in this file
class Mat;
class Range;
class RotatedRect;
class _InputArray;
class _OutputArray;

//! @typedef Shorter aliases for the most popular specializations of the core classes
// Matx
typedef Matx<int, 3, 3>     Matx33i;
typedef Matx<float, 3, 3>   Matx33f;
typedef Matx<double, 3, 3>  Matx33d;
typedef Matx<int, 3, 4>     Matx34i;
typedef Matx<float, 3, 4>   Matx34f;
typedef Matx<double, 3, 4>  Matx34d;
typedef Matx<int, 4, 4>     Matx44i;
typedef Matx<float, 4, 4>   Matx44f;
typedef Matx<double, 4, 4>  Matx44d;
// Vec
typedef Vec<uchar, 2>       Vec2b;
typedef Vec<uchar, 3>       Vec3b;
typedef Vec<uchar, 4>       Vec4b;
typedef Vec<ushort, 2>      Vec2w;
typedef Vec<ushort, 3>      Vec3w;
typedef Vec<ushort, 4>      Vec4w;
typedef Vec<short, 2>       Vec2s;
typedef Vec<short, 3>       Vec3s;
typedef Vec<short, 4>       Vec4s;
typedef Vec<int, 2>         Vec2i;
typedef Vec<int, 3>         Vec3i;
typedef Vec<int, 4>         Vec4i;
typedef Vec<int, 6>         Vec6i;
typedef Vec<int, 8>         Vec8i;
typedef Vec<float, 2>       Vec2f;
typedef Vec<float, 3>       Vec3f;
typedef Vec<float, 4>       Vec4f;
typedef Vec<float, 6>       Vec6f;
typedef Vec<double, 2>      Vec2d;
typedef Vec<double, 3>      Vec3d;
typedef Vec<double, 4>      Vec4d;
typedef Vec<double, 6>      Vec6d;
// Point_
typedef Point_<int>         Point;
typedef Point_<int>         Point2i;
typedef Point_<float>       Point2f;
typedef Point_<double>      Point2d;
// Point3_
typedef Point3_<int>        Point3i;
typedef Point3_<float>      Point3f;
typedef Point3_<double>     Point3d;
// Scalar_
typedef Scalar_<float>      Scalar;
// Complex
typedef Complex<float>      Complexf;
typedef Complex<double>     Complexd;
// Size_
typedef Size_<int>          Size;
typedef Size_<int>          Size2i;
typedef Size_<float>        Size2f;
typedef Size_<double>       Size2d;
// Rect_
typedef Rect_<int>          Rect;
// Input and Output Arrays
typedef const _InputArray&  InputArray;
typedef const _OutputArray& OutputArray;
typedef InputArray          InputArrayOfArrays;
typedef OutputArray         OutputArrayOfArrays;
typedef OutputArray         InputOutputArray;
typedef OutputArray         InputOutputArrayOfArrays;
} // namespace cv

#include "helper.hpp"

namespace cv {

//////////////////////////////// Complex //////////////////////////////
/*!
  A complex number class.

  The template class is similar and compatible with std::complex, however it provides slightly
  more convenient access to the real and imaginary parts using through the simple field access, as opposite
  to std::complex::real() and std::complex::imag().
*/
template<typename _Tp> class Complex
{
public:

    //! constructors
    Complex();
    Complex( _Tp _re, _Tp _im=0 );
    Complex( const std::complex<_Tp>& c );

    //! conversion to another data type
    template<typename T2> operator Complex<T2>() const;
    //! conjugation
    Complex conj() const;
    //! conversion to std::complex
    operator std::complex<_Tp>() const;

    _Tp re, im; //< the real and the imaginary parts
};

//////////////////////////////// Point 2D ////////////////////////////////
/*!
  template 2D point class.

  The class defines a point in 2D space. Data type of the point coordinates is specified
  as a template parameter. There are a few shorter aliases available for user convenience.
  See cv::Point, cv::Point2i, cv::Point2f and cv::Point2d.
*/
template<typename _Tp> class Point_
{
public:
    typedef _Tp value_type;

    // various constructors
    Point_();
    Point_(_Tp _x, _Tp _y);
    Point_(const Point_& pt);
    Point_(const Size_<_Tp>& sz);
    Point_(const Vec<_Tp, 2>& v);

    Point_& operator = (const Point_& pt);
    //! conversion to another data type
    template<typename _Tp2> operator Point_<_Tp2>() const;

    //! conversion to cv::Vec<>
    operator Vec<_Tp, 2>() const;

    //! dot product
    _Tp dot(const Point_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point_& pt) const;
    //! cross-product
    double cross(const Point_& pt) const;
    //! checks whether the point is inside the specified rectangle
    bool inside(const Rect_<_Tp>& r) const;

    _Tp x, y; //< the point coordinates
};

//////////////////////////////// Point 3D ////////////////////////////////
/*!
  template 3D point class.

  The class defines a point in 3D space. Data type of the point coordinates is specified
  as a template parameter.

  \see cv::Point3i, cv::Point3f and cv::Point3d
*/
template<typename _Tp> class Point3_
{
public:
    typedef _Tp value_type;

    // various constructors
    Point3_();
    Point3_(_Tp _x, _Tp _y, _Tp _z);
    Point3_(const Point3_& pt);
    explicit Point3_(const Point_<_Tp>& pt);
    Point3_(const Vec<_Tp, 3>& v);

    Point3_& operator = (const Point3_& pt);
    //! conversion to another data type
    template<typename _Tp2> operator Point3_<_Tp2>() const;
    //! conversion to cv::Vec<>
    operator Vec<_Tp, 3>() const;

    //! dot product
    _Tp dot(const Point3_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point3_& pt) const;
    //! cross product of the 2 3D points
    Point3_ cross(const Point3_& pt) const;

    _Tp x, y, z; //< the point coordinates
};

//////////////////////////////// Size ////////////////////////////////
/*!
  The 2D size class

  The class represents the size of a 2D rectangle, image size, matrix size etc.
  Normally, cv::Size ~ cv::Size_<int> is used.
*/
template<typename _Tp> class Size_
{
public:
    typedef _Tp value_type;

    //! various constructors
    Size_();
    Size_(_Tp _width, _Tp _height);
    Size_(const Size_& sz);
    Size_(const Point_<_Tp>& pt);

    Size_& operator = (const Size_& sz);
    //! the area (width*height)
    _Tp area() const;

    //! conversion of another data type.
    template<typename _Tp2> operator Size_<_Tp2>() const;

    _Tp width, height; // the width and the height
};

//////////////////////////////// Rectangle ////////////////////////////////
/*!
  The 2D up-right rectangle class

  The class represents a 2D rectangle with coordinates of the specified data type.
  Normally, cv::Rect ~ cv::Rect_<int> is used.
*/
template<typename _Tp> class Rect_
{
public:
    typedef _Tp value_type;

    //! various constructors
    Rect_();
    Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    Rect_(const Rect_& r);
    Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz);
    Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2);

    Rect_& operator = ( const Rect_& r );
    //! the top-left corner
    Point_<_Tp> tl() const;
    //! the bottom-right corner
    Point_<_Tp> br() const;

    //! size (width, height) of the rectangle
    Size_<_Tp> size() const;
    //! area (width*height) of the rectangle
    _Tp area() const;

    //! conversion to another data type
    template<typename _Tp2> operator Rect_<_Tp2>() const;

    //! checks whether the rectangle contains the point
    bool contains(const Point_<_Tp>& pt) const;

    _Tp x, y, width, height; //< the top-left corner, as well as width and height of the rectangle
};

//////////////////////////////// Rotated Rectangle ////////////////////////////////
/*!
  The rotated 2D rectangle.

  The class represents rotated (i.e. not up-right) rectangles on a plane.
  Each rectangle is described by the center point (mass center), length of each side
  (represented by cv::Size2f structure) and the rotation angle in degrees.
*/
class RotatedRect
{
public:
    //! various constructors
    RotatedRect();
    RotatedRect(const Point2f& center, const Size2f& size, float angle);

    //! returns 4 vertices of the rectangle
    void points(Point2f pts[]) const;
    //! returns the minimal up-right rectangle containing the rotated rectangle
    Rect_<float> boundingRect() const;

    Point2f center; //< the rectangle mass center
    Size2f size;    //< width and height of the rectangle
    float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle.
};

//////////////////////////////// Range /////////////////////////////////
/*!
   The 2D range class

   This is the class used to specify a continuous subsequence, i.e. part of a contour, or a column span in a matrix.
*/
class Range
{
public:
    Range();
    Range(int _start, int _end);
    size_t size() const;
    bool empty() const;
    static Range all();

    int start, end;
};

////////////////////////////// Small Matrix (Matx<>) //////////////////////////////////
// @enum matrix decomposition types
enum { DECOMP_LU=0, DECOMP_SVD=1, DECOMP_EIG=2, DECOMP_CHOLESKY=3, DECOMP_QR=4, DECOMP_NORMAL=16 };
// @enum matrix norm types
enum { NORM_INF=1, NORM_L1=2, NORM_L2=4, NORM_L2SQR=5, NORM_HAMMING=6, NORM_HAMMING2=7, NORM_TYPE_MASK=7, NORM_RELATIVE=8, NORM_MINMAX=32 };
// @enum matrix logical operations types
enum { CMP_EQ=0, CMP_GT=1, CMP_GE=2, CMP_LT=3, CMP_LE=4, CMP_NE=5 };
enum { GEMM_1_T=1, GEMM_2_T=2, GEMM_3_T=4 };

template<typename _Tp, int m, int n> class Matx
{
public:
    typedef _Tp value_type;
    typedef Matx<_Tp, (m < n ? m : n), 1> diag_type;
    typedef Matx<_Tp, m, n> mat_type;

    enum { depth = DataDepth<_Tp>::value, rows = m, cols = n, channels = rows * cols,
           type = CV_MAKETYPE(depth, channels) };

    //! default constructor
    Matx();

    Matx(_Tp v0); //!< 1x1 matrix
    Matx(_Tp v0, _Tp v1); //!< 1x2 or 2x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2); //!< 1x3 or 3x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 1x4, 2x2 or 4x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 1x5 or 5x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 1x6, 2x3, 3x2 or 6x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 1x7 or 7x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 1x8, 2x4, 4x2 or 8x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 1x9, 3x3 or 9x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 1x10, 2x5 or 5x2 or 10x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11); //!< 1x12, 2x6, 3x4, 4x3, 6x2 or 12x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11,
         _Tp v12, _Tp v13, _Tp v14, _Tp v15); //!< 1x16, 4x4 or 16x1 matrix
    explicit Matx(const _Tp* vals); //!< initialize from a plain array

    static Matx all(_Tp alpha);
    static Matx zeros();
    static Matx ones();
    static Matx eye();
    static Matx diag(const diag_type& d);
    ///static Matx randu(_Tp a, _Tp b);
    ///static Matx randn(_Tp a, _Tp b);

    //! dot product computed with the default precision
    _Tp dot(const Matx<_Tp, m, n>& v) const;

    //! dot product computed in double-precision arithmetics
    double ddot(const Matx<_Tp, m, n>& v) const;

    //! conversion to another data type
    template<typename T2> operator Matx<T2, m, n>() const;

    //! change the matrix shape
    template<int m1, int n1> Matx<_Tp, m1, n1> reshape() const;

    //! extract part of the matrix
    template<int m1, int n1> Matx<_Tp, m1, n1> get_minor(int i, int j) const;

    //! extract the matrix row
    Matx<_Tp, 1, n> row(int i) const;

    //! extract the matrix column
    Matx<_Tp, m, 1> col(int i) const;

    //! extract the matrix diagonal
    diag_type diag() const;

    //! transpose the matrix
    Matx<_Tp, n, m> t() const;

    //! invert matrix the matrix
    Matx<_Tp, n, m> inv(int method=DECOMP_LU) const;

    //! solve linear system
    template<int l> Matx<_Tp, n, l> solve(const Matx<_Tp, m, l>& rhs, int flags=DECOMP_LU) const;
    Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const;

    //! multiply two matrices element-wise
    Matx<_Tp, m, n> mul(const Matx<_Tp, m, n>& a) const;

    //! element access
    const _Tp& operator ()(int i, int j) const;
    _Tp& operator ()(int i, int j);

    //! 1D element access
    const _Tp& operator ()(int k) const;
    _Tp& operator ()(int k);

    _Tp val[m*n]; //< matrix elements
};

////////////////////////////// Small Vector (Vec<>) //////////////////////////////////
template<typename _Tp, int cn> class Vec : public Matx<_Tp, cn, 1>
{
public:
    typedef _Tp value_type;

    //! default constructor
    Vec();

    Vec(_Tp v0); //!< 1-element vector constructor
    Vec(_Tp v0, _Tp v1); //!< 2-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2); //!< 3-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 4-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 5-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 6-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 7-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 8-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 9-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 10-element vector constructor
    explicit Vec(const _Tp* values);

    Vec(const Vec<_Tp, cn>& v);

    static Vec all(_Tp alpha);

    //! per-element multiplication
    Vec mul(const Vec<_Tp, cn>& v) const;

    //! conjugation (makes sense for complex numbers and quaternions)
    Vec conj() const;

    /*!
      cross product of the two 3D vectors.
      For other dimensionalities the exception is raised
    */
    Vec cross(const Vec& v) const;
    //! conversion to another data type
    template<typename T2> operator Vec<T2, cn>() const;

    /*! element access */
    const _Tp& operator [](int i) const;
    _Tp& operator[](int i);
    const _Tp& operator ()(int i) const;
    _Tp& operator ()(int i);
};

//////////////////////////////// Scalar ///////////////////////////////
/*!
   The template scalar class.

   This is partially specialized cv::Vec class with the number of elements = 4, i.e. a short vector of four elements.
   Normally, cv::Scalar ~ cv::Scalar_<double> is used.
*/
template<typename _Tp> class Scalar_ : public Vec<_Tp, 4>
{
public:
    //! various constructors
    Scalar_();
    Scalar_(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0);
    Scalar_(_Tp v0);

    //! returns a scalar with all elements set to v0
    static Scalar_<_Tp> all(_Tp v0);

    //! conversion to another data type
    template<typename T2> operator Scalar_<T2>() const;

    //! per-element product
    Scalar_<_Tp> mul(const Scalar_<_Tp>& t, double scale=1 ) const;

    // returns (v0, -v1, -v2, -v3)
    Scalar_<_Tp> conj() const;

    // returns true iff v1 == v2 == v3 == 0
    bool isReal() const;
};

/////////////////////////////// AutoBuffer ////////////////////////////////

/*!
 Automatically Allocated Buffer Class

 The class is used for temporary buffers in functions and methods.
 If a temporary buffer is usually small (a few K's of memory),
 but its size depends on the parameters, it makes sense to create a small
 fixed-size array on stack and use it if it's large enough. If the required buffer size
 is larger than the fixed size, another buffer of sufficient size is allocated dynamically
 and released after the processing. Therefore, in typical cases, when the buffer size is small,
 there is no overhead associated with malloc()/free().
 At the same time, there is no limit on the size of processed data.

 This is what AutoBuffer does. The template takes 2 parameters - type of the buffer elements and
 the number of stack-allocated elements. Here is how the class is used:

 \code
 void my_func(const cv::Mat& m)
 {
    cv::AutoBuffer<float, 1000> buf; // create automatic buffer containing 1000 floats

    buf.allocate(m.rows); // if m.rows <= 1000, the pre-allocated buffer is used,
                          // otherwise the buffer of "m.rows" floats will be allocated
                          // dynamically and deallocated in cv::AutoBuffer destructor
    ...
 }
 \endcode
*/
template<typename _Tp, size_t fixed_size=4096/sizeof(_Tp)+8> class AutoBuffer
{
public:
    typedef _Tp value_type;
    enum { buffer_padding = (int)((16 + sizeof(_Tp) - 1)/sizeof(_Tp)) };

    //! the default contructor
    AutoBuffer();
    //! constructor taking the real buffer size
    AutoBuffer(size_t _size);
    //! destructor. calls deallocate()
    ~AutoBuffer();

    //! allocates the new buffer of size _size. if the _size is small enough, stack-allocated buffer is used
    void allocate(size_t _size);
    //! deallocates the buffer if it was dynamically allocated
    void deallocate();
    //! returns pointer to the real buffer, stack-allocated or head-allocated
    operator _Tp* ();
    //! returns read-only pointer to the real buffer, stack-allocated or head-allocated
    operator const _Tp* () const;

protected:
    //! pointer to the real buffer, can point to buf if the buffer is small enough
    _Tp* ptr;
    //! size of the real buffer
    size_t size;
    //! pre-allocated buffer
    _Tp buf[fixed_size+buffer_padding];
};

//////////////////////// Input/Output Array Arguments /////////////////////////////////
class MatOp;    // defined in matexpr.hpp
class MatExpr;  // defined in matexpr.hpp

/*!
 Proxy datatype for passing Mat's and vector<>'s as input parameters
 */
class _InputArray
{
public:
    enum {
        KIND_SHIFT = 16,
        FIXED_TYPE = 0x8000 << KIND_SHIFT,
        FIXED_SIZE = 0x4000 << KIND_SHIFT,
        KIND_MASK = ~(FIXED_TYPE|FIXED_SIZE) - (1 << KIND_SHIFT) + 1,

        NONE              = 0 << KIND_SHIFT,
        MAT               = 1 << KIND_SHIFT,
        MATX              = 2 << KIND_SHIFT,
        STD_VECTOR        = 3 << KIND_SHIFT,
        STD_VECTOR_VECTOR = 4 << KIND_SHIFT,
        STD_VECTOR_MAT    = 5 << KIND_SHIFT,
        EXPR              = 6 << KIND_SHIFT,
        OPENGL_BUFFER     = 7 << KIND_SHIFT,
        OPENGL_TEXTURE    = 8 << KIND_SHIFT,
        GPU_MAT           = 9 << KIND_SHIFT,
        OCL_MAT           =10 << KIND_SHIFT
    };
    _InputArray();

    _InputArray(const Mat& m);
    _InputArray(const MatExpr& expr);
    _InputArray(const std::vector<Mat>& vec);
    _InputArray(const Scalar& s);
    _InputArray(const double& val);
    template<typename _Tp> _InputArray(const _Tp* vec, int n);
    template<typename _Tp> _InputArray(const std::vector<_Tp>& vec);
    template<typename _Tp> _InputArray(const std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp, int m, int n> _InputArray(const Matx<_Tp, m, n>& matx);

    virtual Mat getMat(int i=-1) const;
    virtual void getMatVector(std::vector<Mat>& mv) const;

    virtual int kind() const;
    virtual Size size(int i=-1) const;
    virtual size_t total(int i=-1) const;
    virtual int type(int i=-1) const;
    virtual int depth(int i=-1) const;
    virtual int channels(int i=-1) const;
    virtual bool empty() const;

#ifdef OPENCV_CAN_BREAK_BINARY_COMPATIBILITY
    virtual ~_InputArray();
#endif

    int flags;
    void* obj;
    Size sz;
};


enum
{
    DEPTH_MASK_8U = 1 << CV_8U,
    DEPTH_MASK_8S = 1 << CV_8S,
    DEPTH_MASK_16U = 1 << CV_16U,
    DEPTH_MASK_16S = 1 << CV_16S,
    DEPTH_MASK_32S = 1 << CV_32S,
    DEPTH_MASK_32F = 1 << CV_32F,
    DEPTH_MASK_64F = 1 << CV_64F,
    DEPTH_MASK_ALL = (DEPTH_MASK_64F<<1)-1,
    DEPTH_MASK_ALL_BUT_8S = DEPTH_MASK_ALL & ~DEPTH_MASK_8S,
    DEPTH_MASK_FLT = DEPTH_MASK_32F + DEPTH_MASK_64F
};


/*!
 Proxy datatype for passing Mat's and vector<>'s as input parameters
 */
class _OutputArray : public _InputArray
{
public:
    _OutputArray();

    _OutputArray(Mat& m);
    _OutputArray(std::vector<Mat>& vec);
    template<typename _Tp> _OutputArray(std::vector<_Tp>& vec);
    template<typename _Tp> _OutputArray(std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp, int m, int n> _OutputArray(Matx<_Tp, m, n>& matx);
    template<typename _Tp> _OutputArray(_Tp* vec, int n);

    _OutputArray(const Mat& m);
    _OutputArray(const std::vector<Mat>& vec);
    template<typename _Tp> _OutputArray(const std::vector<_Tp>& vec);
    template<typename _Tp> _OutputArray(const std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp, int m, int n> _OutputArray(const Matx<_Tp, m, n>& matx);
    template<typename _Tp> _OutputArray(const _Tp* vec, int n);

    virtual bool fixedSize() const;
    virtual bool fixedType() const;
    virtual bool needed() const;
    virtual Mat& getMatRef(int i=-1) const;
    virtual void create(Size sz, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    virtual void create(int rows, int cols, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    virtual void create(int dims, const int* size, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    virtual void release() const;
    virtual void clear() const;

#ifdef OPENCV_CAN_BREAK_BINARY_COMPATIBILITY
    virtual ~_OutputArray();
#endif
};

OutputArray noArray();

/////////////////////////////////////// Mat ///////////////////////////////////////////

enum { MAGIC_MASK=0xFFFF0000, TYPE_MASK=0x00000FFF, DEPTH_MASK=7 };

static inline size_t getElemSize(int type) { return CV_ELEM_SIZE(type); }

/*!
   Custom array allocator

*/
class MatAllocator
{
public:
    MatAllocator() {}
    virtual ~MatAllocator() {}
    virtual void allocate(int dims, const int* sizes, int type, int*& refcount,
                          uchar*& datastart, uchar*& data, size_t* step) = 0;
    virtual void deallocate(int* refcount, uchar* datastart, uchar* data) = 0;
};

/*!
   The n-dimensional matrix class.

   The class represents an n-dimensional dense numerical array that can act as
   a matrix, image, optical flow map, 3-focal tensor etc.
   It is very similar to CvMat and CvMatND types from earlier versions of OpenCV,
   and similarly to those types, the matrix can be multi-channel. It also fully supports ROI mechanism.

   There are many different ways to create cv::Mat object. Here are the some popular ones:
   <ul>
   <li> using cv::Mat::create(nrows, ncols, type) method or
     the similar constructor cv::Mat::Mat(nrows, ncols, type[, fill_value]) constructor.
     A new matrix of the specified size and specifed type will be allocated.
     "type" has the same meaning as in cvCreateMat function,
     e.g. CV_8UC1 means 8-bit single-channel matrix, CV_32FC2 means 2-channel (i.e. complex)
     floating-point matrix etc:

     \code
     // make 7x7 complex matrix filled with 1+3j.
     cv::Mat M(7,7,CV_32FC2,Scalar(1,3));
     // and now turn M to 100x60 15-channel 8-bit matrix.
     // The old content will be deallocated
     M.create(100,60,CV_8UC(15));
     \endcode

     As noted in the introduction of this chapter, Mat::create()
     will only allocate a new matrix when the current matrix dimensionality
     or type are different from the specified.

   <li> by using a copy constructor or assignment operator, where on the right side it can
     be a matrix or expression, see below. Again, as noted in the introduction,
     matrix assignment is O(1) operation because it only copies the header
     and increases the reference counter. cv::Mat::clone() method can be used to get a full
     (a.k.a. deep) copy of the matrix when you need it.

   <li> by constructing a header for a part of another matrix. It can be a single row, single column,
     several rows, several columns, rectangular region in the matrix (called a minor in algebra) or
     a diagonal. Such operations are also O(1), because the new header will reference the same data.
     You can actually modify a part of the matrix using this feature, e.g.

     \code
     // add 5-th row, multiplied by 3 to the 3rd row
     M.row(3) = M.row(3) + M.row(5)*3;

     // now copy 7-th column to the 1-st column
     // M.col(1) = M.col(7); // this will not work
     Mat M1 = M.col(1);
     M.col(7).copyTo(M1);

     // create new 320x240 image
     cv::Mat img(Size(320,240),CV_8UC3);
     // select a roi
     cv::Mat roi(img, Rect(10,10,100,100));
     // fill the ROI with (0,255,0) (which is green in RGB space);
     // the original 320x240 image will be modified
     roi = Scalar(0,255,0);
     \endcode

     Thanks to the additional cv::Mat::datastart and cv::Mat::dataend members, it is possible to
     compute the relative sub-matrix position in the main "container" matrix using cv::Mat::locateROI():

     \code
     Mat A = Mat::eye(10, 10, CV_32S);
     // extracts A columns, 1 (inclusive) to 3 (exclusive).
     Mat B = A(Range::all(), Range(1, 3));
     // extracts B rows, 5 (inclusive) to 9 (exclusive).
     // that is, C ~ A(Range(5, 9), Range(1, 3))
     Mat C = B(Range(5, 9), Range::all());
     Size size; Point ofs;
     C.locateROI(size, ofs);
     // size will be (width=10,height=10) and the ofs will be (x=1, y=5)
     \endcode

     As in the case of whole matrices, if you need a deep copy, use cv::Mat::clone() method
     of the extracted sub-matrices.

   <li> by making a header for user-allocated-data. It can be useful for
      <ol>
      <li> processing "foreign" data using OpenCV (e.g. when you implement
         a DirectShow filter or a processing module for gstreamer etc.), e.g.

         \code
         void process_video_frame(const unsigned char* pixels,
                                  int width, int height, int step)
         {
            cv::Mat img(height, width, CV_8UC3, pixels, step);
            cv::GaussianBlur(img, img, cv::Size(7,7), 1.5, 1.5);
         }
         \endcode

      <li> for quick initialization of small matrices and/or super-fast element access

         \code
         double m[3][3] = {{a, b, c}, {d, e, f}, {g, h, i}};
         cv::Mat M = cv::Mat(3, 3, CV_64F, m).inv();
         \endcode
      </ol>

       partial yet very common cases of this "user-allocated data" case are conversions
       from CvMat and IplImage to cv::Mat. For this purpose there are special constructors
       taking pointers to CvMat or IplImage and the optional
       flag indicating whether to copy the data or not.

       Backward conversion from cv::Mat to CvMat or IplImage is provided via cast operators
       cv::Mat::operator CvMat() an cv::Mat::operator IplImage().
       The operators do not copy the data.


       \code
       IplImage* img = cvLoadImage("greatwave.jpg", 1);
       Mat mtx(img); // convert IplImage* -> cv::Mat
       CvMat oldmat = mtx; // convert cv::Mat -> CvMat
       AVR_ASSERT(oldmat.cols == img->width && oldmat.rows == img->height &&
           oldmat.data.ptr == (uchar*)img->imageData && oldmat.step == img->widthStep);
       \endcode

   <li> by using MATLAB-style matrix initializers, cv::Mat::zeros(), cv::Mat::ones(), cv::Mat::eye(), e.g.:

   \code
   // create a double-precision identity martix and add it to M.
   M += Mat::eye(M.rows, M.cols, CV_64F);
   \endcode

   <li> by using comma-separated initializer:

   \code
   // create 3x3 double-precision identity matrix
   Mat M = (Mat_<double>(3,3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
   \endcode

   here we first call constructor of cv::Mat_ class (that we describe further) with the proper matrix,
   and then we just put "<<" operator followed by comma-separated values that can be constants,
   variables, expressions etc. Also, note the extra parentheses that are needed to avoid compiler errors.

   </ul>

   Once matrix is created, it will be automatically managed by using reference-counting mechanism
   (unless the matrix header is built on top of user-allocated data,
   in which case you should handle the data by yourself).
   The matrix data will be deallocated when no one points to it;
   if you want to release the data pointed by a matrix header before the matrix destructor is called,
   use cv::Mat::release().

   The next important thing to learn about the matrix class is element access. Here is how the matrix is stored.
   The elements are stored in row-major order (row by row). The cv::Mat::data member points to the first element of the first row,
   cv::Mat::rows contains the number of matrix rows and cv::Mat::cols - the number of matrix columns. There is yet another member,
   cv::Mat::step that is used to actually compute address of a matrix element. cv::Mat::step is needed because the matrix can be
   a part of another matrix or because there can some padding space in the end of each row for a proper alignment.

   \image html roi.png

   Given these parameters, address of the matrix element M_{ij} is computed as following:

   addr(M_{ij})=M.data + M.step*i + j*M.elemSize()

   if you know the matrix element type, e.g. it is float, then you can use cv::Mat::at() method:

   addr(M_{ij})=&M.at<float>(i,j)

   (where & is used to convert the reference returned by cv::Mat::at() to a pointer).
   if you need to process a whole row of matrix, the most efficient way is to get
   the pointer to the row first, and then just use plain C operator []:

   \code
   // compute sum of positive matrix elements
   // (assuming that M is double-precision matrix)
   double sum=0;
   for(int i = 0; i < M.rows; i++)
   {
       const double* Mi = M.ptr<double>(i);
       for(int j = 0; j < M.cols; j++)
           sum += std::max(Mi[j], 0.);
   }
   \endcode

   Some operations, like the above one, do not actually depend on the matrix shape,
   they just process elements of a matrix one by one (or elements from multiple matrices
   that are sitting in the same place, e.g. matrix addition). Such operations are called
   element-wise and it makes sense to check whether all the input/output matrices are continuous,
   i.e. have no gaps in the end of each row, and if yes, process them as a single long row:

   \code
   // compute sum of positive matrix elements, optimized variant
   double sum=0;
   int cols = M.cols, rows = M.rows;
   if(M.isContinuous())
   {
       cols *= rows;
       rows = 1;
   }
   for(int i = 0; i < rows; i++)
   {
       const double* Mi = M.ptr<double>(i);
       for(int j = 0; j < cols; j++)
           sum += std::max(Mi[j], 0.);
   }
   \endcode
   in the case of continuous matrix the outer loop body will be executed just once,
   so the overhead will be smaller, which will be especially noticeable in the case of small matrices.

   Finally, there are STL-style iterators that are smart enough to skip gaps between successive rows:
   \code
   // compute sum of positive matrix elements, iterator-based variant
   double sum=0;
   MatConstIterator_<double> it = M.begin<double>(), it_end = M.end<double>();
   for(; it != it_end; ++it)
       sum += std::max(*it, 0.);
   \endcode

   The matrix iterators are random-access iterators, so they can be passed
   to any STL algorithm, including std::sort().
*/
class Mat
{
public:
    //! default constructor
    Mat();
    //! constructs 2D matrix of the specified size and type
    // (_type is CV_8UC1, CV_64FC3, CV_32SC(12) etc.)
    Mat(int rows, int cols, int type);
    Mat(Size size, int type);
    //! constucts 2D matrix and fills it with the specified value _s.
    Mat(int rows, int cols, int type, const Scalar& s);
    Mat(Size size, int type, const Scalar& s);

    //! constructs n-dimensional matrix
    Mat(int ndims, const int* sizes, int type);
    Mat(int ndims, const int* sizes, int type, const Scalar& s);

    //! copy constructor
    Mat(const Mat& m);
    //! constructor for matrix headers pointing to user-allocated data
    Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);
    Mat(Size size, int type, void* data, size_t step=AUTO_STEP);
    Mat(int ndims, const int* sizes, int type, void* data, const size_t* steps=0);

    //! creates a matrix header for a part of the bigger matrix
    Mat(const Mat& m, const Range& rowRange, const Range& colRange=Range::all());
    Mat(const Mat& m, const Rect& roi);
    Mat(const Mat& m, const Range* ranges);
    //! builds matrix from std::vector with or without copying the data
    template<typename _Tp> explicit Mat(const std::vector<_Tp>& vec, bool copyData=false);
    //! builds matrix from cv::Vec; the data is copied by default
    template<typename _Tp, int n> explicit Mat(const Vec<_Tp, n>& vec, bool copyData=true);
    //! builds matrix from cv::Matx; the data is copied by default
    template<typename _Tp, int m, int n> explicit Mat(const Matx<_Tp, m, n>& mtx, bool copyData=true);
    //! builds matrix from a 2D point
    template<typename _Tp> explicit Mat(const Point_<_Tp>& pt, bool copyData=true);
    //! builds matrix from a 3D point
    template<typename _Tp> explicit Mat(const Point3_<_Tp>& pt, bool copyData=true);

    //! destructor - calls release()
    ~Mat();
    //! assignment operators
    Mat& operator = (const Mat& m);
    Mat& operator = (const MatExpr& expr);

    //! returns a new matrix header for the specified row
    Mat row(int y) const;
    //! returns a new matrix header for the specified column
    Mat col(int x) const;
    //! ... for the specified row span
    Mat rowRange(int startrow, int endrow) const;
    Mat rowRange(const Range& r) const;
    //! ... for the specified column span
    Mat colRange(int startcol, int endcol) const;
    Mat colRange(const Range& r) const;
    //! ... for the specified diagonal
    // (d=0 - the main diagonal,
    //  >0 - a diagonal from the lower half,
    //  <0 - a diagonal from the upper half)
    Mat diag(int d=0) const;
    //! constructs a square diagonal matrix which main diagonal is vector "d"
    static Mat diag(const Mat& d);

    //! returns deep copy of the matrix, i.e. the data is copied
    Mat clone() const;
    //! copies the matrix content to "m".
    // It calls m.create(this->size(), this->type()).
    void copyTo( OutputArray m ) const;
    //! copies those matrix elements to "m" that are marked with non-zero mask elements.
    void copyTo( OutputArray m, InputArray mask ) const;
    //! converts matrix to another datatype with optional scalng. See cvConvertScale.
    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;

    void assignTo( Mat& m, int type=-1 ) const;

    //! sets every matrix element to s
    Mat& operator = (const Scalar& s);
    //! sets some of the matrix elements to s, according to the mask
    Mat& setTo(InputArray value, InputArray mask=noArray());
    //! creates alternative matrix header for the same data, with different
    // number of channels and/or different number of rows. see cvReshape.
    Mat reshape(int cn, int rows=0) const;
    Mat reshape(int cn, int newndims, const int* newsz) const;

    //! matrix transposition by means of matrix expressions
    MatExpr t() const;
    //! matrix inversion by means of matrix expressions
    MatExpr inv(int method=DECOMP_LU) const;
    //! per-element matrix multiplication by means of matrix expressions
    MatExpr mul(InputArray m, double scale=1) const;

    //! computes cross-product of 2 3D vectors
    Mat cross(InputArray m) const;
    //! computes dot-product
    double dot(InputArray m) const;

    //! Matlab-style matrix initialization
    static MatExpr zeros(int rows, int cols, int type);
    static MatExpr zeros(Size size, int type);
    static MatExpr zeros(int ndims, const int* sz, int type);
    static MatExpr ones(int rows, int cols, int type);
    static MatExpr ones(Size size, int type);
    static MatExpr ones(int ndims, const int* sz, int type);
    static MatExpr eye(int rows, int cols, int type);
    static MatExpr eye(Size size, int type);

    //! allocates new matrix data unless the matrix already has specified size and type.
    // previous data is unreferenced if needed.
    void create(int rows, int cols, int type);
    void create(Size size, int type);
    void create(int ndims, const int* sizes, int type);

    //! increases the reference counter; use with care to avoid memleaks
    void addref();
    //! decreases reference counter;
    // deallocates the data when reference counter reaches 0.
    void release();

    //! deallocates the matrix data
    void deallocate();
    //! internal use function; properly re-allocates _size, _step arrays
    void copySize(const Mat& m);

    //! reserves enough space to fit sz hyper-planes
    void reserve(size_t sz);
    //! resizes matrix to the specified number of hyper-planes
    void resize(size_t sz);
    //! resizes matrix to the specified number of hyper-planes; initializes the newly added elements
    void resize(size_t sz, const Scalar& s);
    //! internal function
    void push_back_(const void* elem);
    //! adds element to the end of 1d matrix (or possibly multiple elements when _Tp=Mat)
    template<typename _Tp> void push_back(const _Tp& elem);
    void push_back(const Mat& m);
    //! removes several hyper-planes from bottom of the matrix
    void pop_back(size_t nelems=1);

    //! locates matrix header within a parent matrix. See below
    void locateROI( Size& wholeSize, Point& ofs ) const;
    //! moves/resizes the current matrix ROI inside the parent matrix.
    Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );
    //! extracts a rectangular sub-matrix
    // (this is a generalized form of row, rowRange etc.)
    Mat operator()( Range rowRange, Range colRange ) const;
    Mat operator()( const Rect& roi ) const;
    Mat operator()( const Range* ranges ) const;

    template<typename _Tp> operator std::vector<_Tp>() const;
    template<typename _Tp, int n> operator Vec<_Tp, n>() const;
    template<typename _Tp, int m, int n> operator Matx<_Tp, m, n>() const;

    //! returns true iff the matrix data is continuous
    // (i.e. when there are no gaps between successive rows).
    // similar to CV_IS_MAT_CONT(cvmat->type)
    bool isContinuous() const;

    //! returns true if the matrix is a submatrix of another matrix
    bool isSubmatrix() const;

    //! returns element size in bytes,
    // similar to CV_ELEM_SIZE(cvmat->type)
    size_t elemSize() const;
    //! returns the size of element channel in bytes.
    size_t elemSize1() const;
    //! returns element type, similar to CV_MAT_TYPE(cvmat->type)
    int type() const;
    //! returns element type, similar to CV_MAT_DEPTH(cvmat->type)
    int depth() const;
    //! returns element type, similar to CV_MAT_CN(cvmat->type)
    int channels() const;
    //! returns step/elemSize1()
    size_t step1(int i=0) const;
    //! returns true if matrix data is NULL
    bool empty() const;
    //! returns the total number of matrix elements
    size_t total() const;

    //! returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
    int checkVector(int elemChannels, int depth=-1, bool requireContinuous=true) const;

    //! returns pointer to i0-th submatrix along the dimension #0
    uchar* ptr(int i0=0);
    const uchar* ptr(int i0=0) const;

    //! returns pointer to (i0,i1) submatrix along the dimensions #0 and #1
    uchar* ptr(int i0, int i1);
    const uchar* ptr(int i0, int i1) const;

    //! returns pointer to (i0,i1,i3) submatrix along the dimensions #0, #1, #2
    uchar* ptr(int i0, int i1, int i2);
    const uchar* ptr(int i0, int i1, int i2) const;

    //! returns pointer to the matrix element
    uchar* ptr(const int* idx);
    //! returns read-only pointer to the matrix element
    const uchar* ptr(const int* idx) const;

    //! template version of the above method
    template<typename _Tp> _Tp* ptr(int i0=0);
    template<typename _Tp> const _Tp* ptr(int i0=0) const;

    template<typename _Tp> _Tp* ptr(int i0, int i1);
    template<typename _Tp> const _Tp* ptr(int i0, int i1) const;

    template<typename _Tp> _Tp* ptr(int i0, int i1, int i2);
    template<typename _Tp> const _Tp* ptr(int i0, int i1, int i2) const;

    //! the same as above, with the pointer dereferencing
    template<typename _Tp> _Tp& at(int i0=0);
    template<typename _Tp> const _Tp& at(int i0=0) const;

    template<typename _Tp> _Tp& at(int i0, int i1);
    template<typename _Tp> const _Tp& at(int i0, int i1) const;

    template<typename _Tp> _Tp& at(int i0, int i1, int i2);
    template<typename _Tp> const _Tp& at(int i0, int i1, int i2) const;

    template<typename _Tp> _Tp& at(const int* idx);
    template<typename _Tp> const _Tp& at(const int* idx) const;

    template<typename _Tp, int n> _Tp& at(const Vec<int, n>& idx);
    template<typename _Tp, int n> const _Tp& at(const Vec<int, n>& idx) const;

    //! special versions for 2D arrays (especially convenient for referencing image pixels)
    template<typename _Tp> _Tp& at(Point pt);
    template<typename _Tp> const _Tp& at(Point pt) const;

    enum { MAGIC_VAL=0x42FF0000, AUTO_STEP=0, CONTINUOUS_FLAG=CV_MAT_CONT_FLAG, SUBMATRIX_FLAG=CV_SUBMAT_FLAG };

    /*! includes several bit-fields:
         - the magic signature
         - continuity flag
         - depth
         - number of channels
     */
    int flags;
    //! the matrix dimensionality, >= 2
    int dims;
    //! the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
    int rows, cols;
    //! pointer to the data
    uchar* data;

    //! pointer to the reference counter;
    // when matrix points to user-allocated data, the pointer is NULL
    int* refcount;

    //! helper fields used in locateROI and adjustROI
    uchar* datastart;
    uchar* dataend;
    uchar* datalimit;

    //! custom allocator
    MatAllocator* allocator;

    struct MSize
    {
        MSize(int* _p);
        Size operator()() const;
        const int& operator[](int i) const;
        int& operator[](int i);
        operator const int*() const;
        bool operator == (const MSize& sz) const;
        bool operator != (const MSize& sz) const;

        int* p;
    };

    struct MStep
    {
        MStep();
        MStep(size_t s);
        const size_t& operator[](int i) const;
        size_t& operator[](int i);
        operator size_t() const;
        MStep& operator = (size_t s);

        size_t* p;
        size_t buf[2];
    protected:
        MStep& operator = (const MStep&);
    };

    MSize size;
    MStep step;

protected:
    void initEmpty();
};

/////////////////////////////// RNG ////////////////////////////////
/*!
   Random Number Generator

   The class implements RNG using Multiply-with-Carry algorithm
*/
class RNG
{
public:
    enum { UNIFORM=0, NORMAL=1 };

    RNG();
    RNG(uint64 state);
    //! updates the state and returns the next 32-bit unsigned integer random number
    unsigned next();

    operator uchar();
    operator schar();
    operator ushort();
    operator short();
    operator unsigned();
    //! returns a random integer sampled uniformly from [0, N).
    unsigned operator ()(unsigned N);
    unsigned operator ()();
    operator int();
    operator float();
    operator double();
    //! returns uniformly distributed integer random number from [a,b) range
    int uniform(int a, int b);
    //! returns uniformly distributed floating-point random number from [a,b) range
    float uniform(float a, float b);
    //! returns uniformly distributed double-precision floating-point random number from [a,b) range
    double uniform(double a, double b);

    uint64 state;
};

////////////////////////////////////// SVD //////////////////////////////////////////
/*!
    Singular Value Decomposition class

    The class is used to compute Singular Value Decomposition of a floating-point matrix and then
    use it to solve least-square problems, under-determined linear systems, invert matrices,
    compute condition numbers etc.

    For a bit faster operation you can pass flags=SVD::MODIFY_A|... to modify the decomposed matrix
    when it is not necessarily to preserve it. If you want to compute condition number of a matrix
    or absolute value of its determinant - you do not need SVD::u or SVD::vt,
    so you can pass flags=SVD::NO_UV|... . Another flag SVD::FULL_UV indicates that the full-size SVD::u and SVD::vt
    must be computed, which is not necessary most of the time.
*/
class SVD
{
public:
    enum { MODIFY_A=1, NO_UV=2, FULL_UV=4 };
    //! the default constructor
    SVD();
    //! the constructor that performs SVD
    SVD( InputArray src, int flags=0 );
    //! the operator that performs SVD. The previously allocated SVD::u, SVD::w are SVD::vt are released.
    SVD& operator ()( InputArray src, int flags=0 );

    //! decomposes matrix and stores the results to user-provided matrices
    static void compute( InputArray src, OutputArray w,
                         OutputArray u, OutputArray vt, int flags=0 );
    //! computes singular values of a matrix
    static void compute( InputArray src, OutputArray w, int flags=0 );
    //! performs back substitution
    static void backSubst( InputArray w, InputArray u,
                           InputArray vt, InputArray rhs,
                           OutputArray dst );

    template<typename _Tp, int m, int n, int nm> static void compute( const Matx<_Tp, m, n>& a,
        Matx<_Tp, nm, 1>& w, Matx<_Tp, m, nm>& u, Matx<_Tp, n, nm>& vt );
    template<typename _Tp, int m, int n, int nm> static void compute( const Matx<_Tp, m, n>& a,
        Matx<_Tp, nm, 1>& w );
    template<typename _Tp, int m, int n, int nm, int nb> static void backSubst( const Matx<_Tp, nm, 1>& w,
        const Matx<_Tp, m, nm>& u, const Matx<_Tp, n, nm>& vt, const Matx<_Tp, m, nb>& rhs, Matx<_Tp, n, nb>& dst );

    //! finds dst = arg min_{|dst|=1} |m*dst|
    static void solveZ( InputArray src, OutputArray dst );
    //! performs back substitution, so that dst is the solution or pseudo-solution of m*dst = rhs, where m is the decomposed matrix
    void backSubst( InputArray rhs, OutputArray dst ) const;

    Mat u, w, vt;
};

} // namespace cv

#endif // __cplusplus

// functions declarations
#include "functions.hpp"
// MatOp and MatExpr classes definitions
#include "expressions.hpp"

// templates and inline implementations
#include "impl/basic.tcc"
#include "impl/algebra.tcc"
#include "impl/geometry.tcc"
#include "impl/matrix.tcc"

#endif
