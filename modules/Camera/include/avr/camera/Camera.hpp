#ifndef AVR_CAMERA_HPP
#define AVR_CAMERA_HPP

#include <avr/core/Core.hpp>

namespace avr {

class Camera {
   private:
      std::vector<double>  distortion;
      Matx<double, 3, 3>   intrinsecs;
      Size_<double>        resolution;

   public:
      explicit Camera(const std::string& filename);
      ~Camera();

      //! 2D world coordinate (z = 0)
      void Pose(const std::vector<Point2f>& world,
                const std::vector<Point2f>& location,
                TMatx& pose);

      //! 3D world coordinate
      void Pose(const std::vector<Point3f>& world,
                const std::vector<Point2f>& location,
                TMatx& pose);

      TMatx GetProjection();
};

}

#endif // AVR_CAMERA_HPP
