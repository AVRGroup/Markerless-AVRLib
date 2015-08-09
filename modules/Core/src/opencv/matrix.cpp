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

#include "precomp.hpp"

using namespace std;

// begin matrix.cpp
void cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.rows, b.rows);
    std::swap(a.cols, b.cols);
    std::swap(a.data, b.data);
    std::swap(a.refcount, b.refcount);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);

    std::swap(a.size.p, b.size.p);
    std::swap(a.step.p, b.step.p);
    std::swap(a.step.buf[0], b.step.buf[0]);
    std::swap(a.step.buf[1], b.step.buf[1]);

    if( a.step.p == b.step.buf )
    {
        a.step.p = a.step.buf;
        a.size.p = &a.rows;
    }

    if( b.step.p == a.step.buf )
    {
        b.step.p = b.step.buf;
        b.size.p = &b.rows;
    }
}

namespace cv {

static inline void setSize( Mat& m, int _dims, const int* _sz,
                            const size_t* _steps, bool autoSteps=false )
{
    AVR_ASSERT( 0 <= _dims && _dims <= CV_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m.step.p != m.step.buf )
        {
            fastFree(m.step.p);
            m.step.p = m.step.buf;
            m.size.p = &m.rows;
        }
        if( _dims > 2 )
        {
            m.step.p = (size_t*)fastMalloc(_dims*sizeof(m.step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m.step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m.rows = m.cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CV_ELEM_SIZE(m.flags), total = esz;
    int i;
    for( i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
        AVR_ASSERT( s >= 0 );
        m.size.p[i] = s;

        if( _steps )
            m.step.p[i] = i < _dims-1 ? _steps[i] : esz;
        else if( autoSteps )
        {
            m.step.p[i] = total;
            int64 total1 = (int64)total*s;
            if( (uint64)total1 != (size_t)total1 )
                AVR_ERROR(Cod::OutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 )
    {
        m.dims = 2;
        m.cols = 1;
        m.step[1] = esz;
    }
}

static void updateContinuityFlag(Mat& m)
{
    int i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m.step[j]*m.size[j] < m.step[j-1] )
            break;
    }

    uint64 t = (uint64)m.step[0]*m.size[0];
    if( j <= i && t == (size_t)t )
        m.flags |= Mat::CONTINUOUS_FLAG;
    else
        m.flags &= ~Mat::CONTINUOUS_FLAG;
}

static void finalizeHdr(Mat& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m.rows = m.cols = -1;
    if( m.data )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        if( m.size[0] > 0 )
        {
            m.dataend = m.data + m.size[d-1]*m.step[d-1];
            for( int i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        else
            m.dataend = m.datalimit;
    }
    else
        m.dataend = m.datalimit = 0;
}

void Mat::create(int d, const int* _sizes, int _type)
{
    int i;
    AVR_ASSERT(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
#ifdef HAVE_TGPU
        if( !allocator || allocator == tegra::getAllocator() ) allocator = tegra::getAllocator(d, _sizes, _type);
#endif
        if( !allocator )
        {
            size_t totalsize = alignSize(step.p[0]*size.p[0], (int)sizeof(*refcount));
            data = datastart = (uchar*)fastMalloc(totalsize + (int)sizeof(*refcount));
            refcount = (int*)(data + totalsize);
            *refcount = 1;
        }
        else
        {
#ifdef HAVE_TGPU
           try
            {
                allocator->allocate(dims, size, _type, refcount, datastart, data, step.p);
                AVR_ASSERT( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
            }catch(...)
            {
                allocator = 0;
                size_t totalSize = alignSize(step.p[0]*size.p[0], (int)sizeof(*refcount));
                data = datastart = (uchar*)fastMalloc(totalSize + (int)sizeof(*refcount));
                refcount = (int*)(data + totalSize);
                *refcount = 1;
            }
#else
            allocator->allocate(dims, size, _type, refcount, datastart, data, step.p);
            AVR_ASSERT( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
#endif
        }
    }

    finalizeHdr(*this);
}

void Mat::copySize(const Mat& m)
{
    setSize(*this, m.dims, 0, 0);
    for( int i = 0; i < dims; i++ )
    {
        size[i] = m.size[i];
        step[i] = m.step[i];
    }
}

void Mat::deallocate()
{
    if( allocator )
        allocator->deallocate(refcount, datastart, data);
    else
    {
        AVR_DBG_ASSERT(refcount != 0);
        fastFree(datastart);
    }
}


Mat::Mat(const Mat& m, const Range& _rowRange, const Range& _colRange) : size(&rows)
{
    initEmpty();
    AVR_ASSERT( m.dims >= 2 );
    if( m.dims > 2 )
    {
        AutoBuffer<Range> rs(m.dims);
        rs[0] = _rowRange;
        rs[1] = _colRange;
        for( int i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        return;
    }

    *this = m;
    if( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        AVR_ASSERT( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end && _rowRange.end <= m.rows );
        rows = _rowRange.size();
        data += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    if( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        AVR_ASSERT( 0 <= _colRange.start && _colRange.start <= _colRange.end && _colRange.end <= m.cols );
        cols = _colRange.size();
        data += _colRange.start*elemSize();
        flags &= cols < m.cols ? ~CONTINUOUS_FLAG : -1;
        flags |= SUBMATRIX_FLAG;
    }

    if( rows == 1 )
        flags |= CONTINUOUS_FLAG;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(const Mat& m, const Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m.step[0]), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), size(&rows)
{
    AVR_ASSERT( m.dims <= 2 );
    flags &= roi.width < m.cols ? ~CONTINUOUS_FLAG : -1;
    flags |= roi.height == 1 ? CONTINUOUS_FLAG : 0;

    size_t esz = CV_ELEM_SIZE(flags);
    data += roi.x*esz;
    AVR_ASSERT( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    if( refcount )
        CV_XADD(refcount, 1);
    if( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    step[0] = m.step[0]; step[1] = esz;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(int _dims, const int* _sizes, int _type, void* _data, const size_t* _steps) : size(&rows)
{
    initEmpty();
    flags |= CV_MAT_TYPE(_type);
    data = datastart = (uchar*)_data;
    setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


Mat::Mat(const Mat& m, const Range* ranges) : size(&rows)
{
    initEmpty();
    int i, d = m.dims;

    AVR_ASSERT(ranges);
    for( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        AVR_ASSERT( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    for( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        if( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

Mat Mat::diag(int d) const
{
    AVR_ASSERT( dims <= 2 );
    Mat m = *this;
    size_t esz = elemSize();
    int len;

    if( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    else
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    AVR_DBG_ASSERT( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    if( m.rows > 1 )
        m.flags &= ~CONTINUOUS_FLAG;
    else
        m.flags |= CONTINUOUS_FLAG;

    if( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    return m;
}

void Mat::pop_back(size_t nelems)
{
    AVR_ASSERT( nelems <= (size_t)size.p[0] );

    if( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (int)nelems);
    else
    {
        size.p[0] -= (int)nelems;
        dataend -= nelems*step.p[0];
        /*if( size.p[0] <= 1 )
        {
            if( dims <= 2 )
                flags |= CONTINUOUS_FLAG;
            else
                updateContinuityFlag(*this);
        }*/
    }
}

void Mat::push_back_(const void* elem)
{
    int r = size.p[0];
    if( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = r + 1;
    dataend += step.p[0];
    if( esz < step.p[0] )
        flags &= ~CONTINUOUS_FLAG;
}

void Mat::locateROI( Size& wholeSize, Point& ofs ) const
{
    AVR_ASSERT( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    if( delta1 == 0 )
        ofs.x = ofs.y = 0;
    else
    {
        ofs.y = (int)(delta1/step[0]);
        ofs.x = (int)((delta1 - step[0]*ofs.y)/esz);
        AVR_DBG_ASSERT( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (int)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (int)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( int dtop, int dbottom, int dleft, int dright )
{
    AVR_ASSERT( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    int row1 = std::max(ofs.y - dtop, 0), row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);
    int col1 = std::max(ofs.x - dleft, 0), col2 = std::min(ofs.x + cols + dright, wholeSize.width);
    data += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    if( esz*cols == step[0] || rows == 1 )
        flags |= CONTINUOUS_FLAG;
    else
        flags &= ~CONTINUOUS_FLAG;
    return *this;
}

void Mat::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    AVR_ASSERT( (int)nelems >= 0 );
    if( !isSubmatrix() && data + step.p[0]*nelems <= datalimit )
        return;

    int r = size.p[0];

    if( (size_t)r >= nelems )
        return;

    size.p[0] = std::max((int)nelems, 1);
    size_t newsize = total()*elemSize();

    if( newsize < MIN_SIZE )
        size.p[0] = (int)((MIN_SIZE + newsize - 1)*nelems/newsize);

    Mat m(dims, size.p, type());
    size.p[0] = r;
    if( r > 0 )
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;
    dataend = data + step.p[0]*r;
}


void Mat::resize(size_t nelems)
{
    int saveRows = size.p[0];
    if( saveRows == (int)nelems )
        return;
    AVR_ASSERT( (int)nelems >= 0 );

    if( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (int)nelems;
    dataend += (size.p[0] - saveRows)*step.p[0];

    //updateContinuityFlag(*this);
}


void Mat::resize(size_t nelems, const Scalar& s)
{
    int saveRows = size.p[0];
    resize(nelems);

    if( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

void Mat::push_back(const Mat& elems)
{
    int r = size.p[0], delta = elems.size.p[0];
    if( delta == 0 )
        return;
    if( this == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        return;
    }
    if( !data )
    {
        *this = elems.clone();
        return;
    }

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = r;
    if( !eq )
        AVR_ERROR(Cod::Unknown, "Unmatched sizes between Mat's");
    if( type() != elems.type() )
        AVR_ERROR(Cod::Unknown, "Unmatched formats between Mat's");

    if( isSubmatrix() || dataend + step.p[0]*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += delta;
    dataend += step.p[0]*delta;

    //updateContinuityFlag(*this);

    if( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step.p[0], elems.data, elems.total()*elems.elemSize());
    else
    {
        Mat part = rowRange(r, r + delta);
        elems.copyTo(part);
    }
}

Mat Mat::reshape(int new_cn, int new_rows) const
{
    int cn = channels();
    Mat hdr = *this;

    if( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        return hdr;
    }

    AVR_ASSERT( dims <= 2 );

    if( new_cn == 0 )
        new_cn = cn;

    int total_width = cols * cn;

    if( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    if( new_rows != 0 && new_rows != rows )
    {
        int total_size = total_width * rows;
        if( !isContinuous() )
            AVR_ERROR(Cod::Unknown,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        if( (unsigned)new_rows > (unsigned)total_size )
            AVR_ERROR(Cod::OutOfRange, "Bad new number of rows" );

        total_width = total_size / new_rows;

        if( total_width * new_rows != total_size )
            AVR_ERROR(Cod::FunctionArgument, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    }

    int new_width = total_width / new_cn;

    if( new_width * new_cn != total_width )
        AVR_ERROR(Cod::FunctionArgument,
        "The total width is not divisible by the new number of channels" );

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    return hdr;
}


int Mat::checkVector(int _elemChannels, int _depth, bool _requireContinuous) const
{
    return (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims == 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (int)(total()*channels()/_elemChannels) : -1;
}

} // namespace cv

void cv::scalarToRawData(const Scalar& s, void* _buf, int type, int unroll_to)
{
    int i, depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    AVR_ASSERT(cn <= 4);
    switch(depth)
    {
    case CV_8U:
        {
        uchar* buf = (uchar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<uchar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_8S:
        {
        schar* buf = (schar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<schar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_16U:
        {
        ushort* buf = (ushort*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<ushort>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_16S:
        {
        short* buf = (short*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<short>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_32S:
        {
        int* buf = (int*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<int>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_32F:
        {
        float* buf = (float*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<float>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_64F:
        {
        double* buf = (double*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<double>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        break;
        }
    default:
        AVR_ERROR(Cod::UnsupportedFormat,"");
    }
}

namespace cv {
/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

_InputArray::_InputArray() : flags(0), obj(0) {}
#ifdef OPENCV_CAN_BREAK_BINARY_COMPATIBILITY
_InputArray::~_InputArray() {}
#endif
_InputArray::_InputArray(const Mat& m) : flags(MAT), obj((void*)&m) {}
_InputArray::_InputArray(const vector<Mat>& vec) : flags(STD_VECTOR_MAT), obj((void*)&vec) {}
_InputArray::_InputArray(const double& val) : flags(FIXED_TYPE + FIXED_SIZE + MATX + CV_64F), obj((void*)&val), sz(Size(1,1)) {}
_InputArray::_InputArray(const MatExpr& expr) : flags(FIXED_TYPE + FIXED_SIZE + EXPR), obj((void*)&expr) {}

Mat _InputArray::getMat(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        if( i < 0 )
            return *m;
        return m->row(i);
    }

    if( k == EXPR )
    {
        AVR_ASSERT( i < 0 );
        return (Mat)*((const MatExpr*)obj);
    }

    if( k == MATX )
    {
        AVR_ASSERT( i < 0 );
        return Mat(sz, flags, obj);
    }

    if( k == STD_VECTOR )
    {
        AVR_ASSERT( i < 0 );
        int t = CV_MAT_TYPE(flags);
        const vector<uchar>& v = *(const vector<uchar>*)obj;

        return !v.empty() ? Mat(size(), t, (void*)&v[0]) : Mat();
    }

    if( k == NONE )
        return Mat();

    if( k == STD_VECTOR_VECTOR )
    {
        int t = type(i);
        const vector<vector<uchar> >& vv = *(const vector<vector<uchar> >*)obj;
        AVR_ASSERT( 0 <= i && i < (int)vv.size() );
        const vector<uchar>& v = vv[i];

        return !v.empty() ? Mat(size(i), t, (void*)&v[0]) : Mat();
    }

    if( k == OCL_MAT )
    {
        AVR_ERROR(Cod::NotImplemented, "This method is not implemented for oclMat yet");
    }

    AVR_ASSERT( k == STD_VECTOR_MAT );
    //if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& v = *(const vector<Mat>*)obj;
        AVR_ASSERT( 0 <= i && i < (int)v.size() );

        return v[i];
    }
}


void _InputArray::getMatVector(vector<Mat>& mv) const
{
    int k = kind();

    if( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        int i, n = (int)m.size[0];
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = m.dims == 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        return;
    }

    if( k == EXPR )
    {
        Mat m = *(const MatExpr*)obj;
        int i, n = m.size[0];
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = m.row(i);
        return;
    }

    if( k == MATX )
    {
        size_t i, n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        return;
    }

    if( k == STD_VECTOR )
    {
        const vector<uchar>& v = *(const vector<uchar>*)obj;

        size_t i, n = v.size(), esz = CV_ELEM_SIZE(flags);
        int t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(&v[0] + esz*i));
        return;
    }

    if( k == NONE )
    {
        mv.clear();
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        const vector<vector<uchar> >& vv = *(const vector<vector<uchar> >*)obj;
        int i, n = (int)vv.size();
        int t = CV_MAT_TYPE(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
        {
            const vector<uchar>& v = vv[i];
            mv[i] = Mat(size(i), t, (void*)&v[0]);
        }
        return;
    }

    AVR_ASSERT( k == STD_VECTOR_MAT );
    //if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& v = *(const vector<Mat>*)obj;
        mv.resize(v.size());
        std::copy(v.begin(), v.end(), mv.begin());
        return;
    }
}

int _InputArray::kind() const
{
    return flags & KIND_MASK;
}

Size _InputArray::size(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        AVR_ASSERT( i < 0 );
        return ((const Mat*)obj)->size();
    }

    if( k == EXPR )
    {
        AVR_ASSERT( i < 0 );
        return ((const MatExpr*)obj)->size();
    }

    if( k == MATX )
    {
        AVR_ASSERT( i < 0 );
        return sz;
    }

    if( k == STD_VECTOR )
    {
        AVR_ASSERT( i < 0 );
        const vector<uchar>& v = *(const vector<uchar>*)obj;
        const vector<int>& iv = *(const vector<int>*)obj;
        size_t szb = v.size(), szi = iv.size();
        return szb == szi ? Size((int)szb, 1) : Size((int)(szb/CV_ELEM_SIZE(flags)), 1);
    }

    if( k == NONE )
        return Size();

    if( k == STD_VECTOR_VECTOR )
    {
        const vector<vector<uchar> >& vv = *(const vector<vector<uchar> >*)obj;
        if( i < 0 )
            return vv.empty() ? Size() : Size((int)vv.size(), 1);
        AVR_ASSERT( i < (int)vv.size() );
        const vector<vector<int> >& ivv = *(const vector<vector<int> >*)obj;

        size_t szb = vv[i].size(), szi = ivv[i].size();
        return szb == szi ? Size((int)szb, 1) : Size((int)(szb/CV_ELEM_SIZE(flags)), 1);
    }

    if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& vv = *(const vector<Mat>*)obj;
        if( i < 0 )
            return vv.empty() ? Size() : Size((int)vv.size(), 1);
        AVR_ASSERT( i < (int)vv.size() );

        return vv[i].size();
    }

    return Size();
}

size_t _InputArray::total(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        AVR_ASSERT( i < 0 );
        return ((const Mat*)obj)->total();
    }

    if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& vv = *(const vector<Mat>*)obj;
        if( i < 0 )
            return vv.size();

        AVR_ASSERT( i < (int)vv.size() );
        return vv[i].total();
    }

    return size(i).area();
}

int _InputArray::type(int i) const
{
    int k = kind();

    if( k == MAT )
        return ((const Mat*)obj)->type();

    if( k == EXPR )
        return ((const MatExpr*)obj)->type();

    if( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR )
        return CV_MAT_TYPE(flags);

    if( k == NONE )
        return -1;

    if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& vv = *(const vector<Mat>*)obj;
        AVR_ASSERT( i < (int)vv.size() );

        return vv[i >= 0 ? i : 0].type();
    }

    return -1;
}

int _InputArray::depth(int i) const
{
    return CV_MAT_DEPTH(type(i));
}

int _InputArray::channels(int i) const
{
    return CV_MAT_CN(type(i));
}

bool _InputArray::empty() const
{
    int k = kind();

    if( k == MAT )
        return ((const Mat*)obj)->empty();

    if( k == EXPR )
        return false;

    if( k == MATX )
        return false;

    if( k == STD_VECTOR )
    {
        const vector<uchar>& v = *(const vector<uchar>*)obj;
        return v.empty();
    }

    if( k == NONE )
        return true;

    if( k == STD_VECTOR_VECTOR )
    {
        const vector<vector<uchar> >& vv = *(const vector<vector<uchar> >*)obj;
        return vv.empty();
    }

    if( k == STD_VECTOR_MAT )
    {
        const vector<Mat>& vv = *(const vector<Mat>*)obj;
        return vv.empty();
    }

    return true;
}


_OutputArray::_OutputArray() {}
#ifdef OPENCV_CAN_BREAK_BINARY_COMPATIBILITY
_OutputArray::~_OutputArray() {}
#endif
_OutputArray::_OutputArray(Mat& m) : _InputArray(m) {}
_OutputArray::_OutputArray(vector<Mat>& vec) : _InputArray(vec) {}

_OutputArray::_OutputArray(const Mat& m) : _InputArray(m) {flags |= FIXED_SIZE|FIXED_TYPE;}
_OutputArray::_OutputArray(const vector<Mat>& vec) : _InputArray(vec) {flags |= FIXED_SIZE;}


bool _OutputArray::fixedSize() const
{
    return (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    return (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create(Size _sz, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        AVR_ASSERT(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        AVR_ASSERT(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        return;
    }
    int sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int rows, int cols, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        AVR_ASSERT(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(cols, rows));
        AVR_ASSERT(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(rows, cols, mtype);
        return;
    }
    int sizes[] = {rows, cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int dims, const int* sizes, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    mtype = CV_MAT_TYPE(mtype);

    if( k == MAT )
    {
        AVR_ASSERT( i < 0 );
        Mat& m = *(Mat*)obj;
        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                AVR_ASSERT(!fixedType() && !fixedSize());
                m.release();
            }

            if( dims == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                AVR_ASSERT(CV_MAT_TYPE(mtype) == m.type());
        }
        if(fixedSize())
        {
            AVR_ASSERT(m.dims == dims);
            for(int j = 0; j < dims; ++j)
                AVR_ASSERT(m.size[j] == sizes[j]);
        }
        m.create(dims, sizes, mtype);
        return;
    }

    if( k == MATX )
    {
        AVR_ASSERT( i < 0 );
        int type0 = CV_MAT_TYPE(flags);
        AVR_ASSERT( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        AVR_ASSERT( dims == 2 && ((sizes[0] == sz.height && sizes[1] == sz.width) ||
                                 (allowTransposed && sizes[0] == sz.width && sizes[1] == sz.height)));
        return;
    }

    if( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        AVR_ASSERT( dims == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
        size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0;
        vector<uchar>* v = (vector<uchar>*)obj;

        if( k == STD_VECTOR_VECTOR )
        {
            vector<vector<uchar> >& vv = *(vector<vector<uchar> >*)obj;
            if( i < 0 )
            {
                AVR_ASSERT(!fixedSize() || len == vv.size());
                vv.resize(len);
                return;
            }
            AVR_ASSERT( i < (int)vv.size() );
            v = &vv[i];
        }
        else
            AVR_ASSERT( i < 0 );

        int type0 = CV_MAT_TYPE(flags);
        AVR_ASSERT( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );

        int esz = CV_ELEM_SIZE(type0);
        AVR_ASSERT(!fixedSize() || len == ((vector<uchar>*)v)->size() / esz);
        switch( esz )
        {
        case 1:
            ((vector<uchar>*)v)->resize(len);
            break;
        case 2:
            ((vector<Vec2b>*)v)->resize(len);
            break;
        case 3:
            ((vector<Vec3b>*)v)->resize(len);
            break;
        case 4:
            ((vector<int>*)v)->resize(len);
            break;
        case 6:
            ((vector<Vec3s>*)v)->resize(len);
            break;
        case 8:
            ((vector<Vec2i>*)v)->resize(len);
            break;
        case 12:
            ((vector<Vec3i>*)v)->resize(len);
            break;
        case 16:
            ((vector<Vec4i>*)v)->resize(len);
            break;
        case 24:
            ((vector<Vec6i>*)v)->resize(len);
            break;
        case 32:
            ((vector<Vec8i>*)v)->resize(len);
            break;
        case 36:
            ((vector<Vec<int, 9> >*)v)->resize(len);
            break;
        case 48:
            ((vector<Vec<int, 12> >*)v)->resize(len);
            break;
        case 64:
            ((vector<Vec<int, 16> >*)v)->resize(len);
            break;
        case 128:
            ((vector<Vec<int, 32> >*)v)->resize(len);
            break;
        case 256:
            ((vector<Vec<int, 64> >*)v)->resize(len);
            break;
        case 512:
            ((vector<Vec<int, 128> >*)v)->resize(len);
            break;
        default:
            stringstream stream;
            stream << "Vectors with element size " << esz << " are not supported. Please, modify OutputArray::create()";
            AVR_ERROR(Cod::FunctionArgument, stream.str());
        }
        return;
    }

    if( k == NONE )
    {
        AVR_ERROR(Cod::NullPointer, "create() called for the missing output array" );
        return;
    }

    AVR_ASSERT( k == STD_VECTOR_MAT );
    //if( k == STD_VECTOR_MAT )
    {
        vector<Mat>& v = *(vector<Mat>*)obj;

        if( i < 0 )
        {
            AVR_ASSERT( dims == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            AVR_ASSERT(!fixedSize() || len == len0);
            v.resize(len);
            if( fixedType() )
            {
                int _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    if( v[j].type() == _type )
                        continue;
                    AVR_ASSERT( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            return;
        }

        AVR_ASSERT( i < (int)v.size() );
        Mat& m = v[i];

        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                AVR_ASSERT(!fixedType() && !fixedSize());
                m.release();
            }

            if( dims == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                AVR_ASSERT(!fixedType() || (CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0));
        }
        if(fixedSize())
        {
            AVR_ASSERT(m.dims == dims);
            for(int j = 0; j < dims; ++j)
                AVR_ASSERT(m.size[j] == sizes[j]);
        }

        m.create(dims, sizes, mtype);
    }
}

void _OutputArray::release() const
{
    AVR_ASSERT(!fixedSize());

    int k = kind();

    if( k == MAT )
    {
        ((Mat*)obj)->release();
        return;
    }

    if( k == NONE )
        return;

    if( k == STD_VECTOR )
    {
        create(Size(), CV_MAT_TYPE(flags));
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        ((vector<vector<uchar> >*)obj)->clear();
        return;
    }

    AVR_ASSERT( k == STD_VECTOR_MAT );
    //if( k == STD_VECTOR_MAT )
    {
        ((vector<Mat>*)obj)->clear();
    }
}

void _OutputArray::clear() const
{
    int k = kind();

    if( k == MAT )
    {
        AVR_ASSERT(!fixedSize());
        ((Mat*)obj)->resize(0);
        return;
    }

    release();
}

bool _OutputArray::needed() const
{
    return kind() != NONE;
}

Mat& _OutputArray::getMatRef(int i) const
{
    int k = kind();
    if( i < 0 )
    {
        AVR_ASSERT( k == MAT );
        return *(Mat*)obj;
    }
    else
    {
        AVR_ASSERT( k == STD_VECTOR_MAT );
        vector<Mat>& v = *(vector<Mat>*)obj;
        AVR_ASSERT( i < (int)v.size() );
        return v[i];
    }
}

static _OutputArray _none;
OutputArray noArray() { return _none; }

} // namespace cv

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

//////////////////////////////////////// set identity ////////////////////////////////////////////
void cv::setIdentity( InputOutputArray _m, const Scalar& s )
{
    Mat m = _m.getMat();
    AVR_ASSERT( m.dims <= 2 );
    int i, j, rows = m.rows, cols = m.cols, type = m.type();

    if( type == CV_32FC1 )
    {
        float* data = (float*)m.data;
        float val = (float)s[0];
        size_t step = m.step/sizeof(data[0]);

        for( i = 0; i < rows; i++, data += step )
        {
            for( j = 0; j < cols; j++ )
                data[j] = 0;
            if( i < cols )
                data[i] = val;
        }
    }
    else if( type == CV_64FC1 )
    {
        double* data = (double*)m.data;
        double val = s[0];
        size_t step = m.step/sizeof(data[0]);

        for( i = 0; i < rows; i++, data += step )
        {
            for( j = 0; j < cols; j++ )
                data[j] = j == i ? val : 0;
        }
    }
    else
    {
        m = Scalar(0);
        m.diag() = s;
    }
}

////////////////////////////////////// transpose /////////////////////////////////////////

namespace cv {

template<typename T> static void
transpose_( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz )
{
    int i=0, j, m = sz.width, n = sz.height;

    #if CV_ENABLE_UNROLLED
    for(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        for( j = 0; j <= n - 4; j += 4 )
        {
            const T* s0 = (const T*)(src + i*sizeof(T) + sstep*j);
            const T* s1 = (const T*)(src + i*sizeof(T) + sstep*(j+1));
            const T* s2 = (const T*)(src + i*sizeof(T) + sstep*(j+2));
            const T* s3 = (const T*)(src + i*sizeof(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        for( ; j < n; j++ )
        {
            const T* s0 = (const T*)(src + i*sizeof(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #endif
    for( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #if CV_ENABLE_UNROLLED
        for(; j <= n - 4; j += 4 )
        {
            const T* s0 = (const T*)(src + i*sizeof(T) + sstep*j);
            const T* s1 = (const T*)(src + i*sizeof(T) + sstep*(j+1));
            const T* s2 = (const T*)(src + i*sizeof(T) + sstep*(j+2));
            const T* s3 = (const T*)(src + i*sizeof(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #endif
        for( ; j < n; j++ )
        {
            const T* s0 = (const T*)(src + i*sizeof(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

template<typename T> static void
transposeI_( uchar* data, size_t step, int n )
{
    int i, j;
    for( i = 0; i < n; i++ )
    {
        T* row = (T*)(data + step*i);
        uchar* data1 = data + i*sizeof(T);
        for( j = i+1; j < n; j++ )
            std::swap( row[j], *(T*)(data1 + step*j) );
    }
}

typedef void (*TransposeFunc)( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz );
typedef void (*TransposeInplaceFunc)( uchar* data, size_t step, int n );

#define DEF_TRANSPOSE_FUNC(suffix, type) \
static void transpose_##suffix( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
static void transposeI_##suffix( uchar* data, size_t step, int n ) \
{ transposeI_<type>(data, step, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, int)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

static TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

static TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

} // namespace cv

void cv::transpose( InputArray _src, OutputArray _dst )
{
    Mat src = _src.getMat();
    size_t esz = src.elemSize();
    AVR_ASSERT( src.dims <= 2 && esz <= (size_t)32 );

    _dst.create(src.cols, src.rows, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    if( src.rows != dst.cols || src.cols != dst.rows )
    {
        AVR_ASSERT( src.size() == dst.size() && (src.cols == 1 || src.rows == 1) );
        src.copyTo(dst);
        return;
    }

    if( dst.data == src.data )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        AVR_ASSERT( func != 0 );
        func( dst.data, dst.step, dst.rows );
    }
    else
    {
        TransposeFunc func = transposeTab[esz];
        AVR_ASSERT( func != 0 );
        func( src.data, src.step, dst.data, dst.step, src.size() );
    }
}

////////////////////////////////////// completeSymm /////////////////////////////////////////

void cv::completeSymm( InputOutputArray _m, bool LtoR )
{
    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    AVR_ASSERT( m.dims <= 2 && m.rows == m.cols );

    int rows = m.rows;
    int j0 = 0, j1 = rows;

    uchar* data = m.data;
    for( int i = 0; i < rows; i++ )
    {
        if( !LtoR ) j1 = i; else j0 = i+1;
        for( int j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}

namespace cv {
/****************************************************************************************\
*                                      Cross Product                                     *
\****************************************************************************************/

cv::Mat cv::Mat::cross(InputArray _m) const
{
    Mat m = _m.getMat();
    int tp = type(), d = CV_MAT_DEPTH(tp);
    AVR_ASSERT( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    if( d == CV_32F )
    {
        const float *a = (const float*)data, *b = (const float*)m.data;
        float* c = (float*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    else if( d == CV_64F )
    {
        const double *a = (const double*)data, *b = (const double*)m.data;
        double* c = (double*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    return result;
}

// begin matmul.cpp
/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> double
dotProd_(const T* src1, const T* src2, int len)
{
    int i = 0;
    double result = 0;
     #if CV_ENABLE_UNROLLED
    for( ; i <= len - 4; i += 4 )
        result += (double)src1[i]*src2[i] + (double)src1[i+1]*src2[i+1] +
            (double)src1[i+2]*src2[i+2] + (double)src1[i+3]*src2[i+3];
    #endif
    for( ; i < len; i++ )
        result += (double)src1[i]*src2[i];

    return result;
}


static double dotProd_8u(const uchar* src1, const uchar* src2, int len)
{
    double r = 0;
#if ARITHM_USE_IPP
    ippiDotProd_8u64f_C1R(src1, (int)(len*sizeof(src1[0])),
                          src2, (int)(len*sizeof(src2[0])),
                          ippiSize(len, 1), &r);
    return r;
#else
    int i = 0;

#if CV_SSE2
    if( USE_SSE2 )
    {
        int j, len0 = len & -4, blockSize0 = (1 << 13), blockSize;
        __m128i z = _mm_setzero_si128();
        while( i < len0 )
        {
            blockSize = std::min(len0 - i, blockSize0);
            __m128i s = _mm_setzero_si128();
            j = 0;
            for( ; j <= blockSize - 16; j += 16 )
            {
                __m128i b0 = _mm_loadu_si128((const __m128i*)(src1 + j));
                __m128i b1 = _mm_loadu_si128((const __m128i*)(src2 + j));
                __m128i s0, s1, s2, s3;
                s0 = _mm_unpacklo_epi8(b0, z);
                s2 = _mm_unpackhi_epi8(b0, z);
                s1 = _mm_unpacklo_epi8(b1, z);
                s3 = _mm_unpackhi_epi8(b1, z);
                s0 = _mm_madd_epi16(s0, s1);
                s2 = _mm_madd_epi16(s2, s3);
                s = _mm_add_epi32(s, s0);
                s = _mm_add_epi32(s, s2);
            }

            for( ; j < blockSize; j += 4 )
            {
                __m128i s0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int*)(src1 + j)), z);
                __m128i s1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int*)(src2 + j)), z);
                s0 = _mm_madd_epi16(s0, s1);
                s = _mm_add_epi32(s, s0);
            }
            CV_DECL_ALIGNED(16) int buf[4];
            _mm_store_si128((__m128i*)buf, s);
            r += buf[0] + buf[1] + buf[2] + buf[3];

            src1 += blockSize;
            src2 += blockSize;
            i += blockSize;
        }
    }
#endif
    return r + dotProd_(src1, src2, len - i);
#endif
}


static double dotProd_8s(const schar* src1, const schar* src2, int len)
{
    return dotProd_(src1, src2, len);
}

static double dotProd_16u(const ushort* src1, const ushort* src2, int len)
{
    double r = 0;
    IF_IPP(ippiDotProd_16u64f_C1R(src1, (int)(len*sizeof(src1[0])),
                                  src2, (int)(len*sizeof(src2[0])),
                                  ippiSize(len, 1), &r),
           r = dotProd_(src1, src2, len));
    return r;
}

static double dotProd_16s(const short* src1, const short* src2, int len)
{
    double r = 0;
    IF_IPP(ippiDotProd_16s64f_C1R(src1, (int)(len*sizeof(src1[0])),
                                  src2, (int)(len*sizeof(src2[0])),
                                  ippiSize(len, 1), &r),
           r = dotProd_(src1, src2, len));
    return r;
}

static double dotProd_32s(const int* src1, const int* src2, int len)
{
    double r = 0;
    IF_IPP(ippiDotProd_32s64f_C1R(src1, (int)(len*sizeof(src1[0])),
                                  src2, (int)(len*sizeof(src2[0])),
                                  ippiSize(len, 1), &r),
           r = dotProd_(src1, src2, len));
    return r;
}

static double dotProd_32f(const float* src1, const float* src2, int len)
{
    double r = 0;
    IF_IPP(ippsDotProd_32f64f(src1, src2, len, &r),
           r = dotProd_(src1, src2, len));
    return r;
}

static double dotProd_64f(const double* src1, const double* src2, int len)
{
    double r = 0;
    IF_IPP(ippsDotProd_64f(src1, src2, len, &r),
           r = dotProd_(src1, src2, len));
    return r;
}


typedef double (*DotProdFunc)(const uchar* src1, const uchar* src2, int len);

static DotProdFunc getDotProdFunc(int depth)
{
    static DotProdFunc dotProdTab[] =
    {
        (DotProdFunc)GET_OPTIMIZED(dotProd_8u), (DotProdFunc)GET_OPTIMIZED(dotProd_8s),
        (DotProdFunc)dotProd_16u, (DotProdFunc)dotProd_16s,
        (DotProdFunc)dotProd_32s, (DotProdFunc)GET_OPTIMIZED(dotProd_32f),
        (DotProdFunc)dotProd_64f, 0
    };

    return dotProdTab[depth];
}

double Mat::dot(InputArray _mat) const
{
    Mat mat = _mat.getMat();
    int cn = channels();
    DotProdFunc func = getDotProdFunc(depth());
    AVR_ASSERT( mat.type() == type() && mat.size == size && func != 0 );

    if( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        if( len == (size_t)(int)len )
            return func(data, mat.data, (int)len);
    }

    const Mat* arrays[] = {this, &mat, 0};
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs);
    int len = (int)(it.size*cn);
    double r = 0;

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    return r;
}
// end matmul.cpp

// begin operations.hpp
template<typename _Tp> class LessThan
{
public:
    bool operator()(const _Tp& a, const _Tp& b) const { return a < b; }
};

template<typename _Tp> class GreaterEq
{
public:
    bool operator()(const _Tp& a, const _Tp& b) const { return a >= b; }
};

template<typename _Tp> class LessThanIdx
{
public:
    LessThanIdx( const _Tp* _arr ) : arr(_arr) {}
    bool operator()(int a, int b) const { return arr[a] < arr[b]; }
    const _Tp* arr;
};

template<typename _Tp> class GreaterEqIdx
{
public:
    GreaterEqIdx( const _Tp* _arr ) : arr(_arr) {}
    bool operator()(int a, int b) const { return arr[a] >= arr[b]; }
    const _Tp* arr;
};
// end operations.hpp

} // namespace cv

//////////////////////////////////////// sort ///////////////////////////////////////////

namespace cv {

template<typename T> static void sort_( const Mat& src, Mat& dst, int flags )
{
    AutoBuffer<T> buf;
    T* bptr;
    int i, j, n, len;
    bool sortRows = (flags & 1) == CV_SORT_EVERY_ROW;
    bool inplace = src.data == dst.data;
    bool sortDescending = (flags & CV_SORT_DESCENDING) != 0;

    if( sortRows )
        n = src.rows, len = src.cols;
    else
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
    }
    bptr = (T*)buf;

    for( i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        if( sortRows )
        {
            T* dptr = (T*)(dst.data + dst.step*i);
            if( !inplace )
            {
                const T* sptr = (const T*)(src.data + src.step*i);
                for( j = 0; j < len; j++ )
                    dptr[j] = sptr[j];
            }
            ptr = dptr;
        }
        else
        {
            for( j = 0; j < len; j++ )
                ptr[j] = ((const T*)(src.data + src.step*j))[i];
        }
        std::sort( ptr, ptr + len, LessThan<T>() );
        if( sortDescending )
            for( j = 0; j < len/2; j++ )
                std::swap(ptr[j], ptr[len-1-j]);
        if( !sortRows )
            for( j = 0; j < len; j++ )
                ((T*)(dst.data + dst.step*j))[i] = ptr[j];
    }
}


template<typename T> static void sortIdx_( const Mat& src, Mat& dst, int flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<int> ibuf;
    T* bptr;
    int* _iptr;
    int i, j, n, len;
    bool sortRows = (flags & 1) == CV_SORT_EVERY_ROW;
    bool sortDescending = (flags & CV_SORT_DESCENDING) != 0;

    AVR_ASSERT( src.data != dst.data );

    if( sortRows )
        n = src.rows, len = src.cols;
    else
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    bptr = (T*)buf;
    _iptr = (int*)ibuf;

    for( i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        int* iptr = _iptr;

        if( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = (int*)(dst.data + dst.step*i);
        }
        else
        {
            for( j = 0; j < len; j++ )
                ptr[j] = ((const T*)(src.data + src.step*j))[i];
        }
        for( j = 0; j < len; j++ )
            iptr[j] = j;
        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        if( sortDescending )
            for( j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        if( !sortRows )
            for( j = 0; j < len; j++ )
                ((int*)(dst.data + dst.step*j))[i] = iptr[j];
    }
}

typedef void (*SortFunc)(const Mat& src, Mat& dst, int flags);

} // namespace cv

void cv::sort( InputArray _src, OutputArray _dst, int flags )
{
    static SortFunc tab[] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<int>, sort_<float>, sort_<double>, 0
    };
    Mat src = _src.getMat();
    SortFunc func = tab[src.depth()];
    AVR_ASSERT( src.dims <= 2 && src.channels() == 1 && func != 0 );
    _dst.create( src.size(), src.type() );
    Mat dst = _dst.getMat();
    func( src, dst, flags );
}

void cv::sortIdx( InputArray _src, OutputArray _dst, int flags )
{
    static SortFunc tab[] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<int>, sortIdx_<float>, sortIdx_<double>, 0
    };
    Mat src = _src.getMat();
    SortFunc func = tab[src.depth()];
    AVR_ASSERT( src.dims <= 2 && src.channels() == 1 && func != 0 );

    Mat dst = _dst.getMat();
    if( dst.data == src.data )
        _dst.release();
    _dst.create( src.size(), CV_32S );
    dst = _dst.getMat();
    func( src, dst, flags );
}

///////////////////////////// n-dimensional matrices ////////////////////////////

namespace cv {

Mat Mat::reshape(int _cn, int _newndims, const int* _newsz) const
{
    if(_newndims == dims)
    {
        if(_newsz == 0)
            return reshape(_cn);
        if(_newndims == 2)
            return reshape(_cn, _newsz[0]);
    }

    AVR_ERROR(Cod::NotImplemented, "");
    // TBD
    return Mat();
}

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, int _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, int _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, int _narrays)
{
    AVR_ASSERT( _arrays && (_ptrs || _planes) );
    int i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    if( narrays < 0 )
    {
        for( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        AVR_ASSERT(narrays <= 1000);
    }

    iterdepth = 0;

    for( i = 0; i < narrays; i++ )
    {
        AVR_ASSERT(arrays[i] != 0);
        const Mat& A = *arrays[i];
        if( ptrs )
            ptrs[i] = A.data;

        if( !A.data )
            continue;

        if( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            for( d1 = 0; d1 < d; d1++ )
                if( A.size[d1] > 1 )
                    break;
        }
        else
            AVR_ASSERT( A.size == arrays[i0]->size );

        if( !A.isContinuous() )
        {
            AVR_ASSERT( A.step[d-1] == A.elemSize() );
            for( j = d-1; j > d1; j-- )
                if( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    if( i0 >= 0 )
    {
        size = arrays[i0]->size[d-1];
        for( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            if( total1 != (int)total1 )
                break;
            size = (int)total1;
        }

        iterdepth = j;
        if( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        for( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    else
        iterdepth = 0;

    idx = 0;

    if( !planes )
        return;

    for( i = 0; i < narrays; i++ )
    {
        AVR_ASSERT(arrays[i] != 0);
        const Mat& A = *arrays[i];

        if( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (int)size, A.type(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    if( idx >= nplanes-1 )
        return *this;
    ++idx;

    if( iterdepth == 1 )
    {
        if( ptrs )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        if( planes )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    else
    {
        for( int i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            if( !A.data )
                continue;
            int _idx = (int)idx;
            uchar* data = A.data;
            for( int j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                int szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            if( ptrs )
                ptrs[i] = data;
            if( planes )
                planes[i].data = data;
        }
    }

    return *this;
}

NAryMatIterator NAryMatIterator::operator ++(int)
{
    NAryMatIterator it = *this;
    ++*this;
    return it;
}
// end matrix.cpp

} // namespace cv

/* End of file. */
