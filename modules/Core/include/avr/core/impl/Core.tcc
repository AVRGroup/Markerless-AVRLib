#ifndef AVR_CORE_MODULE_TCC
#define AVR_CORE_MODULE_TCC

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {
/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                  Angle units classes (Degrees and Radians)                                                      *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
//////////////////////////////////////////////////////////////////// Deg //////////////////////////////////////////////////////////////////////////
inline Deg::Deg() : value(0.0) {}
inline Deg::Deg(const double& v) : value(v) {}
inline Deg::Deg(const Rad& rad) : value(rad.ToDeg().value) {}
inline double Deg::Get() const { return this->value; }
inline Rad Deg::ToRad() const { return Rad(static_cast<double>(value * Rad4Deg.value)); }

//////////////////////////////////////////////////////////////////// Rad //////////////////////////////////////////////////////////////////////////
inline Rad::Rad() : value(0.0) {}
inline Rad::Rad(const double& v) : value(v) {}
inline Rad::Rad(const Deg& deg) : value(deg.ToRad().value) {}
inline double Rad::Get() const { return this->value; }
inline Deg Rad::ToDeg() const { return Deg(static_cast<double>(value * Deg4Rad.value)); }

} // namespace avr

// Trigonometric functions overload
inline double avr::sin(const avr::Deg& deg) { return std::sin(deg.ToRad().value); }
inline double avr::cos(const avr::Deg& deg) { return std::cos(deg.ToRad().value); }
inline double avr::tan(const avr::Deg& deg) { return std::tan(deg.ToRad().value); }
inline double avr::sin(const avr::Rad& rad) { return std::sin(rad.value); }
inline double avr::cos(const avr::Rad& rad) { return std::cos(rad.value); }
inline double avr::tan(const avr::Rad& rad) { return std::tan(rad.value); }
inline avr::Rad avr::asin(const float& v)   { return avr::Rad(std::asin(v)); }
inline avr::Rad avr::acos(const float& v)   { return avr::Rad(std::acos(v)); }
inline avr::Rad avr::atan(const float& v)   { return avr::Rad(std::atan(v)); }
inline avr::Rad avr::asin(const double& v)  { return avr::Rad(std::asin(v)); }
inline avr::Rad avr::acos(const double& v)  { return avr::Rad(std::acos(v)); }
inline avr::Rad avr::atan(const double& v)  { return avr::Rad(std::atan(v)); }

