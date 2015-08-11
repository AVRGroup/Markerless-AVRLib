#ifndef AVR_QUATERNION_TCC
#define AVR_QUATERNION_TCC

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {

//! Constructors
inline Quaternion::Quaternion() : data(Vec<double, 4>(1.0, 0.0, 0.0, 0.0)) { }
inline Quaternion::Quaternion(Degree angle, double x, double y, double z) : data() { this->create(angle, x, y, z); }
inline Quaternion::Quaternion(Degree angle, const Point3d& axis) : data() { this->create(angle, axis.x, axis.y, axis.z); }
inline Quaternion::Quaternion(Degree angle, const Vec3d& axis) : data() { this->create(angle, axis(0), axis(1), axis(2)); }
inline Quaternion::Quaternion(const Quaternion& q) : data(q.data) { }
inline Quaternion::Quaternion(const TMatx& matx) : data(matx.GetQuaternion().data) { }
template <typename Tp> inline Quaternion::Quaternion(const Point3<Tp>& p) : data(Vec<double, 4>(0.0, double(p.x), double(p.y), double(p.z))) { }
template <typename Tp> inline Quaternion::Quaternion(const Vec<Tp, 3>& v) : data(Vec<double, 4>(0.0, double(v(0)), double(v(1)), double(v(2)))) { }
template <typename Tp> inline Quaternion::Quaternion(const Vec<Tp, 4>& v) : data(Vec<double, 4>(double(v(0)), double(v(1)), double(v(2)), double(v(3)))) { }
template <typename Tp> inline Quaternion::Quaternion(const Scalar<Tp>& s) : data(Vec<double, 4>(double(s(0)), double(s(1)), double(s(2)), double(s(3)))) { }

//! Getters
inline double Quaternion::Qw() const { return this->data(0); }
inline double Quaternion::Qx() const { return this->data(1); }
inline double Quaternion::Qy() const { return this->data(2); }
inline double Quaternion::Qz() const { return this->data(3); }
inline Vec3d Quaternion::GetAxis() const {
   double sin = std::sin(std::acos(Qw()));
   return Vec3d(Qx()/sin, Qy()/sin, Qz()/sin);
}
inline double Quaternion::GetAngle() const {
   return avr::degrees(2.0 * std::acos(Qw()));
}

//! Static methods
inline double Quaternion::Norm(const Quaternion& q) {
   return std::sqrt(q.Qw() * q.Qw() + q.Qx() * q.Qx() + q.Qy() * q.Qy() + q.Qz() * q.Qz());
}
inline Quaternion Quaternion::Conjugate(const Quaternion& q) {
   return Quaternion(q.Qw(), -q.Qx(), -q.Qy(), -q.Qz());
}
inline Quaternion Quaternion::Inverse(const Quaternion& q) {
   return Quaternion::Conjugate(q) * (1.0/Quaternion::Norm(q));
}

//! Element-access Operators
inline double& Quaternion::operator () (uint8 i) { return this->data(i); }
inline const double& Quaternion::operator () (uint8 i) const { return this->data(i); }

//! Assign Operators
inline Quaternion& Quaternion::operator =  (const Quaternion& q) {
   this->data = q.data;
   return * this;
}
inline Quaternion& Quaternion::operator += (const Quaternion& q) {
   this->data += q.data;
   return * this;
}
inline Quaternion& Quaternion::operator -= (const Quaternion& q) {
   this->data -= q.data;
   return * this;
}
inline Quaternion& Quaternion::operator *= (const Quaternion& q) {
   this->data *= q.data;
   return * this;
}
inline Quaternion& Quaternion::operator *= (double scalar) {
   this->data = scalar * this->data;
   return * this;
}

//! Arithmetic Operators
inline Quaternion Quaternion::operator + (const Quaternion& q) const {
   return Quaternion(this->data + q.data);
}
inline Quaternion Quaternion::operator - (const Quaternion& q) const {
   return Quaternion(this->data + q.data);
}
inline Quaternion Quaternion::operator * (const Quaternion& q) const {
   return Quaternion(this->data + q.data);
}
inline Quaternion Quaternion::operator * (double scalar) const {
   return Quaternion(scalar * this->data);
}

//! Private methods
inline void Quaternion::create(Double deg, Double x, Double y, Double z) {
   double angle2 = avr::radians(deg) / 2.0;
   this->data(0) = std::cos(angle2);
   this->data(1) = x * std::sin(angle2);
   this->data(2) = y * std::sin(angle2);
   this->data(3) = z * std::sin(angle2);
}

} // namespace avr

#endif // __cplusplus

#endif // AVR_QUATERNION_TCC
