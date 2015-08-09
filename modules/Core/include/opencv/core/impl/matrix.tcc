

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

//This file implements the classes:
//    - _InputArray
//    - _OutputArray
//    - Mat
//    - SVD

#if !defined(AVR_CORE_MATRIX_TCC) && !defined(__OPENCV_CORE_HPP__)
#define AVR_CORE_MATRIX_TCC

// begin operations.hpp
/////// exchange-add operation for atomic operations on reference counters ///////
#if defined __INTEL_COMPILER && !(defined WIN32 || defined _WIN32)   // atomic increment on the linux version of the Intel(tm) compiler
  #define CV_XADD(addr,delta) _InterlockedExchangeAdd(const_cast<void*>(reinterpret_cast<volatile void*>(addr)), delta)
#elif defined __GNUC__

  #if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__  && !defined(__CUDACC__)
    #ifdef __ATOMIC_SEQ_CST
        #define CV_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), (delta), __ATOMIC_SEQ_CST)
    #else
        #define CV_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), (delta), 5)
    #endif
  #elif __GNUC__*10 + __GNUC_MINOR__ >= 42

    #if !(defined WIN32 || defined _WIN32) && (defined __i486__ || defined __i586__ || \
        defined __i686__ || defined __MMX__ || defined __SSE__  || defined __ppc__) || \
        (defined __GNUC__ && defined _STLPORT_MAJOR) || \
        defined __EMSCRIPTEN__

      #define CV_XADD __sync_fetch_and_add
    #else
      #include <ext/atomicity.h>
      #define CV_XADD __gnu_cxx::__exchange_and_add
    #endif

  #else
    #include <bits/atomicity.h>
    #if __GNUC__*10 + __GNUC_MINOR__ >= 34
      #define CV_XADD __gnu_cxx::__exchange_and_add
    #else
      #define CV_XADD __exchange_and_add
    #endif
  #endif

#elif defined WIN32 || defined _WIN32 || defined WINCE
  namespace cv { CV_EXPORTS int _interlockedExchangeAdd(int* addr, int delta); }
  #define CV_XADD cv::_interlockedExchangeAdd

#else
  static inline int CV_XADD(int* addr, int delta)
  { int tmp = *addr; *addr += delta; return tmp; }
#endif
// end operations.hpp

#ifdef __cplusplus

