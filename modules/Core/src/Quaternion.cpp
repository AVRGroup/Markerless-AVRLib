#include <avr/core/Core.hpp>

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {

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

   TMatx matx;;
   matx(0, 0) = 1.0 - (_2yy + _2zz);
   matx(1, 1) = 1.0 - (_2xx + _2zz);
   matx(2, 2) = 1.0 - (_2xx + _2yy);
   matx(0, 1) = _2xy + _2wz;
   matx(1, 0) = _2xy - _2wz;
   matx(0, 2) = _2xz - _2wy;
   matx(2, 0) = _2xz + _2wy;
   matx(1, 2) = _2yz + _2wx;
   matx(2, 1) = _2yz - _2wx;

   return matx;
}

} // namespace avr

#endif // __cplusplus
