#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <avr/camera/Camera.hpp>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace avr;

const unsigned IT_MAX = 72;

Camera::Camera(const std::string& filename) : intrinsecs(), resolution(Size2f(0.0, 0.0)) {
   cv::FileStorage reader(filename, cv::FileStorage::READ);
   cv::Mat cameraMatrix, distCoeffs;

   reader["image_width"] >> this->resolution.width;
   reader["image_height"] >> this->resolution.height;
   reader["camera_matrix"] >> cameraMatrix;
   reader["distortion_coefficients"] >> distCoeffs;

   cameraMatrix.copyTo(this->intrinsecs);
   distCoeffs.copyTo(this->distortion);
}

TMatx Camera::Pose(const std::vector<Point3f>& world, const std::vector<Point2f>& location, bool lost) const {
   static bool first = true;

   AVR_ASSERT(world.size() == location.size());

   cv::Vec3d   rotVec;
   cv::Vec3d   translat;

   if(lost){
      cv::solvePnPRansac(world, location, this->intrinsecs, this->distortion, rotVec, translat, !first, IT_MAX, 8.0, 4, cv::noArray(), CV_EPNP);
      first = false;
   } else { // tracking
      cv::solvePnP(world, location, this->intrinsecs, this->distortion, rotVec, translat, false, CV_ITERATIVE);
   }

   cv::Matx33d rotation;
   cv::Rodrigues(rotVec, rotation);
   return TMatx(rotation, translat);
}

TMatx Camera::Pose(const std::vector<Point2f>& world, const std::vector<Point2f>& location, bool lost) const {
   std::vector<Point3f> _world(world.begin(), world.end());
   return this->Pose(_world, location, lost);
}

TMatx Camera::Projection(float near, float far) const {
   const float& width = resolution.width;
   const float& height = resolution.height;

   const float& fx = intrinsecs(0, 0);
   const float& fy = intrinsecs(1, 1);
   const float& cx = intrinsecs(0, 2);
   const float& cy = intrinsecs(1, 2);

   TMatx projection;
   projection(0, 0) = 2.0 * fx / width;
   projection(0, 2) = 1.0 - (2.0 * cx / width);
   projection(1, 1) = 2.0 * fy / height;
   projection(1, 2) = 1.0 - (2.0 * cy / height);
   projection(2, 2) = (near - far) / (far - near);
   projection(2, 3) = (-2.0 * far * near) / (far - near);
   projection(3, 2) = -1.0;
   projection(3, 3) = 0.0;

   return projection;
}

void Camera::Convert(const std::vector<Point2f>& imageCoord, std::vector<Point3f>& worldCoord) const {
   Point2f c = Point2f(resolution.width/2.0, resolution.height/2.0);
   Point2f f = Point2f(intrinsecs(0, 0), intrinsecs(1, 1));

   worldCoord.clear();
   for(auto pi : imageCoord)
      worldCoord.push_back(Point3f( (pi.x - c.x) / f.x, (pi.y - c.y) / f.y, 1.0 ));
}
void Camera::Convert(const std::vector<Point3f>& worldCoord, std::vector<Point2f>& imageCoord) const {
   Point2f c = Point2f(resolution.width/2.0, resolution.height/2.0);
   Point2f f = Point2f(intrinsecs(0, 0), intrinsecs(1, 1));

   imageCoord.clear();
   for(auto pw : worldCoord)
      imageCoord.push_back(Point2f( f.x * pw.x + c.x, f.y * pw.y + c.y ));
}
