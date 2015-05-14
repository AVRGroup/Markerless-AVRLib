#ifndef AVR_CAMERA_HPP
#define AVR_CAMERA_HPP

#include <string>
#include <vector>

#include "Point.hpp"

namespace avr {

class TransformationMatrix;

class Camera {
   private:
      class IntrinsicParams;

      double            width, height;
      IntrinsicParams&  cameraParams;

   public:
      explicit Camera(const std::string& filename);
      ~Camera();

      //! 2D world coordinate (z = 0)
      void Pose(const std::vector<Point2f>& world,
                const std::vector<Point2f>& location,
                TransformationMatrix& pose);

      //! 3D world coordinate
      void Pose(const std::vector<Point3f>& world,
                const std::vector<Point2f>& location,
                TransformationMatrix& pose);

      TransformationMatrix& GetProjectionMatrix();
};

}

#endif // AVR_CAMERA_HPP
