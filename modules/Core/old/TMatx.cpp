#include <avr/core/Core.hpp>

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {

///////////////////////////////// Transformation Matrix /////////////////////////////////
TMatx::TMatx(const double (&v)[16]) : data() {
   for(int i = 0; i < 16; i++) this->data(i) = v[i];
}

Quaternion TMatx::GetQuaternion() const {
   Quaternion q;
   q(0) = this->At(0, 0) + this->At(1, 1) + this->At(2, 2) + 1.0;
   if( q(0) < 0.0 ) return Quaternion();

   q(0) = sqrt( q(0) );
   q(1) = (this->At(1, 2) - this->At(2, 1)) / (2.0 * q(0));
   q(2) = (this->At(2, 0) - this->At(0, 2)) / (2.0 * q(0));
   q(3) = (this->At(0, 1) - this->At(1, 0)) / (2.0 * q(0));
   q(0) = q(0) / 2.0;

   return q;
}

void TMatx::GetGLData(double glm[16]) const {
   for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
         glm[i * 4 + j] = this->data(j, i);
}

} // namespace avr

#endif // __cplusplus
