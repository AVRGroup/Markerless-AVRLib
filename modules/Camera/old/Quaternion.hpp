#ifndef AVR_QUATERNION_HPP
#define AVR_QUATERNION_HPP

#ifdef __cplusplus

#include "Point.hpp"

namespace avr {

class TransformationMatrix;

class Quaternion {
   private:
      double qx, qy, qz, w;

   public:
      explicit Quaternion(const Point3d& rotationAxis, double angle);
      Quaternion();
      ~Quaternion();

      inline double GetQx() const { return this->qx; }
      inline double GetQy() const { return this->qy; }
      inline double GetQz() const { return this->qz; }
      inline double GetW()  const { return this->w;  }

      TransformationMatrix& GetMatrix() const;

      friend class TransformationMatrix;
};

} //namespace avr

#endif // __cplusplus
#endif // AVR_QUATERNION_HPP
