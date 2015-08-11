/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
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

#if !defined(AVR_CORE_HANDLING_HPP)
#define AVR_CORE_HANDLING_HPP

#ifdef __cplusplus

#include <string>
#include <iostream>
#include <exception>

#if defined __GNUC__
    #define AVR_FUNC __func__
#elif defined _MSC_VER
    #define AVR_FUNC __FUNCTION__
#else
    #define AVR_FUNC ""
#endif

/** @def AVR_ERROR macro function: it prints a message error and finished running
 *  Usage: AVR_ERROR(<error code>, "your message")
 *  Ex: AVR_ERROR(Cod::BadAllocation, "memory error");
 */
#define AVR_ERROR( code, msg ) avr::error( avr::Exception(code, msg, AVR_FUNC, __FILE__, __LINE__) )

/** @def AVR_FMT_ERROR macro function: the same of AVR_ERROR but with a formatted message error
 *  Usage: AVR_FMT_ERROR(<error code>, "format mask", <args>)
 *  Ex: AVR_FMT_ERROR(Cod::Undefined, "%d is not within the range", value);
 */
#define AVR_FMT_ERROR( code, fmt, ... ) avr::error( avr::Exception(code, avr::format(fmt, ##__VA_ARGS__), AVR_FUNC, __FILE__, __LINE__) )

/** @def AVR_ASSERT macro function: it compares an expression, if it is false then an assertion error is throws
 *  Usage: AVR_ASSERT(<your expression>)
 *  Ex: AVR_ASSERT(denominator != 0);
 */
#define AVR_ASSERT( expr ) if(expr); else avr::error( avr::Exception(Cod::AssertionFailed, #expr, AVR_FUNC, __FILE__, __LINE__) )

//! @def AVR_DBG_ASSERT macro function: the same of AVR_ASSERT but the expression is checked only in debug mode
#ifdef _DEBUG
    #define AVR_DBG_ASSERT(expr) AVR_ASSERT(expr)
#else
    #define AVR_DBG_ASSERT(expr)
#endif

/** @enum ERROR_COD defines error code flags
 *  independent of language version uses Cod::SomeFlag
 */
#if __cplusplus > 199711L // C++11
    enum class ERROR_COD : uint8_t {
#else
struct Cod {
    enum ERROR_COD {
#endif // __cplusplus
        AssertionFailed = 101,  //!< not passed in assert condition
        FunctionArgument,       //!< any function's argument is invalid
        TemplateArgument,       //!< any template's argument is invalid
        MatrixFormat,           //!< matrix type is unsupported
        OutOfRange,             //!< exceeded array size
        NullPointer,            //!< attempt to access a null pointer
        NotImplemented,         //!< functionality not implemented yet
        BadAllocation,          //!< memory error
        BadCasting,             //!< dynamic cast error
        BadFlag,                //!< flag is invalid
        Undefined               //!< other
    };
#if __cplusplus > 199711L // C++11
    typedef ERROR_COD Cod;
#else
    };
#endif // __cplusplus

namespace avr {
/*!
 The standard AVR exception class, based on cv::Exception.
 Instances of the class are thrown by various functions and methods in the case of critical errors.
 */
class Exception : public std::exception
{
public:
    /*!
     Full constructor. Normally the constuctor is not called explicitly.
     Instead, the macros AVR_ERROR() and AVR_ASSERT() are used.
    */
    Exception(ERROR_COD _id, const std::string& _err, const std::string& _func, const std::string& _file, int _line);
    virtual ~Exception() throw();

    /*!
     \return the error description and the context as a text string.
    */
    virtual const char *what() const throw();
    void formatMessage();

    ERROR_COD   id;
    std::string msg; ///< the formatted error message
    std::string err; ///< error description
    std::string func; ///< function name. Available only when the compiler supports getting it
    std::string file; ///< source file name where the error has occured
    int line; ///< line number in the source file where the error has occured
};

/*!
  Prints information about the error to stderr and then raises the exception
  \param exc the exception raised.
 */
void error( const Exception& exc );
/*!
  Formats a string
  \param fmt the format mask in c-style
 */
std::string format(const char* fmt, ...);

} // namespace avr

#endif // __cplusplus

#endif