namespace cv
{
// begin mat.hpp
/////////////////////////////// Input/Output Arrays /////////////////////////////////
template<typename _Tp> inline _InputArray::_InputArray(const std::vector<_Tp>& vec)
    : flags(FIXED_TYPE + STD_VECTOR + DataType<_Tp>::type), obj((void*)&vec) {}

template<typename _Tp> inline _InputArray::_InputArray(const std::vector<std::vector<_Tp> >& vec)
    : flags(FIXED_TYPE + STD_VECTOR_VECTOR + DataType<_Tp>::type), obj((void*)&vec) {}

template<typename _Tp, int m, int n> inline _InputArray::_InputArray(const Matx<_Tp, m, n>& mtx)
    : flags(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type), obj((void*)&mtx), sz(n, m) {}

template<typename _Tp> inline _InputArray::_InputArray(const _Tp* vec, int n)
    : flags(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type), obj((void*)vec), sz(n, 1) {}

inline _InputArray::_InputArray(const Scalar& s)
    : flags(FIXED_TYPE + FIXED_SIZE + MATX + CV_64F), obj((void*)&s), sz(1, 4) {}

template<typename _Tp> inline _OutputArray::_OutputArray(std::vector<_Tp>& vec)
    : _InputArray(vec) {}
template<typename _Tp> inline _OutputArray::_OutputArray(std::vector<std::vector<_Tp> >& vec)
    : _InputArray(vec) {}
template<typename _Tp, int m, int n> inline _OutputArray::_OutputArray(Matx<_Tp, m, n>& mtx)
    : _InputArray(mtx) {}
template<typename _Tp> inline _OutputArray::_OutputArray(_Tp* vec, int n)
    : _InputArray(vec, n) {}

template<typename _Tp> inline _OutputArray::_OutputArray(const std::vector<_Tp>& vec)
    : _InputArray(vec) {flags |= FIXED_SIZE;}
template<typename _Tp> inline _OutputArray::_OutputArray(const std::vector<std::vector<_Tp> >& vec)
    : _InputArray(vec) {flags |= FIXED_SIZE;}

template<typename _Tp, int m, int n> inline _OutputArray::_OutputArray(const Matx<_Tp, m, n>& mtx)
    : _InputArray(mtx) {}
template<typename _Tp> inline _OutputArray::_OutputArray(const _Tp* vec, int n)
    : _InputArray(vec, n) {}

//////////////////////////////// Mat ////////////////////////////////
inline void Mat::initEmpty()
{
    flags = MAGIC_VAL;
    dims = rows = cols = 0;
    data = datastart = dataend = datalimit = 0;
    refcount = 0;
    allocator = 0;
}

inline Mat::Mat() : size(&rows)
{ initEmpty(); }

inline Mat::Mat(int _rows, int _cols, int _type) : size(&rows)
{
    initEmpty();
    create(_rows, _cols, _type);
}

inline Mat::Mat(int _rows, int _cols, int _type, const Scalar& _s) : size(&rows)
{
    initEmpty();
    create(_rows, _cols, _type);
    *this = _s;
}

inline Mat::Mat(Size _sz, int _type) : size(&rows)
{
    initEmpty();
    create( _sz.height, _sz.width, _type );
}

inline Mat::Mat(Size _sz, int _type, const Scalar& _s) : size(&rows)
{
    initEmpty();
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

inline Mat::Mat(int _dims, const int* _sz, int _type) : size(&rows)
{
    initEmpty();
    create(_dims, _sz, _type);
}

inline Mat::Mat(int _dims, const int* _sz, int _type, const Scalar& _s) : size(&rows)
{
    initEmpty();
    create(_dims, _sz, _type);
    *this = _s;
}

inline Mat::Mat(const Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
    refcount(m.refcount), datastart(m.datastart), dataend(m.dataend),
    datalimit(m.datalimit), allocator(m.allocator), size(&rows)
{
    if( refcount )
        CV_XADD(refcount, 1);
    if( m.dims <= 2 )
    {
        step[0] = m.step[0]; step[1] = m.step[1];
    }
    else
    {
        dims = 0;
        copySize(m);
    }
}

inline Mat::Mat(int _rows, int _cols, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
    data((uchar*)_data), refcount(0), datastart((uchar*)_data), dataend(0),
    datalimit(0), allocator(0), size(&rows)
{
    size_t esz = CV_ELEM_SIZE(_type), minstep = cols*esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        AVR_DBG_ASSERT( _step >= minstep );
        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step; step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
}

inline Mat::Mat(Size _sz, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
    data((uchar*)_data), refcount(0), datastart((uchar*)_data), dataend(0),
    datalimit(0), allocator(0), size(&rows)
{
    size_t esz = CV_ELEM_SIZE(_type), minstep = cols*esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        AVR_DBG_ASSERT( _step >= minstep );
        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step; step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
}


template<typename _Tp> inline Mat::Mat(const std::vector<_Tp>& vec, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG),
    dims(2), rows((int)vec.size()), cols(1), data(0), refcount(0),
    datastart(0), dataend(0), allocator(0), size(&rows)
{
    if(vec.empty())
        return;
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        data = datastart = (uchar*)&vec[0];
        datalimit = dataend = datastart + rows*step[0];
    }
    else
        Mat((int)vec.size(), 1, DataType<_Tp>::type, (uchar*)&vec[0]).copyTo(*this);
}


template<typename _Tp, int n> inline Mat::Mat(const Vec<_Tp, n>& vec, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG),
    dims(2), rows(n), cols(1), data(0), refcount(0),
    datastart(0), dataend(0), allocator(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        data = datastart = (uchar*)vec.val;
        datalimit = dataend = datastart + rows*step[0];
    }
    else
        Mat(n, 1, DataType<_Tp>::type, (void*)vec.val).copyTo(*this);
}


template<typename _Tp, int m, int n> inline Mat::Mat(const Matx<_Tp,m,n>& M, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG),
    dims(2), rows(m), cols(n), data(0), refcount(0),
    datastart(0), dataend(0), allocator(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = cols*sizeof(_Tp);
        step[1] = sizeof(_Tp);
        data = datastart = (uchar*)M.val;
        datalimit = dataend = datastart + rows*step[0];
    }
    else
        Mat(m, n, DataType<_Tp>::type, (uchar*)M.val).copyTo(*this);
}


template<typename _Tp> inline Mat::Mat(const Point_<_Tp>& pt, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG),
    dims(2), rows(2), cols(1), data(0), refcount(0),
    datastart(0), dataend(0), allocator(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        data = datastart = (uchar*)&pt.x;
        datalimit = dataend = datastart + rows*step[0];
    }
    else
    {
        create(2, 1, DataType<_Tp>::type);
        ((_Tp*)data)[0] = pt.x;
        ((_Tp*)data)[1] = pt.y;
    }
}


template<typename _Tp> inline Mat::Mat(const Point3_<_Tp>& pt, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG),
    dims(2), rows(3), cols(1), data(0), refcount(0),
    datastart(0), dataend(0), allocator(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        data = datastart = (uchar*)&pt.x;
        datalimit = dataend = datastart + rows*step[0];
    }
    else
    {
        create(3, 1, DataType<_Tp>::type);
        ((_Tp*)data)[0] = pt.x;
        ((_Tp*)data)[1] = pt.y;
        ((_Tp*)data)[2] = pt.z;
    }
}

