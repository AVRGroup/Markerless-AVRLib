#ifndef AVR_SAFE_FLOAT_POINT_HPP
#define AVR_SAFE_FLOAT_POINT_HPP

#ifdef __cplusplus

namespace avr {
// classes in this file
class Float;
class Double;

//////////////////////////////////////////////////// Safe Float /////////////////////////////////////////////////////////
/**
 * @class Float Core.hpp <avr/core/Core.hpp>
 * @brief The safe singular precision float point class implements an "specialization" of @em float primitive type
 *
 * Comparison operations are specialized to allow minimum precision errors defined by machine. The implicity conversion
 * to float primitive type allows instances of this class may be used like the C++ primitive type.
 */
class Float {
public:
   static const float eps;
   float value;
   // conversion constructors
   Float(const float& v);
   Float(const Double& d);
   Float(/* default */);
   //! Converts to float primitive type implicitly
   operator float() const;
   //! Converts to boolean for use in if's, && and || operator, etc...
   operator bool() const;
   //! Converts to int (
   operator int() const;
};

//////////////////////////////////////////////////// Safe Double ////////////////////////////////////////////////////////
/**
 * @class Double Core.hpp <avr/core/Core.hpp>
 * @brief The safe double precision float point class implements an "specialization" of @em double primitive type
 *
 * Comparison operations are specialized to allow minimum precision errors defined by machine. The implicity conversion
 * to double primitive type allows instances of this class may be used like the C++ primitive type.
 */
class Double {
public:
   static const double eps;
   double value;
   // conversion constructors
   Double(const double& v);
   Double(const Float& f);
   Double(/* default */);
   //! Converts to double primitive type implicitly
   operator double() const;
   //! Converts to boolean for use in if's, && and || operator, etc...
   operator bool() const;
   //! Converts to int, rounding the value
   operator int() const;
};

} // namespace avr

#endif // __cplusplus

#include "impl/SafeFloatPoint.tcc"

#endif // AVR_SAFE_FLOAT_POINT_HPP
