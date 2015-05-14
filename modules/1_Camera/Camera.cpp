#include "Camera.hpp"
#include "TransformationMatrix.hpp"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>
#include <stdexcept>

#define cast(Tp) reinterpret_cast<const std::vector<Tp>& >

using namespace std;
using namespace avr;

const unsigned IT_MAX = 72;

class Camera::IntrinsicParams : public cv::Matx <double, 3, 3> {
public:
   std::vector<double> distortion;

   IntrinsicParams() : cv::Matx<double, 3, 3>() {}
   ~IntrinsicParams() {}
};

Camera::Camera(const std::string& filename) : width(0.0), height(0.0), cameraParams(* new Camera::IntrinsicParams()) {
   cv::FileStorage reader(filename, cv::FileStorage::READ);
   cv::Mat cameraMatrix, distCoeffs;

   reader["image_width"] >> this->width;
   reader["image_height"] >> this->height;
   reader["camera_matrix"] >> cameraMatrix;
   reader["distortion_coefficients"] >> distCoeffs;

   cameraMatrix.copyTo(this->cameraParams);
   distCoeffs.copyTo(this->cameraParams.distortion);
}

Camera::~Camera(){
   // dtor
}

void Camera::Pose(const std::vector<Point2f>& world, const std::vector<Point2f>& location, TransformationMatrix& pose){
   std::vector<Point3f> _world(world.begin(), world.end());
   this->Pose(_world, location, pose);
}

void Camera::Pose(const std::vector<Point3f>& world, const std::vector<Point2f>& location, TransformationMatrix& pose){
   static bool first = true;

   cv::Mat _rotation;
   cv::Mat _translat;
   // temp
   bool lost_mode = true;

   if(lost_mode){
      cv::solvePnPRansac(cast(cv::Point3f)(world), cast(cv::Point2f)(location), this->cameraParams, this->cameraParams.distortion, _rotation, _translat, !first, IT_MAX, 8.0, 100, cv::noArray(), CV_EPNP);
   } else { // tracking
      cv::solvePnP(cast(cv::Point3f)(world), cast(cv::Point2f)(location), this->cameraParams, this->cameraParams.distortion, _rotation, _translat, false, CV_ITERATIVE);
   }
   first = false;

   std::vector<double> rotation;
   std::vector<double> translat;

   _rotation.copyTo(rotation);
   _translat.copyTo(translat);

   pose = TransformationMatrix(rotation, translat);
}

TransformationMatrix& Camera::GetProjectionMatrix(){
   const double near = 0.05;
   const double far  = 1000;
   bool invert = false;

   TransformationMatrix& projection = * new TransformationMatrix();
   projection(0, 0) = 2.0 * this->cameraParams(0, 0) / this->width;
   projection(0, 2) = -((2.0 * this->cameraParams(0, 2) / this->width) - 1);
   projection(1, 1) = -(2.0 * this->cameraParams(1, 1) / this->height);
   projection(1, 2) = (2.0 * this->cameraParams(1, 2) / this->height) - 1;
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
