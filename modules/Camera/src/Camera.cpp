#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <avr/camera/Camera.hpp>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace avr;

const unsigned IT_MAX = 72;

Camera::Camera(const std::string& filename) : intrinsecs(), resolution(Size2d(0.0, 0.0)) {
   cv::FileStorage reader(filename, cv::FileStorage::READ);
   cv::Mat cameraMatrix, distCoeffs;

   reader["image_width"] >> this->resolution.width;
   reader["image_height"] >> this->resolution.height;
   reader["camera_matrix"] >> cameraMatrix;
   reader["distortion_coefficients"] >> distCoeffs;

   cameraMatrix.copyTo(this->intrinsecs);
   distCoeffs.copyTo(this->distortion);
}

TMatx Camera::Pose(const std::vector<Point3d>& world, const std::vector<Point2d>& location, bool lost) const {
   static bool first = true;

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

TMatx Camera::Pose(const std::vector<Point2d>& world, const std::vector<Point2d>& location, bool lost) const {
   std::vector<Point3d> _world(world.begin(), world.end());
   return this->Pose(_world, location, lost);
}

TMatx Camera::Projection(double near, double far) const {
   const double& width = resolution.width;
   const double& height = resolution.height;

   const double& fx = intrinsecs(0, 0);
   const double& fy = intrinsecs(1, 1);
   const double& cx = intrinsecs(0, 2);
   const double& cy = intrinsecs(1, 2);

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

void Camera::Convert(const std::vector<Point2d>& imageCoord, std::vector<Point3d>& worldCoord) const {
   Point2d c = Point2d(resolution.width/2.0, resolution.height/2.0);
   Point2d f = Point2d(intrinsecs(0, 0), intrinsecs(1, 1));

   worldCoord.clear();
   for(auto pi : imageCoord)
      worldCoord.push_back(Point3d( (pi.x - c.x) / f.x, (pi.y - c.y) / f.y, 1.0 ));
}
void Camera::Convert(const std::vector<Point3d>& worldCoord, std::vector<Point2d>& imageCoord) const {
   Point2d c = Point2d(resolution.width/2.0, resolution.height/2.0);
   Point2d f = Point2d(intrinsecs(0, 0), intrinsecs(1, 1));

   imageCoord.clear();
   for(auto pw : worldCoord)
      imageCoord.push_back(Point2d( f.x * pw.x + c.x, f.y * pw.y + c.y ));
}