inline Mat::~Mat()
{
    release();
    if( step.p != step.buf )
        fastFree(step.p);
}

inline Mat& Mat::operator = (const Mat& m)
{
    if( this != &m )
    {
        if( m.refcount )
            CV_XADD(m.refcount, 1);
        release();
        flags = m.flags;
        if( dims <= 2 && m.dims <= 2 )
        {
            dims = m.dims;
            rows = m.rows;
            cols = m.cols;
            step[0] = m.step[0];
            step[1] = m.step[1];
        }
        else
            copySize(m);
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        refcount = m.refcount;
        allocator = m.allocator;
    }
    return *this;
}

inline Mat& Mat::operator = (const MatExpr& e)
{
    e.op->assign(e, *this);
    return *this;
}

inline Mat Mat::row(int y) const { return Mat(*this, Range(y, y+1), Range::all()); }
inline Mat Mat::col(int x) const { return Mat(*this, Range::all(), Range(x, x+1)); }
inline Mat Mat::rowRange(int startrow, int endrow) const
    { return Mat(*this, Range(startrow, endrow), Range::all()); }
inline Mat Mat::rowRange(const Range& r) const
    { return Mat(*this, r, Range::all()); }
inline Mat Mat::colRange(int startcol, int endcol) const
    { return Mat(*this, Range::all(), Range(startcol, endcol)); }
inline Mat Mat::colRange(const Range& r) const
    { return Mat(*this, Range::all(), r); }

inline Mat Mat::diag(const Mat& d)
{
    AVR_ASSERT( d.cols == 1 || d.rows == 1 );
    int len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0)), md = m.diag();
    if( d.cols == 1 )
        d.copyTo(md);
    else
        transpose(d, md);
    return m;
}

inline Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    return m;
}

inline void Mat::assignTo( Mat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
        convertTo(m, _type);
}

inline void Mat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type);
}

inline void Mat::create(Size _sz, int _type)
{
    create(_sz.height, _sz.width, _type);
}

inline void Mat::addref()
{ if( refcount ) CV_XADD(refcount, 1); }

inline void Mat::release()
{
    if( refcount && CV_XADD(refcount, -1) == 1 )
        deallocate();
    data = datastart = dataend = datalimit = 0;
    for(int i = 0; i < dims; i++)
        size.p[i] = 0;
    refcount = 0;
}

inline Mat Mat::operator()( Range _rowRange, Range _colRange ) const
{
    return Mat(*this, _rowRange, _colRange);
}

inline Mat Mat::operator()( const Rect& roi ) const
{ return Mat(*this, roi); }

inline Mat Mat::operator()(const Range* ranges) const
{
    return Mat(*this, ranges);
}

