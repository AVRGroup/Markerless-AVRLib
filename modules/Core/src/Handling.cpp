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

#include <sstream>
#include <cstdio>
#include <cstdarg>

#include <avr/core/Handling.hpp>

#if defined(_WIN32) || defined(WIN32) || defined(WINCE)
    #include <windows.h>
    #define STDERR          GetStdHandle(STD_ERROR_HANDLE)
    #define RED_TEXT()      SetConsoleTextAttribute(STDERR, FOREGROUND_RED | FOREGROUND_INTENSITY)
    #define BLUE_TEXT()     SetConsoleTextAttribute(STDERR, FOREGROUND_BLUE | FOREGROUND_GREEN)
    #define YELLOW_TEXT()   SetConsoleTextAttribute(STDERR, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
    #define DEFAULT_TEXT()  SetConsoleTextAttribute(STDERR, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#elif defined(__linux)
    #define RED_TEXT()      fprintf(stderr, "\033[31m");
    #define BLUE_TEXT()     fprintf(stderr, "\033[36m");
    #define YELLOW_TEXT()   fprintf(stderr, "\033[33m");
    #define DEFAULT_TEXT()  fprintf(stderr, "\033[0m");
#else
    #define RED_TEXT()
    #define BLUE_TEXT()
    #define YELLOW_TEXT()
    #define DEFAULT_TEXT()
#endif

using std::string;
using std::stringstream;

//! convert code to description string
static string str (ERROR_COD c);
static string format(const char* fmt, ...);

avr::Exception::Exception(ERROR_COD _id, const std::string& _err, const std::string& _func, const std::string& _file, int _line) :
    id(_id), msg(""), err(_err), func(_func), file(_file), line(_line)
    {this->formatMessage();}

avr::Exception::~Exception() throw() {}

const char* avr::Exception::what() const throw() {
    return msg.c_str();
}

void avr::Exception::formatMessage() {
    if(func.size() > 0)
        this->msg = format("error (%s) in function %s, file %s, line %d", err.c_str(), func.c_str(), file.c_str(), line);
    else
        this->msg = format("error (%s), file %s, line %d", err.c_str(), func.c_str(), file.c_str(), line);
}

void avr::error(const avr::Exception& exc){
    YELLOW_TEXT();
    string buf = format("AVRLIB Error (%d): %s (%s) in file %s:%d", exc.id, str(exc.id).c_str(), exc.err.c_str(), exc.file.c_str(), exc.line);
    fprintf(stderr, "%s\n", buf.c_str());
    DEFAULT_TEXT();
    throw exc;
}

inline string str (ERROR_COD c) {
    switch(c){
        case Cod::AssertionFailed:      return "Assertion failed";
        case Cod::BadAllocation:        return "Bad Allocation";
        case Cod::BadFlag:              return "Invalid flag";
        case Cod::DividedByZero:        return "Division by zero";
        case Cod::FunctionArgument:     return "Invalid function's argument";
        case Cod::TemplateArgument:     return "Invalid template's argument";
        case Cod::UnsupportedFormat:    return "Unsupported format of matrix";
        case Cod::NotImplemented:       return "Not implemented";
        case Cod::NullPointer:          return "Null pointer";
        case Cod::OutOfRange:           return "Out of range";
        default: /* Unknown */          return "Unspecified error";
    }
}

string format(const char* fmt, ...) {
    std::string buf;
    buf.resize(1024);

    for ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        size_t len = vsnprintf((char *)buf.c_str(), buf.size(), fmt, va);
        va_end(va);

        if (len < 0 || len >= buf.size())
        {
            buf.resize(std::max(buf.size() << 1, len + 1));
            continue;
        }
        return buf;
    }
}