namespace avr {
/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                   Bounding Rectangle template class                                                             *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
////////////////////////////////////////////////////////////// BoundingRect<> /////////////////////////////////////////////////////////////////////
template <typename Tp> inline BRect<Tp>::BRect() : data() {}
template <typename Tp> inline BRect<Tp>::BRect(Tp w, Tp h, Tp x, Tp y) : data(x, y, w, h) {}
template <typename Tp> inline BRect<Tp>::BRect(const Point_<Tp>& p, const Size_<Tp>& s) : data(p, s) {}
template <typename Tp> inline BRect<Tp>::BRect(const Point_<Tp>& p1, const Point_<Tp>& p2) : data(p1, p2) {}
template <typename Tp> inline BRect<Tp>::BRect(const BRect<Tp>& rect) : data(rect.data) {}
template <typename Tp> inline BRect<Tp>::BRect(const Rect_<Tp>& _data) : data(_data) {}
template <typename Tp> template <typename _Tp> inline BRect<Tp>::BRect(const BRect<_Tp>& rect) : data(rect.data) {}
template <typename Tp> template <typename _Tp> inline BRect<Tp>::BRect(const Rect_<_Tp>& _data) : data(_data) {}
// Getters
template <typename Tp> inline Tp BRect<Tp>::GetArea() const { return (data.width * data.height); }
template <typename Tp> inline Point2f BRect<Tp>::GetCenter() const { return Point2f((data.width - data.x)/2.f, (data.height - data.y)/2.f); }
template <typename Tp> inline Size_<Tp> BRect<Tp>::GetSize() const { return data.size(); }
// Checkers
template <typename Tp> inline bool BRect<Tp>::Contains(const Point_<Tp>& p) const { return data.contains(p); }
template <typename Tp> inline bool BRect<Tp>::Intersects(const BRect<Tp>& rect) const {
   Rect_<Tp> in = this->data & rect.data;
   return (in.width != 0);
}
// == Ops
template <typename Tp> inline bool BRect<Tp>::operator == (const BRect<Tp>& rect) const { return (this->data == rect.data); }
template </* float */> inline bool BRect<float>::operator == (const BRect<float>& rect) const {
   return (float(this->data.x) == float(rect.data.x) && float(this->data.y) == float(rect.data.y) &&
            float(this->data.width) == float(rect.data.width) && float(this->data.height) == float(rect.data.height));
}
template </* double*/> inline bool BRect<double>::operator == (const BRect<double>& rect) const {
   return (double(this->data.x) == double(rect.data.x) && double(this->data.y) == double(rect.data.y) &&
            double(this->data.width) == double(rect.data.width) && double(this->data.height) == double(rect.data.height));
}
// != Ops
template <typename Tp> inline bool BRect<Tp>::operator != (const BRect<Tp>& rect) const { return !(this->data == rect.data); }
template </* float */> inline bool BRect<float>::operator != (const BRect<float>& rect) const { return !( (*this) == rect ); }
template </* double*/> inline bool BRect<double>::operator != (const BRect<double>& rect) const { return !( (*this) == rect ); }
// ! Ops
template <typename Tp> inline bool BRect<Tp>::operator ! () const { return (this->data.width == Tp(0) || this->data.height == Tp(0)); }
template </* float */> inline bool BRect<float>::operator ! () const { return (float(this->data.width) == 0.0f || float(this->data.height) == 0.0f); }
template </* double*/> inline bool BRect<double>::operator ! () const { return (double(this->data.width) == 0.0 || double(this->data.height) == 0.0); }
// Updates Ops
template <typename Tp> inline BRect<Tp>& BRect<Tp>::operator ()(Tp x, Tp y, Tp w, Tp h) {
   data.x = x; data.width = w;
   data.y = y; data.height = h;
   return * this;
}
template <typename Tp> inline BRect<Tp>& BRect<Tp>::operator ()(const Point_<Tp>& p, const Size_<Tp>& s) {
   data.x = p.x; data.width = s.width;
   data.y = p.y; data.height = s.height;
   return * this;
}
template <typename Tp> inline BRect<Tp>& BRect<Tp>::operator ()(const Point_<Tp>& p1, const Point_<Tp>& p2) {
   data.x = std::min(p1.x, p2.x); data.width = std::max(p1.x, p2.x) - data.x;
   data.y = std::min(p1.y, p2.y); data.height = std::max(p1.y, p2.y) - data.y;
   return * this;
}

/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                        Transformation Matrix class                                                              *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
//////////////////////////////////////////////////////////////////// TMatx ////////////////////////////////////////////////////////////////////////
inline TMatx::TMatx() : data(cv::Matx44d::eye()) { }
inline TMatx::TMatx(const TMatx& transf) : data(transf.data) { }
inline TMatx::TMatx(const Quaternion& rot, const Point3d& pos) : data(rot.GetMatrix().data) {
   this->data(0, 3) = pos.x;
   this->data(1, 3) = pos.y;
   this->data(2, 3) = pos.z;
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 3, 3>& rotation, const Vec<Tp, 3>& translation) : data(cv::Matx44d::eye()) {
   for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++)
         this->data(i, j) = double(rotation(i, j));
      this->data(i, 3) = double(translation[i]);
   }
   //OpenGL has reversed Y & Z coords
   Matx44d reverseyz = Matx44d::eye();
   reverseyz(1, 1) = reverseyz(2, 2) = -1;
   this->data = reverseyz * this->data;
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 3, 3>& rotation) : data() {
   for(int i = 0; i < 3; i++)
      for(int j = 0; j < 3; j++)
         this->data(i, j) = double(rotation(i, j));
   this->data(3, 3) = 1.0;
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 3, 4>& transf) : data() {
   for(int i = 0; i < 12; i++)
      this->data(i) = double(transf(i));
   this->data(3, 3) = 1.0;
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 4, 4>& transf) : data() {
   for(int i = 0; i < 16; i++)
      this->data(i) = double(transf(i));
}
#if __cplusplus > 199711L // C++11
template <typename Tp> TMatx::TMatx(std::initializer_list<Tp> _list) : data(Matx44d::eye()) {
   auto it = _list.begin();
   for(size_t i = 0; i < std::min(_list.size(), size_t(16)); i++, it++)
      this->data(i) = double(*it);
}
#endif // __cplusplus
// Access a particular matrix cell
inline double& TMatx::At(ubyte row, ubyte col) { return this->data(row, col); }
inline const double& TMatx::At(ubyte row, ubyte col) const { return this->data(row, col); }
inline double& TMatx::operator ()(ubyte row, ubyte col) { return this->data(row, col); }
inline const double& TMatx::operator ()(ubyte row, ubyte col) const { return this->data(row, col); }
// Getters
inline Point3d TMatx::GetPosition() const {
   return Point3d(this->data(0, 3), this->data(1, 3), this->data(2, 3));
}
// Transpose, Inverse and Determinant operations
inline TMatx TMatx::T() const { return this->data.t(); }
inline TMatx TMatx::Inv() const { return this->data.inv(); }
inline double TMatx::Det() const { return cv::determinant(this->data); }
// Operator overlodas
inline TMatx& TMatx::operator += (const TMatx& matx) {
   this->data += matx.data;
   return * this;
}
inline TMatx& TMatx::operator -= (const TMatx& matx) {
   this->data -= matx.data;
   return * this;
}
inline TMatx& TMatx::operator *= (const TMatx& matx) {
   this->data = this->data * matx.data;
   return * this;
}
inline TMatx TMatx::operator + (const TMatx& matx) const { return this->data + matx.data; }
inline TMatx TMatx::operator - (const TMatx& matx) const { return this->data - matx.data; }
inline TMatx TMatx::operator * (const TMatx& matx) const { return this->data * matx.data; }
// Template operator overlodas
template <typename Tp> inline TMatx& TMatx::operator *= (Tp scalar) {
   this->data *= double(scalar);
   return * this;
}
template <typename Tp> inline TMatx TMatx::operator * (Tp scalar) const {
   return double(scalar) * this->data;
}
template <typename Tp> inline Vec<double, 4> TMatx::operator * (const Vec<Tp, 4>& vec) const {
   return this->data * Vec<double, 4>(vec);
}
template <typename Tp> inline Vec<double, 3> TMatx::operator * (const Vec<Tp, 3>& vec) const {
   Vec<double, 4> tmp (double(vec(0)), double(vec(1)), double(vec(2)), 1.0);
   tmp = this->data * tmp;
   return Vec<double, 3>(tmp[0], tmp[1], tmp[2]);
}
template <typename Tp> inline Point3_<double> TMatx::operator * (const Point3_<Tp>& pt) const {
   Matx<double, 4, 1> tmp = this->data * Point3_<double>(pt);
   return Point3_<double>(tmp(0), tmp(1), tmp(2));
}

