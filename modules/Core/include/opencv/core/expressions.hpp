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
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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

//  This file defines the classes
//    - MatOp
//    - MatExpr
//  and various Mat operations

#if !defined(AVR_CORE_EXPRESSIONS_HPP) && !defined(__OPENCV_CORE_MATRIX_OPERATIONS_HPP__)
#define AVR_CORE_EXPRESSIONS_HPP

#ifdef __cplusplus

namespace cv {

// begin mat.hpp
//////////////////////////////////// Matrix Expressions /////////////////////////////////////////
class MatOp
{
public:
    MatOp() {};
    virtual ~MatOp() {};

    virtual bool elementWise(const MatExpr& expr) const;
    virtual void assign(const MatExpr& expr, Mat& m, int type=-1) const = 0;
    virtual void roi(const MatExpr& expr, const Range& rowRange,
                     const Range& colRange, MatExpr& res) const;
    virtual void diag(const MatExpr& expr, int d, MatExpr& res) const;
    virtual void augAssignAdd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignSubtract(const MatExpr& expr, Mat& m) const;
    virtual void augAssignMultiply(const MatExpr& expr, Mat& m) const;
    virtual void augAssignDivide(const MatExpr& expr, Mat& m) const;
    virtual void augAssignAnd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignOr(const MatExpr& expr, Mat& m) const;
    virtual void augAssignXor(const MatExpr& expr, Mat& m) const;

