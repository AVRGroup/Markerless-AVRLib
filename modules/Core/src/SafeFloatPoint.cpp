#include <iostream>
#include <limits>
#include <cmath>

#include <avr/core/SafeFloatPoint.hpp>

using std::cout;
using std::endl;

namespace avr {

const float Float::eps = std::numeric_limits<float>::epsilon();
const double Double::eps = std::numeric_limits<double>::epsilon();

static int fastPow(int base, unsigned exp) {
   if(exp == 0) return 1;
   if(exp == 1) return base;
   if(exp == 2) return base * base;
   int _pow = base;
   while(--exp > 1)
      _pow *= base;
   return _pow;
}

Float Float::Round(unsigned prec) const {
   int   mult = fastPow(10, prec);
   float temp = mult * this->value;
   int   integer = int(temp);
   Float decimal = temp - integer;

   if(decimal < 0.5f)
      return Float(integer / (float)mult);
   if(decimal > 0.5f || integer % 2 != 0)
      return Float(++integer / (float)mult);
   return Float(integer / (float)mult);
}

Float Float::Trunc(unsigned prec) const {
   int mult = fastPow(10, prec);
   int integer = int(mult * this->value);
   return Float(integer / (float)mult);
}

Double Double::Round(unsigned prec) const {
   int    mult = fastPow(10, prec);
   double temp = mult * this->value;
   int    integer = int(temp);
   Double decimal = temp - integer;

   if(decimal < 0.5d)
      return Double(integer / (double)mult);
   if(decimal > 0.5d || integer % 2 != 0)
      return Double(++integer / (double)mult);
   return Double(integer / (double)mult);
}

Double Double::Trunc(unsigned prec) const {
   int mult = fastPow(10, prec);
   int integer = int(mult * this->value);
   return Double(integer / (double)mult);
}

} // namespace avr