/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                         Rotation Quaternion class                                                               *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
///////////////////////////////////////////////////////////////// Quaternion //////////////////////////////////////////////////////////////////////
inline Quaternion::Quaternion() : data(Vec<double, 4>(1.0, 0.0, 0.0, 0.0)) { }
inline Quaternion::Quaternion(const Deg& angle, double x, double y, double z) : data() { this->create(angle.ToRad().value, x, y, z); }
inline Quaternion::Quaternion(const Deg& angle, const Point3d& axis) : data() { this->create(angle.ToRad().value, axis.x, axis.y, axis.z); }
inline Quaternion::Quaternion(const Deg& angle, const Vec3d& axis) : data() { this->create(angle.ToRad().value, axis(0), axis(1), axis(2)); }
inline Quaternion::Quaternion(const Quaternion& q) : data(q.data) { }
inline Quaternion::Quaternion(const TMatx& matx) : data(matx.GetQuaternion().data) { }
template <typename Tp> inline Quaternion::Quaternion(const Point3_<Tp>& p) : data(Vec<double, 4>(0.0, double(p.x), double(p.y), double(p.z))) { }
template <typename Tp> inline Quaternion::Quaternion(const Vec<Tp, 3>& v) : data(Vec<double, 4>(0.0, double(v(0)), double(v(1)), double(v(2)))) { }
template <typename Tp> inline Quaternion::Quaternion(const Vec<Tp, 4>& v) : data(Vec<double, 4>(double(v(0)), double(v(1)), double(v(2)), double(v(3)))) { }
template <typename Tp> inline Quaternion::Quaternion(const Scalar_<Tp>& s) : data(Vec<double, 4>(double(s(0)), double(s(1)), double(s(2)), double(s(3)))) { }
// Getters
inline double Quaternion::Qw() const { return this->data(0); }
inline double Quaternion::Qx() const { return this->data(1); }
inline double Quaternion::Qy() const { return this->data(2); }
inline double Quaternion::Qz() const { return this->data(3); }
inline Deg Quaternion::GetAngle() const { return Rad(2.0 * avr::acos(Qw())).ToDeg(); }
inline Vec3d Quaternion::GetAxis() const {
   if(Qw() == 0.0) return Vec3d(Qx(), Qy(), Qz());
   if(Qw() == 1.0) return Vec3d(0.0, 0.0, 0.0);
   double sin = avr::sin(avr::acos(Qw()));
   return Vec3d(Qx()/sin, Qy()/sin, Qz()/sin);
}
// Static methods
inline double Quaternion::Norm() const       { return std::sqrt(double(Qw() * Qw() + Qx() * Qx() + Qy() * Qy() + Qz() * Qz())); }
inline Quaternion Quaternion::Conj() const   { return Quaternion(Qw(), -Qx(), -Qy(), -Qz()); }
inline Quaternion Quaternion::Inv() const    { return Conj() * (1.0/Norm()); }
// Element-access operators
inline double& Quaternion::operator [] (ubyte i)               { return this->data[i]; }
inline const double& Quaternion::operator [] (ubyte i) const   { return this->data[i]; }
// Assign operator overloads
inline Quaternion& Quaternion::operator += (const Quaternion& q)  { this->data += q.data; return * this; }
inline Quaternion& Quaternion::operator -= (const Quaternion& q)  { this->data -= q.data; return * this; }
inline Quaternion& Quaternion::operator *= (const Quaternion& q)  { this->data *= q.data; return * this; }
inline Quaternion& Quaternion::operator *= (const double& scalar) { this->data = this->data * double(scalar); return * this; }
// Arithmetic operator overloads
inline Quaternion Quaternion::operator + (const Quaternion& q) const    { return Quaternion(this->data + q.data); }
inline Quaternion Quaternion::operator - (const Quaternion& q) const    { return Quaternion(this->data - q.data); }
inline Quaternion Quaternion::operator * (const Quaternion& q) const    { return Quaternion(this->data * q.data); }
inline Quaternion Quaternion::operator * (const double& scalar) const   { return Quaternion(this->data * double(scalar)); }
// Private methods
inline Quaternion::Quaternion(double qw, double qx, double qy, double qz) : data(Vec<double, 4>(qw, qx, qy, qz)) { }
inline void Quaternion::create(Cdouble theta, Cdouble x, Cdouble y, Cdouble z) {
   const double theta2 = double(theta / 2.0);
   this->data(0) = std::cos(theta2);
   this->data(1) = x * std::sin(theta2);
   this->data(2) = y * std::sin(theta2);
   this->data(3) = z * std::sin(theta2);
}

