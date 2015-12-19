#ifndef AVR_SAFE_FLOAT_POINT_HPP
#define AVR_SAFE_FLOAT_POINT_HPP

#ifdef __cplusplus

namespace avr {
// classes in this file
class Float;
class Double;
class FPOperand;

//////////////////////////////////////////////////// Safe Float /////////////////////////////////////////////////////////
/**
 * @class Float Core.hpp <avr/core/Core.hpp>
 * @brief The safe simple precision float point class implements an "specialization" of @em float primitive type
 *
 * Comparison operations are specialized to allow minimum precision errors defined by machine. All operators applicable
 * to float point numbers are overloaded. Unary and Assign operators are defined in class, the others are global
 * overloads to allow any order between operands.
 */
class Float {
public:
   Float(/* default */);
   Float(const float& v);
   Float(const FPOperand&);
   //! Explicit conversion from Double
   explicit Float(const Double& d);

   //! @return The float point value in simple precision
   float Get() const;
   /**
    * Rounds the value gives the desired precision. When decimals are 0.5 the parity of the integer part is considered
    * Ex: prec = 0: 1.49999 -> 1; 1.50001 -> 2; 1.5 -> 2; 0.5 -> 0
    *     prec = 2: 1.44499 -> 1.44000; 1.44501 -> 1.45000
    * @param prec Number of decimals places to maintain, the default is 0
    * @return The rounded number
    */
   Float Round(unsigned prec = 0) const;
   /**
    * Truncs the value gives the desired precision.
    * @param prec Number of decimals places to maintain, the default is 0
    * @return The truncated number
    */
   Float Trunc(unsigned prec = 0) const;

   // Unary operators overload
   //! Checks if value is zero @return true if it's zero or false otherwise
   bool  operator ! () const;
   //! Module operator @return |value|
   Float operator + () const;
   //! Inverts the signal @return -value
   Float operator - () const;
   //! Pre-fixed increment (++value) @return this
   Float& operator ++ (int);
   //! Pre-fixed decrement (--value) @return this
   Float& operator -- (int);
   //! Pos-fixed increment (value++) @return this
   Float& operator ++ (void);
   //! Pos-fixed decrement (value--) @return this
   Float& operator -- (void);

   // Assign operators overload
   Float& operator =  (const Float&);
   Float& operator =  (const FPOperand&);
   Float& operator += (const FPOperand&);
   Float& operator -= (const FPOperand&);
   Float& operator *= (const FPOperand&);
   Float& operator /= (const FPOperand&);

   //! Explicit conversion to float primitive type
   explicit operator float() const { return this->value; }
   //! Explicit conversion to 32-bit integer primitive type
   explicit operator int() const { return int(this->value); }

public:
   float value;
   static const float eps;
};

//////////////////////////////////////////////////// Safe Double ////////////////////////////////////////////////////////
/**
 * @class Double Core.hpp <avr/core/Core.hpp>
 * @brief The safe double precision float point class implements an "specialization" of @em double primitive type
 *
 * Comparison operations are specialized to allow minimum precision errors defined by machine. All operators applicable
 * to float point numbers are overloaded. Unary and Assign operators are defined in class, the others are global
 * overloads to allow any order between operands.
 */
class Double {
public:
   Double(/* default */);
   Double(const double& v);
   Double(const FPOperand&);
   //! Explicit conversion from Float
   explicit Double(const Float& d);

   //! @return The float point value in double precision
   double Get() const;
   /**
    * Rounds the value gives the desired precision. When decimals are 0.5 the parity of the integer part is considered
    * Ex: prec = 0: 1.49999 -> 1; 1.50001 -> 2; 1.5 -> 2; 0.5 -> 0
    *     prec = 2: 1.44499 -> 1.44000; 1.44501 -> 1.45000
    * @param prec Number of decimals places to maintain, the default is 0
    * @return The rounded number
    */
   Double Round(unsigned prec = 0) const;
   /**
    * Truncs the value gives the desired precision.
    * @param prec Number of decimals places to maintain, the default is 0
    * @return The truncated number
    */
   Double Trunc(unsigned prec = 0) const;

   // Unary operators overload
   //! Checks if value is zero @return true if it's zero or false otherwise
   bool  operator ! () const;
   //! Module operator @return |value|
   Double operator + () const;
   //! Inverts the signal @return -value
   Double operator - () const;
   //! Pre-fixed increment (++value) @return this
   Double& operator ++ (int);
   //! Pre-fixed decrement (--value) @return this
   Double& operator -- (int);
   //! Pos-fixed increment (value++) @return this
   Double& operator ++ (void);
   //! Pos-fixed decrement (value--) @return this
   Double& operator -- (void);

   // Assign operators overload
   Double& operator =  (const Double&);
   Double& operator =  (const FPOperand&);
   Double& operator += (const FPOperand&);
   Double& operator -= (const FPOperand&);
   Double& operator *= (const FPOperand&);
   Double& operator /= (const FPOperand&);

   //! Explicit conversion to double primitive type
   explicit operator double() const { return this->value; }
   //! Explicit conversion to 32-bit integer primitive type
   explicit operator int() const { return int(this->value); }

public:
   double value;
   static const double eps;
};

//////////////////////////////////////////////////// Float Point Operand /////////////////////////////////////////////////////////
/**
 * @class FPOperand SafeFloatPoint.hpp <avr/core/SafeFloatPoint.hpp>
 * @brief The Float point operand class represents a generic operand for operations between safe float point objects
 *
 * This class is used to generalize the operands of float point operations. Any number, integer or real, represented
 * by Float or Double classes or primitive types, is implicitly converted to FPOperand, thus it is possible to make
 * expressions concatenating these types.
 */
class FPOperand {
public:
   FPOperand(const Float&);
   FPOperand(const Double&);
   template <typename Tp> FPOperand(const Tp&);

   // Conversion Ops
   operator Float() const;
   operator Double() const;

   // Arithmetic Ops
   friend FPOperand operator + (const FPOperand&, const FPOperand&);
   friend FPOperand operator - (const FPOperand&, const FPOperand&);
   friend FPOperand operator * (const FPOperand&, const FPOperand&);
   friend FPOperand operator / (const FPOperand&, const FPOperand&);
   // Logical Ops
   friend bool operator == (const FPOperand&, const FPOperand&);
   friend bool operator != (const FPOperand&, const FPOperand&);
   friend bool operator <= (const FPOperand&, const FPOperand&);
   friend bool operator >= (const FPOperand&, const FPOperand&);
   friend bool operator <  (const FPOperand&, const FPOperand&);
   friend bool operator >  (const FPOperand&, const FPOperand&);
   friend bool operator && (const FPOperand&, const FPOperand&);
   friend bool operator || (const FPOperand&, const FPOperand&);

   friend class Float;
   friend class Double;

private:
   FPOperand(const double&, const double&);

   double val;
   double eps;
};

} // namespace avr

#endif // __cplusplus

#include "impl/SafeFloatPoint.tcc"

#endif // AVR_SAFE_FLOAT_POINT_HPP
