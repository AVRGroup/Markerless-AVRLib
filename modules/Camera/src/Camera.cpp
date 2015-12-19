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

Camera::~Camera(){
   // dtor
}

void Camera::Pose(const std::vector<Point2f>& world, const std::vector<Point2f>& location, TMatx& pose){
   std::vector<Point3f> _world(world.begin(), world.end());
   this->Pose(_world, location, pose);
}

void Camera::Pose(const std::vector<Point3f>& world, const std::vector<Point2f>& location, TMatx& pose){
   static bool first = true;

   cv::Vec3d   rotVec;
   cv::Vec3d   translat;
   // temp
   bool lost_mode = true;

   if(lost_mode){
      cv::solvePnPRansac(world, location, this->intrinsecs, this->distortion, rotVec, translat, !first, IT_MAX, 8.0, 100, cv::noArray(), CV_EPNP);
   } else { // tracking
      cv::solvePnP(world, location, this->intrinsecs, this->distortion, rotVec, translat, false, CV_ITERATIVE);
   }
   first = false;

   cv::Matx33d rotation;
   cv::Rodrigues(rotVec, rotation);
   pose = TMatx(rotation, translat);

   //   std::vector<double> rotation;
   //   std::vector<double> translat;
   //
   //   _rotation.copyTo(rotation);
   //   _translat.copyTo(translat);
   //
   //   pose = TMatx(rotation, translat);

}

TMatx Camera::GetProjection(){
   const double near = 0.05;
   const double far  = 1000;
   bool invert = false;

   TMatx projection;
   projection(0, 0) = 2.0 * this->intrinsecs(0, 0) / this->resolution.width;
   projection(0, 2) = -((2.0 * this->intrinsecs(0, 2) / this->resolution.width) - 1);
   projection(1, 1) = -(2.0 * this->intrinsecs(1, 1) / this->resolution.height);
   projection(1, 2) = (2.0 * this->intrinsecs(1, 2) / this->resolution.height) - 1;
   projection(2, 2) = -(far + near)/(far - near);
   projection(2, 3) = -2.0 * (far * near)/(far - near);
   projection(3, 2) = -1.0;
//   projection(3, 3) = 0.0; // ??

   if (invert) {
      projection(1, 1) = - projection(1, 1);
      projection(1, 2) = - projection(1, 2);
//      projection(1, 3) = - projection(1, 3); // = 0
//      projection(1, 0) = - projection(1, 0); // = 0
   }

   return projection;
}