/**---------------------------------------------------------------------------------------------------------------------------------------------**\
*                                       IO operator overloads to cv::core and avr::Core classes                                                   *
\**---------------------------------------------------------------------------------------------------------------------------------------------**/
//////////////// Size_<>
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Size_<_Tp>& b) { return (in >> b.width >> b.height); }
////////////////// Range
static inline std::ostream& operator << (std::ostream& out, const Range& r) { return (out << "[" << r.start << ":" << r.end << "]"); }
static inline std::istream& operator >> (std::istream& in, Range& r) { return (in >> r.start >> r.end); }
/////////////// Point_<>
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Point_<_Tp>& b) { return (in >> b.x >> b.y); }
////////////// Point3_<>
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Point3_<_Tp>& b) { return (in >> b.x >> b.y >> b.z); }
////////////// Complex<>
template <typename _Tp> static inline std::ostream& operator << (std::ostream& out, const Complex<_Tp>& b) { return (out << b.re << "+" << b.im << "i"); }
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Complex<_Tp>& b) { return (in >> b.re >> b.im); }
////////////// Scalar_<>
template <typename _Tp> static inline std::istream& operator >> (std::istream& in, Scalar_<_Tp>& s) {
   return (in >> s[0] >> s[1] >> s[2] >> s[3]);
}
//////////////// BRect<>
template <typename _Tp> static inline std::ostream& operator << (std::ostream& out, const BRect<_Tp>& rect) { return (out << rect.Get()); }
////////////////// TMatx
static inline std::ostream& operator << (std::ostream& out, const TMatx& tm) {
   out << "|" << tm(0, 0) << ", " << tm(0, 1) << ", " << tm(0, 2) << ", " << tm(0, 3) << "|" << endl;
   out << "|" << tm(1, 0) << ", " << tm(1, 1) << ", " << tm(1, 2) << ", " << tm(1, 3) << "|" << endl;
   out << "|" << tm(2, 0) << ", " << tm(2, 1) << ", " << tm(2, 2) << ", " << tm(2, 3) << "|" << endl;
   out << "|" << tm(3, 0) << ", " << tm(3, 1) << ", " << tm(3, 2) << ", " << tm(3, 3) << "|";
   return out;
}
///////////// Quaternion
static inline std::ostream& operator << (std::ostream& out, const Quaternion& q) {
   return (out << "[" << q[0] << ", [" << q[1] << ", " << q[2] << ", " << q[3] << "]]");
}
static inline std::istream& operator >> (std::istream& in, Quaternion& q) {
   return (in >> q[0] >> q[1] >> q[2] >> q[3]);
}
//////////// Deg and Rad
static inline std::ostream& operator << (std::ostream& out, const Deg& deg) { return (out << deg.value << (char)248); }
static inline std::ostream& operator << (std::ostream& out, const Rad& rad) { return (out << rad.value << "r"); }
static inline std::istream& operator >> (std::istream& in, Deg& deg) { return (in >> deg.value); }
static inline std::istream& operator >> (std::istream& in, Rad& rad) { return (in >> rad.value); }

} // namespace avr

#endif // __cplusplus

#endif // AVR_CORE_MODULE_TCC
