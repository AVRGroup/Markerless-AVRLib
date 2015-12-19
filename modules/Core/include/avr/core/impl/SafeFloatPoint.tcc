#ifndef AVR_SAFE_FLOAT_POINT_TCC
#define AVR_SAFE_FLOAT_POINT_TCC

#ifdef __cplusplus

namespace avr {
/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                         Safe float point classes                                                                *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
//////////////////////////////////////////////////////////////// FPOperand ////////////////////////////////////////////////////////////////////////
inline FPOperand::FPOperand(const Float& f) : val(f.value), eps(Float::eps) { }
inline FPOperand::FPOperand(const Double& d) : val(d.value), eps(Double::eps) { }
inline FPOperand::FPOperand(const double& _val, const double& _eps) : val(_val), eps(_eps) { }
template <typename Tp> inline FPOperand::FPOperand(const Tp& _val) : val(float(_val)), eps(Float::eps) { }
template </*double */> inline FPOperand::FPOperand(const double& _val) : val(_val), eps(Double::eps){ }
template </*ldouble*/> inline FPOperand::FPOperand(const long double& _val) : val(_val), eps(Double::eps) { }
// Conversion Ops
inline FPOperand::operator Float() const { return Float(this->val); }
inline FPOperand::operator Double() const { return Double(this->val); }
// Arithmetic Ops Declaration
extern FPOperand operator + (const FPOperand&, const FPOperand&);
extern FPOperand operator - (const FPOperand&, const FPOperand&);
extern FPOperand operator * (const FPOperand&, const FPOperand&);
extern FPOperand operator / (const FPOperand&, const FPOperand&);
// Logical Ops Declaration
extern bool operator == (const FPOperand&, const FPOperand&);
extern bool operator != (const FPOperand&, const FPOperand&);
extern bool operator <= (const FPOperand&, const FPOperand&);
extern bool operator >= (const FPOperand&, const FPOperand&);
extern bool operator <  (const FPOperand&, const FPOperand&);
extern bool operator >  (const FPOperand&, const FPOperand&);
extern bool operator && (const FPOperand&, const FPOperand&);
extern bool operator || (const FPOperand&, const FPOperand&);

////////////////////////////////////////////////////////////////// Float //////////////////////////////////////////////////////////////////////////
inline Float::Float() : value(0.0f) { }
inline Float::Float(const float& v) : value(v) { }
inline Float::Float(const Double& d) : value(float(d.value)) { }
inline Float::Float(const FPOperand& opd) : value(float(opd.val)) { }
inline float Float::Get() const { return this->value; }
// Unary Ops
inline bool   Float::operator ! () const { return ((*this) == 0.0f); }
inline Float  Float::operator - () const { return (Float(-value)); }
inline Float  Float::operator + () const { return (Float( ((*this) < 0.0f) ? Float(-value) : Float(value) )); }
inline Float& Float::operator ++ (int) { ++this->value; return * this; }
inline Float& Float::operator -- (int) { --this->value; return * this; }
inline Float& Float::operator ++ (void) { this->value++; return * this; }
inline Float& Float::operator -- (void) { this->value--; return * this; }
// Assign Ops
inline Float& Float::operator =  (const Float& f)   { this->value =  f.value; return * this; }
inline Float& Float::operator =  (const FPOperand& e) { this->value =  float(e.val); return * this; }
inline Float& Float::operator += (const FPOperand& e) { this->value += float(e.val); return * this; }
inline Float& Float::operator -= (const FPOperand& e) { this->value -= float(e.val); return * this; }
inline Float& Float::operator *= (const FPOperand& e) { this->value *= float(e.val); return * this; }
inline Float& Float::operator /= (const FPOperand& e) { this->value /= float(e.val); return * this; }

////////////////////////////////////////////////////////////////// Double //////////////////////////////////////////////////////////////////////////
inline Double::Double() : value(0.0) { }
inline Double::Double(const double& v) : value(v) { }
inline Double::Double(const Float& f) : value(double(f.value)) { }
inline Double::Double(const FPOperand& opd) : value(opd.val) { }
inline double Double::Get() const { return this->value; }
// Unary Ops
inline bool   Double::operator ! () const { return ((*this) == 0.0f); }
inline Double  Double::operator - () const { return (Double(-value)); }
inline Double  Double::operator + () const { return (Double( ((*this) < 0.0f) ? Double(-value) : Double(value) )); }
inline Double& Double::operator ++ (int) { ++this->value; return * this; }
inline Double& Double::operator -- (int) { --this->value; return * this; }
inline Double& Double::operator ++ (void) { this->value++; return * this; }
inline Double& Double::operator -- (void) { this->value--; return * this; }
// Assign Ops
inline Double& Double::operator =  (const Double& f)    { this->value =  f.value; return * this; }
inline Double& Double::operator =  (const FPOperand& e) { this->value =  e.val; return * this; }
inline Double& Double::operator += (const FPOperand& e) { this->value += e.val; return * this; }
inline Double& Double::operator -= (const FPOperand& e) { this->value -= e.val; return * this; }
inline Double& Double::operator *= (const FPOperand& e) { this->value *= e.val; return * this; }
inline Double& Double::operator /= (const FPOperand& e) { this->value /= e.val; return * this; }

//////////// IO ////////////
#ifdef _GLIBCXX_OSTREAM
   static inline std::ostream& operator << (std::ostream& out, const Float& f) { return (out << f.value); }
   static inline std::ostream& operator << (std::ostream& out, const Double& d) { return (out << d.value); }
#endif // _GLIBCXX_OSTREAM
#ifdef _GLIBCXX_ISTREAM
   static inline std::istream& operator >> (std::istream& in, Float& f) { return (in >> f.value); }
   static inline std::istream& operator >> (std::istream& in, Double& d) { return (in >> d.value); }
#endif // _GLIBCXX_ISTREAM

} // namespace avr

