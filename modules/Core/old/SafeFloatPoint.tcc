#ifndef AVR_SAFE_FLOAT_POINT_TCC
#define AVR_SAFE_FLOAT_POINT_TCC

#ifdef __cplusplus

#ifndef _GLIBCXX_NUMERIC_LIMITS
   #include <limits>
#endif // _GLIBCXX_NUMERIC_LIMITS

namespace avr {
/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                         Safe float point classes                                                                *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
////////////////////////////////////////////////////////////////// Float //////////////////////////////////////////////////////////////////////////
inline Float::Float() : value(0.0f) { }
inline Float::Float(const float& v) : value(v) { }
inline Float::Float(const Double& d) : value(float(d.value)) { }
const float Float::eps = std::numeric_limits<float>::epsilon();
// == Ops
static inline bool operator == (const Float& f1, const Float& f2)  /* --- main --> */  { return (f2.value - Float::eps <= f1.value && f1.value <= f2.value + Float::eps); }
static inline bool operator == (const Float& f1, const Double& f2) /* ------------ */  { return (f1 == Float(f2.value)); }
template <typename Tp> static inline bool operator == (const Tp& f1, const Float& f2)  { return (Float(f1) == f2); }
template <typename Tp> static inline bool operator == (const Float& f1, const Tp& f2)  { return (f1 == Float(f2)); }
// != Ops
static inline bool operator != (const Float& f1, const Float& f2)  /* --- main --> */  { return (f1.value < f2.value - Float::eps || Float::eps + f2.value < f1.value); }
static inline bool operator != (const Float& f1, const Double& f2) /* ------------ */  { return (f1 != Float(f2.value)); }
template <typename Tp> static inline bool operator != (const Tp& f1, const Float& f2)  { return (Float(f1) != f2); }
template <typename Tp> static inline bool operator != (const Float& f1, const Tp& f2)  { return (f1 != Float(f2)); }
// <= Ops
static inline bool operator <= (const Float& f1, const Float& f2)  /* --- main --> */  { return (f1.value <= f2.value + Float::eps); }
static inline bool operator <= (const Float& f1, const Double& f2) /* ------------ */  { return (f1 <= Float(f2.value)); }
template <typename Tp> static inline bool operator <= (const Tp& f1, const Float& f2)  { return (Float(f1) <= f2); }
template <typename Tp> static inline bool operator <= (const Float& f1, const Tp& f2)  { return (f1 <= Float(f2)); }
// >= Ops
static inline bool operator >= (const Float& f1, const Float& f2)  /* --- main --> */  { return (f1.value >= f2.value - Float::eps); }
static inline bool operator >= (const Float& f1, const Double& f2) /* ------------ */  { return (f1 >= Float(f2.value)); }
template <typename Tp> static inline bool operator >= (const Tp& f1, const Float& f2)  { return (Float(f1) >= f2); }
template <typename Tp> static inline bool operator >= (const Float& f1, const Tp& f2)  { return (f1 >= Float(f2)); }
// < Ops
static inline bool operator < (const Float& f1, const Float& f2)  /* --- main --> */   { return (f1.value < f2.value - Float::eps); }
static inline bool operator < (const Float& f1, const Double& f2) /* ------------ */   { return (f1 < Float(f2.value)); }
template <typename Tp> static inline bool operator < (const Tp& f1, const Float& f2)   { return (Float(f1) < f2); }
template <typename Tp> static inline bool operator < (const Float& f1, const Tp& f2)   { return (f1 < Float(f2)); }
// > Ops
static inline bool operator > (const Float& f1, const Float& f2)  /* --- main --> */   { return (f1.value > f2.value + Float::eps); }
static inline bool operator > (const Float& f1, const Double& f2) /* ------------ */   { return (f1 > Float(f2.value)); }
template <typename Tp> static inline bool operator > (const Tp& f1, const Float& f2)   { return (Float(f1) > f2); }
template <typename Tp> static inline bool operator > (const Float& f1, const Tp& f2)   { return (f1 > Float(f2)); }
// Unary Op
static inline bool  operator ! (const Float& f1) { return (f1 == 0.0f); }
static inline Float operator - (const Float& f1) { return (Float(-f1.value)); }
static inline Float operator + (const Float& f1) { return (Float( (f1 < 0.0f) ? Float(-f1.value) : f1 )); }
// + Ops
static inline Float  operator + (const Float& f1, const Float& f2)  /* ------------ */ { return Float(f1.value + f2.value); }
static inline Double operator + (const Float& f1, const Double& f2) /* ------------ */ { return Double(f1.value + f2.value); }
template <typename Tp> static inline Float  operator + (const Float& f1, const Tp& f2) { return Float(f1.value + float(f2)); }
template <typename Tp> static inline Float  operator + (const Tp& f1, const Float& f2) { return Float(float(f1) + f2.value); }
// - Ops
static inline Float  operator - (const Float& f1, const Float& f2)  /* ------------ */ { return Float(f1.value - f2.value); }
static inline Double operator - (const Float& f1, const Double& f2) /* ------------ */ { return Double(f1.value - f2.value); }
template <typename Tp> static inline Float  operator - (const Float& f1, const Tp& f2) { return Float(f1.value - float(f2)); }
template <typename Tp> static inline Float  operator - (const Tp& f1, const Float& f2) { return Float(float(f1) - f2.value); }
// * Ops
static inline Float  operator * (const Float& f1, const Float& f2)  /* ------------ */ { return Float(f1.value * f2.value); }
static inline Double operator * (const Float& f1, const Double& f2) /* ------------ */ { return Double(f1.value * f2.value); }
template <typename Tp> static inline Float  operator * (const Float& f1, const Tp& f2) { return Float(f1.value * float(f2)); }
template <typename Tp> static inline Float  operator * (const Tp& f1, const Float& f2) { return Float(float(f1) * f2.value); }
// / Ops
static inline Float  operator / (const Float& f1, const Float& f2)  /* ------------ */ { return Float(f1.value / f2.value); }
static inline Double operator / (const Float& f1, const Double& f2) /* ------------ */ { return Double(f1.value / f2.value); }
template <typename Tp> static inline Float  operator / (const Float& f1, const Tp& f2) { return Float(f1.value / float(f2)); }
template <typename Tp> static inline Float  operator / (const Tp& f1, const Float& f2) { return Float(float(f1) / f2.value); }
// += Ops
static inline Float& operator += (Float& f1, const Float& f2) /* ------------- */ { f1.value += f2.value; return f1; }
static inline Float& operator += (Float& f1, const Double& f2)/* ------------- */ { f1.value += f2.value; return f1; }
template <typename Tp> static inline Float& operator += (Float& f1, const Tp& f2) { f1.value += float(f2); return f1; }
template <typename Tp> static inline Tp& operator += (Tp& f1, const Float& f2)    { f1 += Tp(f2.value); return f1; }
// -= Ops
static inline Float& operator -= (Float& f1, const Float& f2) /* ------------- */ { f1.value -= f2.value; return f1; }
static inline Float& operator -= (Float& f1, const Double& f2)/* ------------- */ { f1.value -= f2.value; return f1; }
template <typename Tp> static inline Float& operator -= (Float& f1, const Tp& f2) { f1.value -= float(f2); return f1; }
template <typename Tp> static inline Tp& operator -= (Tp& f1, const Float& f2)    { f1 -= Tp(f2.value); return f1; }
// *= Ops
static inline Float& operator *= (Float& f1, const Float& f2) /* ------------- */ { f1.value *= f2.value; return f1; }
static inline Float& operator *= (Float& f1, const Double& f2)/* ------------- */ { f1.value *= f2.value; return f1; }
template <typename Tp> static inline Float& operator *= (Float& f1, const Tp& f2) { f1.value *= float(f2); return f1; }
template <typename Tp> static inline Tp& operator *= (Tp& f1, const Float& f2)    { f1 *= Tp(f2.value); return f1; }
// /= Ops
static inline Float& operator /= (Float& f1, const Float& f2) /* ------------- */ { f1.value /= f2.value; return f1; }
static inline Float& operator /= (Float& f1, const Double& f2)/* ------------- */ { f1.value /= f2.value; return f1; }
template <typename Tp> static inline Float& operator /= (Float& f1, const Tp& f2) { f1.value /= float(f2); return f1; }
template <typename Tp> static inline Tp& operator /= (Tp& f1, const Float& f2)    { f1 /= Tp(f2.value); return f1; }
// Conversion Ops
inline Float::operator float() const { return this->value; }
inline Float::operator bool() const { return ( (*this) != Float(0.0f) ); }
inline Float::operator int() const {
   int   integer = int(this->value);
   Float decimal = this->value - integer;
   if(decimal < Float(0.5f)) return integer;
   else if(decimal > Float(0.5f)) return ++integer;
   else if(integer % 2 == 0) return integer;
   else return ++integer;
}
////////////////////////////////////////////////////////////////// Double //////////////////////////////////////////////////////////////////////////
inline Double::Double() : value(0.0) { }
inline Double::Double(const double& v) : value(v) { }
inline Double::Double(const Float& f) : value(double(f.value)) { }
const double Double::eps   = std::numeric_limits<double>::epsilon();
// == Ops
static inline bool operator == (const Double& d1, const Double& d2) /* ----- main ---> */ { return (d2.value - Double::eps <= d1.value && d1.value <= d2.value + Double::eps); }
static inline bool operator == (const Double& d1, const Float& d2)  /* --------------- */ { return (Float(d1.value) == d2); }
template <typename Tp> static inline bool operator == (const Tp& d1, const Double& d2)    { return (Double(d1) == d2); }
template </* float */> /*  */ inline bool operator == (const float& d1, const Double& d2) { return (Float(d1) == Float(d2.value)); }
template <typename Tp> static inline bool operator == (const Double& d1, const Tp& d2)    { return (d1 == Double(d2)); }
template </* float */> /*  */ inline bool operator == (const Double& d1, const float& d2) { return (Float(d1.value) == Float(d2)); }
// != Ops
static inline bool operator != (const Double& d1, const Double& d2) /* ----- main ---> */ { return (d1.value < d2.value - Double::eps || Double::eps + d2.value < d1.value); }
static inline bool operator != (const Double& d1, const Float& d2)  /* --------------- */ { return (Float(d1.value) != d2); }
template <typename Tp> static inline bool operator != (const Tp& d1, const Double& d2)    { return (Double(d1) != d2); }
template </* float */> /*  */ inline bool operator != (const float& d1, const Double& d2) { return (Float(d1) != Float(d2.value)); }
template <typename Tp> static inline bool operator != (const Double& d1, const Tp& d2)    { return (d1 != Double(d2)); }
template </* float */> /*  */ inline bool operator != (const Double& d1, const float& d2) { return (Float(d1.value) != Float(d2)); }
// <= Ops
static inline bool operator <= (const Double& d1, const Double& d2) /* ----- main ---> */ { return (d1.value <= d2.value + Double::eps); }
static inline bool operator <= (const Double& d1, const Float& d2)  /* --------------- */ { return (Float(d1.value) <= d2); }
template <typename Tp> static inline bool operator <= (const Tp& d1, const Double& d2)    { return (Double(d1) <= d2); }
template </* float */> /*  */ inline bool operator <= (const float& d1, const Double& d2) { return (Float(d1) <= Float(d2.value)); }
template <typename Tp> static inline bool operator <= (const Double& d1, const Tp& d2)    { return (d1 <= Double(d2)); }
template </* float */> /*  */ inline bool operator <= (const Double& d1, const float& d2) { return (Float(d1.value) <= Float(d2)); }
// >= Ops
static inline bool operator >= (const Double& d1, const Double& d2) /* ----- main ---> */ { return (d1.value >= d2.value - Double::eps); }
static inline bool operator >= (const Double& d1, const Float& d2)  /* --------------- */ { return (Float(d1.value) >= d2); }
template <typename Tp> static inline bool operator >= (const Tp& d1, const Double& d2)    { return (Double(d1) >= d2); }
template </* float */> /*  */ inline bool operator >= (const float& d1, const Double& d2) { return (Float(d1) >= Float(d2.value)); }
template <typename Tp> static inline bool operator >= (const Double& d1, const Tp& d2)    { return (d1 >= Double(d2)); }
template </* float */> /*  */ inline bool operator >= (const Double& d1, const float& d2) { return (Float(d1.value) >= Float(d2)); }
// < Ops
static inline bool operator < (const Double& d1, const Double& d2) /* ----- main ---> */  { return (d1.value < d2.value - Double::eps); }
static inline bool operator < (const Double& d1, const Float& d2)  /* --------------- */  { return (Float(d1.value) < d2); }
template <typename Tp> static inline bool operator < (const Tp& d1, const Double& d2)     { return (Double(d1) < d2); }
template </* float */> /*  */ inline bool operator < (const float& d1, const Double& d2)  { return (Float(d1) < Float(d2.value)); }
template <typename Tp> static inline bool operator < (const Double& d1, const Tp& d2)     { return (d1 < Double(d2)); }
template </* float */> /*  */ inline bool operator < (const Double& d1, const float& d2)  { return (Float(d1.value) < Float(d2)); }
// > Ops
static inline bool operator > (const Double& d1, const Double& d2) /* ----- main ---> */  { return (d1.value > d2.value + Double::eps); }
static inline bool operator > (const Double& d1, const Float& d2)  /* --------------- */  { return (Float(d1.value) > d2); }
template <typename Tp> static inline bool operator > (const Tp& d1, const Double& d2)     { return (Double(d1) > d2); }
template </* float */> /*  */ inline bool operator > (const float& d1, const Double& d2)  { return (Float(d1) > Float(d2.value)); }
template <typename Tp> static inline bool operator > (const Double& d1, const Tp& d2)     { return (d1 > Double(d2)); }
template </* float */> /*  */ inline bool operator > (const Double& d1, const float& d2)  { return (Float(d1.value) > Float(d2)); }
// Unary Ops
static inline bool   operator ! (const Double& d1) { return (d1 == 0.0d); }
static inline Double operator - (const Double& d1) { return (Double(-d1.value)); }
static inline Double operator + (const Double& d1) { return (Double( (d1 < 0.0d) ? Double(-d1.value) : d1 )); }
// + Ops
static inline Double operator + (const Double& f1, const Double& f2)/* ------------- */ { return Double(f1.value + f2.value); }
static inline Double operator + (const Double& f1, const Float& f2) /* ------------- */ { return Double(f1.value + f2.value); }
template <typename Tp> static inline Double operator + (const Double& f1, const Tp& f2) { return Double(f1.value + double(f2)); }
template <typename Tp> static inline Double operator + (const Tp& f1, const Double& f2) { return Double(double(f1) + f2.value); }
// - Ops
static inline Double operator - (const Double& f1, const Double& f2)/* ------------- */ { return Double(f1.value - f2.value); }
static inline Double operator - (const Double& f1, const Float& f2) /* ------------- */ { return Double(f1.value - f2.value); }
template <typename Tp> static inline Double operator - (const Double& f1, const Tp& f2) { return Double(f1.value - double(f2)); }
template <typename Tp> static inline Double operator - (const Tp& f1, const Double& f2) { return Double(double(f1) - f2.value); }
// * Ops
static inline Double operator * (const Double& f1, const Double& f2)/* ------------- */ { return Double(f1.value * f2.value); }
static inline Double operator * (const Double& f1, const Float& f2) /* ------------- */ { return Double(f1.value * f2.value); }
template <typename Tp> static inline Double operator * (const Double& f1, const Tp& f2) { return Double(f1.value * double(f2)); }
template <typename Tp> static inline Double operator * (const Tp& f1, const Double& f2) { return Double(double(f1) * f2.value); }
// / Ops
static inline Double operator / (const Double& f1, const Double& f2)/* ------------- */ { return Double(f1.value / f2.value); }
static inline Double operator / (const Double& f1, const Float& f2) /* ------------- */ { return Double(f1.value / f2.value); }
template <typename Tp> static inline Double operator / (const Double& f1, const Tp& f2) { return Double(f1.value / double(f2)); }
template <typename Tp> static inline Double operator / (const Tp& f1, const Double& f2) { return Double(double(f1) / f2.value); }
// += Ops
static inline Double& operator += (Double& f1, const Double& f2)/* ------------- */ { f1.value += f2.value; return f1; }
static inline Double& operator += (Double& f1, const Float& f2) /* ------------- */ { f1.value += f2.value; return f1; }
template <typename Tp> static inline Double& operator += (Double& f1, const Tp& f2) { f1.value += double(f2); return f1; }
template <typename Tp> static inline Tp& operator += (Tp& f1, const Double& f2)/**/ { f1 += Tp(f2.value); return f1; }
// -= Ops
static inline Double& operator -= (Double& f1, const Double& f2)/* ------------- */ { f1.value -= f2.value; return f1; }
static inline Double& operator -= (Double& f1, const Float& f2) /* ------------- */ { f1.value -= f2.value; return f1; }
template <typename Tp> static inline Double& operator -= (Double& f1, const Tp& f2) { f1.value -= double(f2); return f1; }
template <typename Tp> static inline Tp& operator -= (Tp& f1, const Double& f2)/**/ { f1 -= Tp(f2.value); return f1; }
// *= Ops
static inline Double& operator *= (Double& f1, const Double& f2)/* ------------- */ { f1.value *= f2.value; return f1; }
static inline Double& operator *= (Double& f1, const Float& f2) /* ------------- */ { f1.value *= f2.value; return f1; }
template <typename Tp> static inline Double& operator *= (Double& f1, const Tp& f2) { f1.value *= double(f2); return f1; }
template <typename Tp> static inline Tp& operator *= (Tp& f1, const Double& f2)/**/ { f1 *= Tp(f2.value); return f1; }
// /= Ops
static inline Double& operator /= (Double& f1, const Double& f2)/* ------------- */ { f1.value /= f2.value; return f1; }
static inline Double& operator /= (Double& f1, const Float& f2) /* ------------- */ { f1.value /= f2.value; return f1; }
template <typename Tp> static inline Double& operator /= (Double& f1, const Tp& f2) { f1.value /= double(f2); return f1; }
template <typename Tp> static inline Tp& operator /= (Tp& f1, const Double& f2)/**/ { f1 /= Tp(f2.value); return f1; }
// Conversion Ops
inline Double::operator double() const { return this->value; }
inline Double::operator bool() const { return ( (*this) != Float(0.0d) ); }
inline Double::operator int() const {
   int   integer = int(this->value);
   Double decimal = this->value - integer;
   if(decimal < Double(0.5d)) return integer;
   else if(decimal > Double(0.5d)) return ++integer;
   else if(integer % 2 == 0) return integer;
   else return ++integer;
}

/////////// IO
static inline std::ostream& operator << (std::ostream& out, const Float& f) { return (out << f.value); }
static inline std::ostream& operator << (std::ostream& out, const Double& d) { return (out << d.value); }
static inline std::istream& operator >> (std::istream& in, Float& f) { return (in >> f.value); }
static inline std::istream& operator >> (std::istream& in, Double& d) { return (in >> d.value); }

} // namespace avr

#ifdef AVR_CORE_MODULE_HPP
/**
 * Templates specialization to become safe float point classes compatible with OpenCV matrices
 */
namespace cv {
   template<> class DataDepth<avr::Float> { public: enum { value = CV_32F, fmt=(int)'f' }; };
   template<> class DataDepth<avr::Double> { public: enum { value = CV_64F, fmt=(int)'d' }; };

   template<> class DataType<avr::Float> {
   public:
      typedef avr::Float value_type;
      typedef value_type work_type;
      typedef value_type channel_type;
      typedef value_type vec_type;
      enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 1,
           fmt=((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };

   };
   template<> class DataType<avr::Double> {
   public:
      typedef avr::Double value_type;
      typedef value_type work_type;
      typedef value_type channel_type;
      typedef value_type vec_type;
      enum { generic_type = 0, depth = DataDepth<channel_type>::value, channels = 1,
           fmt=((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };

   };
}
#endif // AVR_CORE_MODULE_HPP

#endif // __cplusplus

#endif // AVR_SAFE_FLOAT_POINT_TCC
