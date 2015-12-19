#ifndef AVR_TMATX_TCC
#define AVR_TMATX_TCC

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {

///////////////////////////////// Transformation Matrix /////////////////////////////////
inline TMatx::TMatx() : data(cv::Matx44d::eye()) { }
inline TMatx::TMatx(const TMatx& transf) : data(transf.data) { }
inline TMatx::TMatx(const Quaternion& q) : data(q.GetMatrix().data) { }
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
template <typename Tp> TMatx::TMatx(const Matx<Tp, 3, 3>& rotation) : data(cv::Matx44d::eye()) {
   for(int i = 0; i < 9; i++)
      this->data(i) = double(rotation(i));
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 3, 4>& transf) : data(cv::Matx44d::eye()) {
   for(int i = 0; i < 12; i++)
      this->data(i) = double(transf(i));
}
template <typename Tp> TMatx::TMatx(const Matx<Tp, 4, 4>& transf) : data() {
   for(int i = 0; i < 12; i++)
      this->data(i) = double(transf(i));
}

// Access a particular matrix cell
inline double& TMatx::At(uint8 row, uint8 col) { return this->data(row, col); }
inline const double& TMatx::At(uint8 row, uint8 col) const { return this->data(row, col); }
inline double& TMatx::operator ()(uint8 row, uint8 col) { return this->data(row, col); }
inline const double& TMatx::operator ()(uint8 row, uint8 col) const { return this->data(row, col); }

// Getters
inline Matx44d TMatx::GetMatx() const { return this->data; }
inline Point3d TMatx::GetPosition() const {
   return Point3d(this->data(0, 3), this->data(1, 3), this->data(2, 3));
}

// Statics methods
inline TMatx TMatx::Invert(const TMatx& matx) { return matx.data.inv(); }
inline TMatx TMatx::Transpose(const TMatx& matx) { return matx.data.t(); }
inline double TMatx::Determinant(const TMatx& matx) { return cv::determinant(matx.data); }

// Assign Operators
inline TMatx& TMatx::operator = (const TMatx& matx) {
   this->data = matx.data;
   return * this;
}
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
template <typename Tp> inline TMatx& TMatx::operator *= (Tp scalar) {
   this->data *= double(scalar);
   return * this;
}

// Arithmetic Operators
inline TMatx TMatx::operator + (const TMatx& matx) const { return this->data + matx.data; }
inline TMatx TMatx::operator - (const TMatx& matx) const { return this->data - matx.data; }
inline TMatx TMatx::operator * (const TMatx& matx) const { return this->data * matx.data; }

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
template <typename Tp> inline Point3<double> TMatx::operator * (const Point3<Tp>& pt) const {
   Matx<double, 4, 1> tmp = this->data * Point3<double>(pt);
   return Point3<double>(tmp(0), tmp(1), tmp(2));
}

// Conversion Operators
inline const cv::Matx<double, 4, 4>& TMatx::operator ()() const { return this->data; }
inline TMatx::operator double*() const {
   double* glm = new double[16];
   this->GetGLData(glm);
   return glm;
}

} // namespace avr

#endif // __cplusplus

#endif // AVR_TMATX_TCC