inline bool Mat::isContinuous() const { return (flags & CONTINUOUS_FLAG) != 0; }
inline bool Mat::isSubmatrix() const { return (flags & SUBMATRIX_FLAG) != 0; }
inline size_t Mat::elemSize() const { return dims > 0 ? step.p[dims-1] : 0; }
inline size_t Mat::elemSize1() const { return CV_ELEM_SIZE1(flags); }
inline int Mat::type() const { return CV_MAT_TYPE(flags); }
inline int Mat::depth() const { return CV_MAT_DEPTH(flags); }
inline int Mat::channels() const { return CV_MAT_CN(flags); }
inline size_t Mat::step1(int i) const { return step.p[i]/elemSize1(); }
inline bool Mat::empty() const { return data == 0 || total() == 0; }
inline size_t Mat::total() const
{
    if( dims <= 2 )
        return (size_t)rows*cols;
    size_t p = 1;
    for( int i = 0; i < dims; i++ )
        p *= size[i];
    return p;
}

inline uchar* Mat::ptr(int y)
{
    AVR_DBG_ASSERT( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0]*y;
}

inline const uchar* Mat::ptr(int y) const
{
    AVR_DBG_ASSERT( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0]*y;
}

template<typename _Tp> inline _Tp* Mat::ptr(int y)
{
    AVR_DBG_ASSERT( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return (_Tp*)(data + step.p[0]*y);
}

template<typename _Tp> inline const _Tp* Mat::ptr(int y) const
{
    AVR_DBG_ASSERT( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return (const _Tp*)(data + step.p[0]*y);
}


inline uchar* Mat::ptr(int i0, int i1)
{
    AVR_DBG_ASSERT( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return data + i0*step.p[0] + i1*step.p[1];
}

inline const uchar* Mat::ptr(int i0, int i1) const
{
    AVR_DBG_ASSERT( dims >= 2 && data &&
                 (unsigned)i0 < (unsigned)size.p[0] &&
                 (unsigned)i1 < (unsigned)size.p[1] );
    return data + i0*step.p[0] + i1*step.p[1];
}

template<typename _Tp> inline _Tp* Mat::ptr(int i0, int i1)
{
    AVR_DBG_ASSERT( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return (_Tp*)(data + i0*step.p[0] + i1*step.p[1]);
}

template<typename _Tp> inline const _Tp* Mat::ptr(int i0, int i1) const
{
    AVR_DBG_ASSERT( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return (const _Tp*)(data + i0*step.p[0] + i1*step.p[1]);
}

inline uchar* Mat::ptr(int i0, int i1, int i2)
{
    AVR_DBG_ASSERT( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return data + i0*step.p[0] + i1*step.p[1] + i2*step.p[2];
}

inline const uchar* Mat::ptr(int i0, int i1, int i2) const
{
    AVR_DBG_ASSERT( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return data + i0*step.p[0] + i1*step.p[1] + i2*step.p[2];
}

template<typename _Tp> inline _Tp* Mat::ptr(int i0, int i1, int i2)
{
    AVR_DBG_ASSERT( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return (_Tp*)(data + i0*step.p[0] + i1*step.p[1] + i2*step.p[2]);
}

template<typename _Tp> inline const _Tp* Mat::ptr(int i0, int i1, int i2) const
{
    AVR_DBG_ASSERT( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return (const _Tp*)(data + i0*step.p[0] + i1*step.p[1] + i2*step.p[2]);
}

inline uchar* Mat::ptr(const int* idx)
{
    int i, d = dims;
    uchar* p = data;
    AVR_DBG_ASSERT( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        AVR_DBG_ASSERT( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i]*step.p[i];
    }
    return p;
}

inline const uchar* Mat::ptr(const int* idx) const
{
    int i, d = dims;
    uchar* p = data;
    AVR_DBG_ASSERT( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        AVR_DBG_ASSERT( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i]*step.p[i];
    }
    return p;
}

template<typename _Tp> inline _Tp& Mat::at(int i0, int i1)
{
    AVR_DBG_ASSERT( dims <= 2 && data && (unsigned)i0 < (unsigned)size.p[0] &&
        (unsigned)(i1*DataType<_Tp>::channels) < (unsigned)(size.p[1]*channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0]*i0))[i1];
}

template<typename _Tp> inline const _Tp& Mat::at(int i0, int i1) const
{
    AVR_DBG_ASSERT( dims <= 2 && data && (unsigned)i0 < (unsigned)size.p[0] &&
        (unsigned)(i1*DataType<_Tp>::channels) < (unsigned)(size.p[1]*channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0]*i0))[i1];
}

template<typename _Tp> inline _Tp& Mat::at(Point pt)
{
    AVR_DBG_ASSERT( dims <= 2 && data && (unsigned)pt.y < (unsigned)size.p[0] &&
        (unsigned)(pt.x*DataType<_Tp>::channels) < (unsigned)(size.p[1]*channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0]*pt.y))[pt.x];
}

template<typename _Tp> inline const _Tp& Mat::at(Point pt) const
{
    AVR_DBG_ASSERT( dims <= 2 && data && (unsigned)pt.y < (unsigned)size.p[0] &&
        (unsigned)(pt.x*DataType<_Tp>::channels) < (unsigned)(size.p[1]*channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0]*pt.y))[pt.x];
}

template<typename _Tp> inline _Tp& Mat::at(int i0)
{
    AVR_DBG_ASSERT( dims <= 2 && data &&
                 (unsigned)i0 < (unsigned)(size.p[0]*size.p[1]) &&
                 elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    if( isContinuous() || size.p[0] == 1 )
        return ((_Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(_Tp*)(data + step.p[0]*i0);
    int i = i0/cols, j = i0 - i*cols;
    return ((_Tp*)(data + step.p[0]*i))[j];
}

template<typename _Tp> inline const _Tp& Mat::at(int i0) const
{
    AVR_DBG_ASSERT( dims <= 2 && data &&
                 (unsigned)i0 < (unsigned)(size.p[0]*size.p[1]) &&
                 elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    if( isContinuous() || size.p[0] == 1 )
        return ((const _Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(const _Tp*)(data + step.p[0]*i0);
    int i = i0/cols, j = i0 - i*cols;
    return ((const _Tp*)(data + step.p[0]*i))[j];
}

template<typename _Tp> inline _Tp& Mat::at(int i0, int i1, int i2)
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(i0, i1, i2);
}
template<typename _Tp> inline const _Tp& Mat::at(int i0, int i1, int i2) const
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(i0, i1, i2);
}
template<typename _Tp> inline _Tp& Mat::at(const int* idx)
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx);
}
template<typename _Tp> inline const _Tp& Mat::at(const int* idx) const
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx);
}
template<typename _Tp, int n> _Tp& Mat::at(const Vec<int, n>& idx)
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx.val);
}
template<typename _Tp, int n> inline const _Tp& Mat::at(const Vec<int, n>& idx) const
{
    AVR_DBG_ASSERT( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx.val);
}

template<typename _Tp> inline Mat::operator std::vector<_Tp>() const
{
    std::vector<_Tp> v;
    copyTo(v);
    return v;
}

template<typename _Tp, int n> inline Mat::operator Vec<_Tp, n>() const
{
    AVR_ASSERT( data && dims <= 2 && (rows == 1 || cols == 1) &&
               rows + cols - 1 == n && channels() == 1 );

    if( isContinuous() && type() == DataType<_Tp>::type )
        return Vec<_Tp, n>((_Tp*)data);
    Vec<_Tp, n> v; Mat tmp(rows, cols, DataType<_Tp>::type, v.val);
    convertTo(tmp, tmp.type());
    return v;
}

template<typename _Tp, int m, int n> inline Mat::operator Matx<_Tp, m, n>() const
{
    AVR_ASSERT( data && dims <= 2 && rows == m && cols == n && channels() == 1 );

    if( isContinuous() && type() == DataType<_Tp>::type )
        return Matx<_Tp, m, n>((_Tp*)data);
    Matx<_Tp, m, n> mtx; Mat tmp(rows, cols, DataType<_Tp>::type, mtx.val);
    convertTo(tmp, tmp.type());
    return mtx;
}


template<typename _Tp> inline void Mat::push_back(const _Tp& elem)
{
    if( !data )
    {
        AVR_ASSERT((type()==0) || (DataType<_Tp>::type == type()));

        *this = Mat(1, 1, DataType<_Tp>::type, (void*)&elem).clone();
        return;
    }
    AVR_ASSERT(DataType<_Tp>::type == type() && cols == 1
              /* && dims == 2 (cols == 1 implies dims == 2) */);
    uchar* tmp = dataend + step[0];
    if( !isSubmatrix() && isContinuous() && tmp <= datalimit )
    {
        *(_Tp*)(data + (size.p[0]++)*step.p[0]) = elem;
        dataend = tmp;
    }
    else
        push_back_(&elem);
}

inline Mat::MSize::MSize(int* _p) : p(_p) {}
inline Size Mat::MSize::operator()() const
{
    AVR_DBG_ASSERT(p[-1] <= 2);
    return Size(p[1], p[0]);
}
inline const int& Mat::MSize::operator[](int i) const { return p[i]; }
inline int& Mat::MSize::operator[](int i) { return p[i]; }
inline Mat::MSize::operator const int*() const { return p; }

inline bool Mat::MSize::operator == (const MSize& sz) const
{
    int d = p[-1], dsz = sz.p[-1];
    if( d != dsz )
        return false;
    if( d == 2 )
        return p[0] == sz.p[0] && p[1] == sz.p[1];

    for( int i = 0; i < d; i++ )
        if( p[i] != sz.p[i] )
            return false;
    return true;
}

inline bool Mat::MSize::operator != (const MSize& sz) const
{
    return !(*this == sz);
}

inline Mat::MStep::MStep() { p = buf; p[0] = p[1] = 0; }
inline Mat::MStep::MStep(size_t s) { p = buf; p[0] = s; p[1] = 0; }
inline const size_t& Mat::MStep::operator[](int i) const { return p[i]; }
inline size_t& Mat::MStep::operator[](int i) { return p[i]; }
inline Mat::MStep::operator size_t() const
{
    AVR_DBG_ASSERT( p == buf );
    return buf[0];
}
inline Mat::MStep& Mat::MStep::operator = (size_t s)
{
    AVR_DBG_ASSERT( p == buf );
    buf[0] = s;
    return *this;
}

///////////////////////////////////////////// SVD //////////////////////////////////////////////////////
inline SVD::SVD() {}
inline SVD::SVD( InputArray m, int flags ) { operator ()(m, flags); }
inline void SVD::solveZ( InputArray m, OutputArray _dst )
{
    Mat mtx = m.getMat();
    SVD svd(mtx, (mtx.rows >= mtx.cols ? 0 : SVD::FULL_UV));
    _dst.create(svd.vt.cols, 1, svd.vt.type());
    Mat dst = _dst.getMat();
    svd.vt.row(svd.vt.rows-1).reshape(1,svd.vt.cols).copyTo(dst);
}

template<typename _Tp, int m, int n, int nm> inline void
    SVD::compute( const Matx<_Tp, m, n>& a, Matx<_Tp, nm, 1>& w, Matx<_Tp, m, nm>& u, Matx<_Tp, n, nm>& vt )
{
    AVR_ASSERT( nm == MIN(m, n));
    Mat _a(a, false), _u(u, false), _w(w, false), _vt(vt, false);
    SVD::compute(_a, _w, _u, _vt);
    AVR_ASSERT(_w.data == (uchar*)&w.val[0] && _u.data == (uchar*)&u.val[0] && _vt.data == (uchar*)&vt.val[0]);
}

template<typename _Tp, int m, int n, int nm> inline void
SVD::compute( const Matx<_Tp, m, n>& a, Matx<_Tp, nm, 1>& w )
{
    AVR_ASSERT( nm == MIN(m, n));
    Mat _a(a, false), _w(w, false);
    SVD::compute(_a, _w);
    AVR_ASSERT(_w.data == (uchar*)&w.val[0]);
}

template<typename _Tp, int m, int n, int nm, int nb> inline void
SVD::backSubst( const Matx<_Tp, nm, 1>& w, const Matx<_Tp, m, nm>& u,
                const Matx<_Tp, n, nm>& vt, const Matx<_Tp, m, nb>& rhs,
                Matx<_Tp, n, nb>& dst )
{
    AVR_ASSERT( nm == MIN(m, n));
    Mat _u(u, false), _w(w, false), _vt(vt, false), _rhs(rhs, false), _dst(dst, false);
    SVD::backSubst(_w, _u, _vt, _rhs, _dst);
    AVR_ASSERT(_dst.data == (uchar*)&dst.val[0]);
}
// end mat.hpp

} //namespace cv

#endif // __cplusplus

#endif
