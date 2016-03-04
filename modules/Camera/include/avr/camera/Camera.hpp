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
   TMatx Pose(const std::vector<Point3f>& world,
             const std::vector<Point2f>& location,
             bool lost = true) const;

   //! @return The camera's pose matrix
   //! @note param world is a 3D world coordinates with z = 0
   TMatx Pose(const std::vector<Point2f>& world,
             const std::vector<Point2f>& location,
             bool lost = true) const;

   //! @return The camera's projection matrix
   TMatx Projection(float near, float far) const;

   //! @brief Convert from image coordinates to world coordinates
   void Convert(const std::vector<Point2f>& imageCoord, std::vector<Point3f>& worldCoord) const;
   //! @brief Convert from world coordinates to image coordinates
   void Convert(const std::vector<Point3f>& worldCoord, std::vector<Point2f>& imageCoord) const;

private:
   std::vector<float>  distortion;
   Matx<float, 3, 3>   intrinsecs;
   Size_<float>        resolution;
};

}

#endif // AVR_CAMERA_HPP
