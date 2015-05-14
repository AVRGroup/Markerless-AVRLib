#include "TransformationMatrix.hpp"
#include "Quaternion.hpp"
//#include "Point.hpp"

#include <cmath>

using namespace avr;

Quaternion::Quaternion() : qx(0.0), qy(0.0), qz(0.0), w(1.0){}
Quaternion::Quaternion(const Point3d& axis, double angle) {
   this->qx = axis.GetX() * sin(angle / 2.0);
   this->qx = axis.GetY() * sin(angle / 2.0);
   this->qx = axis.GetZ() * sin(angle / 2.0);
   this->w  = cos(angle / 2.0);
}
Quaternion::~Quaternion(){ /* dtor */ }

TransformationMatrix& Quaternion::GetMatrix() const {
   double _2x = 2.0 * this->qx;
   double _2y = 2.0 * this->qy;
   double _2z = 2.0 * this->qz;
   double _2w = 2.0 * this->w;

   double _2xx = _2x * this->qx;
   double _2xy = _2x * this->qy;
   double _2xz = _2x * this->qz;
   double _2yy = _2y * this->qy;
   double _2yz = _2y * this->qz;
   double _2zz = _2z * this->qz;
   double _2wx = _2w * this->qx;
   double _2wy = _2w * this->qy;
   double _2wz = _2w * this->qz;

   TMat& matx = *(new TransformationMatrix());
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
