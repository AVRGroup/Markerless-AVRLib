#include <avr/core/Core.hpp>

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {
///////////////////////////// initialization of static data members /////////////////////////////
const float  Float::eps    = std::numeric_limits<float>::epsilon();
const double Double::eps   = std::numeric_limits<double>::epsilon();
const Double Rad::PI       = 4.0 * std::atan(static_cast<long double>(1.0));

////////////////////////////////////  Transformation Matrix ////////////////////////////////////
TMatx::TMatx(const double (&buf)[16], bool byRow) : data() {
   if(byRow) {
      for(int i = 0; i < 4; i++)
         for(int j = 0; j < 4; j++)
            this->data(i, j) = buf[i * 4 + j];
   } else {
      for(int i = 0; i < 4; i++)
         for(int j = 0; j < 4; j++)
            this->data(j, i) = buf[i * 4 + j];
   }
}

Quaternion TMatx::GetQuaternion() const {
   Quaternion q;
   q[0] = this->At(0, 0) + this->At(1, 1) + this->At(2, 2) + 1.0;
   if(Double(q[0]) < 0.0) return Quaternion();

   q[0] = std::sqrt(q[0]);
   q[1] = (this->At(2, 1) - this->At(1, 2)) / (2.0 * q[0]);
   q[2] = (this->At(0, 2) - this->At(2, 0)) / (2.0 * q[0]);
   q[3] = (this->At(1, 0) - this->At(0, 1)) / (2.0 * q[0]);
   q[0] = q[0] / 2.0;

   return q;
}

void TMatx::GetData(double buf[16], bool byRow) const {
   if(byRow) {
      for(int i = 0; i < 4; i++)
         for(int j = 0; j < 4; j++)
            buf[i * 4 + j] = this->data(i, j);
   } else {
      for(int i = 0; i < 4; i++)
         for(int j = 0; j < 4; j++)
            buf[i * 4 + j] = this->data(j, i);
   }
}

///////////////////////////////////////// Quaternion ////////////////////////////////////////
TMatx Quaternion::GetMatrix() const {
   double _2x = 2.0 * Qx();
   double _2y = 2.0 * Qy();
   double _2z = 2.0 * Qz();
   double _2w = 2.0 * Qw();

   double _2xx = _2x * Qx();
   double _2xy = _2x * Qy();
   double _2xz = _2x * Qz();
   double _2yy = _2y * Qy();
   double _2yz = _2y * Qz();
   double _2zz = _2z * Qz();
   double _2wx = _2w * Qx();
   double _2wy = _2w * Qy();
   double _2wz = _2w * Qz();

   TMatx matx;
   matx(0, 0) = 1.0 - (_2yy + _2zz);
   matx(1, 1) = 1.0 - (_2xx + _2zz);
   matx(2, 2) = 1.0 - (_2xx + _2yy);
   matx(0, 1) = _2xy - _2wz;
   matx(1, 0) = _2xy + _2wz;
   matx(0, 2) = _2xz + _2wy;
   matx(2, 0) = _2xz - _2wy;
   matx(1, 2) = _2yz - _2wx;
   matx(2, 1) = _2yz + _2wx;

   return matx;
}

} // namespace avr

#endif // __cplusplus
