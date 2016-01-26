#include <avr/core/Core.hpp>

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {
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
   Matx44d matx = this->data;

   float det = float(cv::determinant(matx));
   if(det != 1.0f) matx *= (1.0/double(det));

   Quaternion q;
   // 0: qw; 1: qx; 2: qy; 3: qz
   q[0] = matx(0, 0) + matx(1, 1) + matx(2, 2) + 1.0;
   if(Double(q[0]) < 0.0) return Quaternion();

   q[0] = std::sqrt(q[0]);
   q[1] = (matx(2, 1) - matx(1, 2)) / (2.0 * q[0]);
   q[2] = (matx(0, 2) - matx(2, 0)) / (2.0 * q[0]);
   q[3] = (matx(1, 0) - matx(0, 1)) / (2.0 * q[0]);
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
   Vec4d q = this->data;

   float norm = cv::norm(q);
   if(norm != 1.0f) q = cv::normalize(q);
   // 0: qw; 1: qx; 2: qy; 3: qz
   double _2xx = 2.0 * q[1] * q[1];
   double _2xy = 2.0 * q[1] * q[2];
   double _2xz = 2.0 * q[1] * q[3];
   double _2yy = 2.0 * q[2] * q[2];
   double _2yz = 2.0 * q[2] * q[3];
   double _2zz = 2.0 * q[3] * q[3];
   double _2wx = 2.0 * q[0] * q[1];
   double _2wy = 2.0 * q[0] * q[2];
   double _2wz = 2.0 * q[0] * q[3];

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
