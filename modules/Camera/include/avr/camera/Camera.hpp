#ifndef AVR_CAMERA_HPP
#define AVR_CAMERA_HPP

#include <avr/core/Core.hpp>

namespace avr {

class Camera {
public:
   //! @param filename Path to camera calibration file
   explicit Camera(const std::string& filename);
   virtual ~Camera() {/* dtor */}

   //! @return The camera's pose matrix
   TMatx Pose(const std::vector<Point3d>& world,
             const std::vector<Point2d>& location,
             bool lost = true) const;

   //! @return The camera's pose matrix
   //! @note param world is a 3D world coordinates with z = 0
   TMatx Pose(const std::vector<Point2d>& world,
             const std::vector<Point2d>& location,
             bool lost = true) const;

   //! @return The camera's projection matrix
   TMatx Projection(double near, double far) const;

   //! @brief Convert from image coordinates to world coordinates
   void Convert(const std::vector<Point2d>& imageCoord, std::vector<Point3d>& worldCoord) const;
   //! @brief Convert from world coordinates to image coordinates
   void Convert(const std::vector<Point3d>& worldCoord, std::vector<Point2d>& imageCoord) const;

private:
   std::vector<double>  distortion;
   Matx<double, 3, 3>   intrinsecs;
   Size_<double>        resolution;
};

}

#endif // AVR_CAMERA_HPP