    virtual void add(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void add(const MatExpr& expr1, const Scalar& s, MatExpr& res) const;

    virtual void subtract(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void subtract(const Scalar& s, const MatExpr& expr, MatExpr& res) const;

    virtual void multiply(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void multiply(const MatExpr& expr1, double s, MatExpr& res) const;

    virtual void divide(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void divide(double s, const MatExpr& expr, MatExpr& res) const;

    virtual void abs(const MatExpr& expr, MatExpr& res) const;

    virtual void transpose(const MatExpr& expr, MatExpr& res) const;
    virtual void matmul(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void invert(const MatExpr& expr, int method, MatExpr& res) const;

    virtual Size size(const MatExpr& expr) const;
    virtual int type(const MatExpr& expr) const;
};

class MatExpr
{
public:
    MatExpr() : op(0), flags(0), a(Mat()), b(Mat()), c(Mat()), alpha(0), beta(0), s(Scalar()) {}
    MatExpr(const MatOp* _op, int _flags, const Mat& _a=Mat(), const Mat& _b=Mat(),
            const Mat& _c=Mat(), double _alpha=1, double _beta=1, const Scalar& _s=Scalar())
        : op(_op), flags(_flags), a(_a), b(_b), c(_c), alpha(_alpha), beta(_beta), s(_s) {}
    explicit MatExpr(const Mat& m);
    operator Mat() const
    {
        Mat m;
        op->assign(*this, m);
        return m;
    }

    MatExpr row(int y) const;
    MatExpr col(int x) const;
    MatExpr diag(int d=0) const;
    MatExpr operator()( const Range& rowRange, const Range& colRange ) const;
    MatExpr operator()( const Rect& roi ) const;

    Mat cross(const Mat& m) const;
    double dot(const Mat& m) const;

    MatExpr t() const;
    MatExpr inv(int method = DECOMP_LU) const;
    MatExpr mul(const MatExpr& e, double scale=1) const;
    MatExpr mul(const Mat& m, double scale=1) const;

    Size size() const;
    int type() const;

    const MatOp* op;
    int flags;

    Mat a, b, c;
    double alpha, beta;
    Scalar s;
};

/// Operators
MatExpr operator + (const Mat& a, const Mat& b);
MatExpr operator + (const Mat& a, const Scalar& s);
MatExpr operator + (const Scalar& s, const Mat& a);
MatExpr operator + (const MatExpr& e, const Mat& m);
MatExpr operator + (const Mat& m, const MatExpr& e);
MatExpr operator + (const MatExpr& e, const Scalar& s);
MatExpr operator + (const Scalar& s, const MatExpr& e);
MatExpr operator + (const MatExpr& e1, const MatExpr& e2);

MatExpr operator - (const Mat& a, const Mat& b);
MatExpr operator - (const Mat& a, const Scalar& s);
MatExpr operator - (const Scalar& s, const Mat& a);
MatExpr operator - (const MatExpr& e, const Mat& m);
MatExpr operator - (const Mat& m, const MatExpr& e);
MatExpr operator - (const MatExpr& e, const Scalar& s);
MatExpr operator - (const Scalar& s, const MatExpr& e);
MatExpr operator - (const MatExpr& e1, const MatExpr& e2);

MatExpr operator - (const Mat& m);
MatExpr operator - (const MatExpr& e);

MatExpr operator * (const Mat& a, const Mat& b);
MatExpr operator * (const Mat& a, double s);
MatExpr operator * (double s, const Mat& a);
MatExpr operator * (const MatExpr& e, const Mat& m);
MatExpr operator * (const Mat& m, const MatExpr& e);
MatExpr operator * (const MatExpr& e, double s);
MatExpr operator * (double s, const MatExpr& e);
MatExpr operator * (const MatExpr& e1, const MatExpr& e2);

MatExpr operator / (const Mat& a, const Mat& b);
MatExpr operator / (const Mat& a, double s);
MatExpr operator / (double s, const Mat& a);
MatExpr operator / (const MatExpr& e, const Mat& m);
MatExpr operator / (const Mat& m, const MatExpr& e);
MatExpr operator / (const MatExpr& e, double s);
MatExpr operator / (double s, const MatExpr& e);
MatExpr operator / (const MatExpr& e1, const MatExpr& e2);

MatExpr operator < (const Mat& a, const Mat& b);
MatExpr operator < (const Mat& a, double s);
MatExpr operator < (double s, const Mat& a);

MatExpr operator <= (const Mat& a, const Mat& b);
MatExpr operator <= (const Mat& a, double s);
MatExpr operator <= (double s, const Mat& a);

MatExpr operator == (const Mat& a, const Mat& b);
MatExpr operator == (const Mat& a, double s);
MatExpr operator == (double s, const Mat& a);

MatExpr operator != (const Mat& a, const Mat& b);
MatExpr operator != (const Mat& a, double s);
MatExpr operator != (double s, const Mat& a);

MatExpr operator >= (const Mat& a, const Mat& b);
MatExpr operator >= (const Mat& a, double s);
MatExpr operator >= (double s, const Mat& a);

MatExpr operator > (const Mat& a, const Mat& b);
MatExpr operator > (const Mat& a, double s);
MatExpr operator > (double s, const Mat& a);

MatExpr operator & (const Mat& a, const Mat& b);
MatExpr operator & (const Mat& a, const Scalar& s);
MatExpr operator & (const Scalar& s, const Mat& a);

MatExpr operator | (const Mat& a, const Mat& b);
MatExpr operator | (const Mat& a, const Scalar& s);
MatExpr operator | (const Scalar& s, const Mat& a);

MatExpr operator ^ (const Mat& a, const Mat& b);
MatExpr operator ^ (const Mat& a, const Scalar& s);
MatExpr operator ^ (const Scalar& s, const Mat& a);

MatExpr operator ~(const Mat& m);

/// Functions
MatExpr min(const Mat& a, const Mat& b);
MatExpr min(const Mat& a, double s);
MatExpr min(double s, const Mat& a);

MatExpr max(const Mat& a, const Mat& b);
MatExpr max(const Mat& a, double s);
MatExpr max(double s, const Mat& a);

MatExpr abs(const Mat& m);
MatExpr abs(const MatExpr& e);

////////////////////////////// Augmenting algebraic operations //////////////////////////////////

static inline Mat& operator += (const Mat& a, const Mat& b)
{
    add(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator += (const Mat& a, const Scalar& s)
{
    add(a, s, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator += (const Mat& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator -= (const Mat& a, const Mat& b)
{
    subtract(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator -= (const Mat& a, const Scalar& s)
{
    subtract(a, s, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator -= (const Mat& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator *= (const Mat& a, const Mat& b)
{
    gemm(a, b, 1, Mat(), 0, (Mat&)a, 0);
    return (Mat&)a;
}

static inline Mat& operator *= (const Mat& a, double s)
{
    a.convertTo((Mat&)a, -1, s);
    return (Mat&)a;
}

static inline Mat& operator *= (const Mat& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator /= (const Mat& a, const Mat& b)
{
    divide(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator /= (const Mat& a, double s)
{
    a.convertTo((Mat&)a, -1, 1./s);
    return (Mat&)a;
}

////////////////////////////// Logical operations ///////////////////////////////

static inline Mat& operator &= (const Mat& a, const Mat& b)
{
    bitwise_and(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator &= (const Mat& a, const Scalar& s)
{
    bitwise_and(a, s, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator |= (const Mat& a, const Mat& b)
{
    bitwise_or(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator |= (const Mat& a, const Scalar& s)
{
    bitwise_or(a, s, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator ^= (const Mat& a, const Mat& b)
{
    bitwise_xor(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator ^= (const Mat& a, const Scalar& s)
{
    bitwise_xor(a, s, (Mat&)a);
    return (Mat&)a;
}
// end mat.hpp

} // namespace cv

#endif // __cplusplus

#endif