// Arithmetic Ops
inline avr::FPOperand avr::operator + (const FPOperand& e1, const FPOperand& e2) {
   return FPOperand(e1.val + e2.val, (e1.eps <= e2.eps) ? e1.eps : e2.eps);
}
inline avr::FPOperand avr::operator - (const FPOperand& e1, const FPOperand& e2) {
   return FPOperand(e1.val - e2.val, (e1.eps <= e2.eps) ? e1.eps : e2.eps);
}
inline avr::FPOperand avr::operator * (const FPOperand& e1, const FPOperand& e2) {
   return FPOperand(e1.val * e2.val, (e1.eps <= e2.eps) ? e1.eps : e2.eps);
}
inline avr::FPOperand avr::operator / (const FPOperand& e1, const FPOperand& e2) {
   return FPOperand(e1.val / e2.val, (e1.eps <= e2.eps) ? e1.eps : e2.eps);
}
// Logical Ops
inline bool avr::operator == (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e2.val - eps <= e1.val && e1.val <= e2.val + eps);
}
inline bool avr::operator != (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e1.val < e2.val - eps || e2.val + eps < e1.val);
}
inline bool avr::operator <= (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e1.val <= e2.val + eps);
}
inline bool avr::operator >= (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e1.val >= e2.val - eps);
}
inline bool avr::operator <  (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e1.val < e2.val - eps);
}
inline bool avr::operator >  (const FPOperand& e1, const FPOperand& e2) {
   double eps = (e1.eps >= e2.eps) ? e1.eps : e2.eps;
   return (e1.val > e2.val + eps);
}
inline bool avr::operator && (const FPOperand& e1, const FPOperand& e2) {
   return (e1 == 1.0 && e2 == 1.0);
}
inline bool avr::operator || (const FPOperand& e1, const FPOperand& e2) {
   return (e1 == 1.0 || e2 == 1.0);
}

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
