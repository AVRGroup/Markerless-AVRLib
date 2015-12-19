#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>

#include "TransformationMatrix.hpp"
#include "Quaternion.hpp"
#include "Point.hpp"

using namespace std;
using namespace avr;

TransformationMatrix::TransformationMatrix(cv::Matx33d& rotation, cv::Vec3d& translation) {
   cv::Matx44d trans = cv::Matx44d::eye();
   for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++)
         trans(i, j) = rotation(i, j);
      trans(i, 3) = translation(i);
   }

   cv::Matx44d revyz = cv::Matx44d::eye();
   revyz(1, 1) = revyz(2, 2) = -1;
   trans = revyz * trans;

   for(int j = 0; j < 4; j++)
      for(int i = 0; i < 4; i++)
         this->At(i, j) = trans(i, j);
}

TransformationMatrix::TransformationMatrix(std::vector<double>& _rotation, std::vector<double>& _translation) {
   cv::Mat translation(_translation, true), rotationVector;
   Rodrigues(_rotation, rotationVector);

   cv::Mat transf = cv::Mat::zeros(4, 4, CV_64FC1);
   for(int y = 0; y < 3; y++) {
      for(int x = 0; x < 3; x++)
         transf.at<double>(y, x) = rotationVector.at<double>(y, x);
      transf.at<double>(y, 3) = translation.ptr<double>(0)[y];
   }
   transf.at<double>(3, 3) = 1.0;

   //OpenGL has reversed Y & Z coords
   cv::Mat reverseYZ = cv::Mat::eye(4, 4, CV_64FC1);
   reverseYZ.at<double>(1, 1) = reverseYZ.at<double>(2, 2) = -1;
   transf = reverseYZ * transf;

   for(int j = 0; j < 4; j++)
      for(int i = 0; i < 4; i++)
         this->At(i, j) = transf.at<double>(i, j);
}

TransformationMatrix::TransformationMatrix(const TransformationMatrix& matx) {
   matx.GetData(this->data);
}

TransformationMatrix::TransformationMatrix() {
   // set identity matrix
   for(unsigned i = 0; i < 16; i++)
      this->data[i] = 0.0 + (i % 5 == 0);
}

TransformationMatrix::~TransformationMatrix() {
   // dtor
}

void TransformationMatrix::GetData(double data[16]) const {
   for(unsigned i = 0; i < 16; i++)
      data[i] = this->data[i];
}

Quaternion& TransformationMatrix::GetQuaternion() const {
   Quaternion& quat = * new Quaternion();
   quat.w = this->At(0, 0) + this->At(1, 1) + this->At(2, 2) + 1.0;
   //if( quat.w < 0.0 ) return -1;

   quat.w = sqrt( quat.w );
   quat.qx = (this->At(1, 2) - this->At(2, 1)) / (2.0 * quat.w);
   quat.qy = (this->At(2, 0) - this->At(0, 2)) / (2.0 * quat.w);
   quat.qz = (this->At(0, 1) - this->At(1, 0)) / (2.0 * quat.w);
   quat.w = quat.w / 2.0;

   return quat;
}
Point3d& TransformationMatrix::GetPosition() const {
   double x = this->At(0, 3);
   double y = this->At(1, 3);
   double z = this->At(2, 3);

   return * new Point3d(x, y, z);
}

double TransformationMatrix::Determinant() const {
   cv::Mat matx = cv::Mat(4, 4, CV_64F, (void*) this->data, 4 * sizeof(double));
   return cv::determinant(matx);
}
TransformationMatrix& TransformationMatrix::Inverse() const {
   cv::Mat inv = cv::Mat(4, 4, CV_64F, (void*) this->data, 4 * sizeof(double));
   inv = inv.inv(cv::DECOMP_LU);

   TransformationMatrix& ret = * new TransformationMatrix();
   for(unsigned j = 0; j < 4; j++)
      for(unsigned i = 0; i < 4; i++)
         ret.At(i, j) = inv.at<double>(i, j);

   return ret;
}
TransformationMatrix& TransformationMatrix::Transpose() const {
   TransformationMatrix& ret = * new TransformationMatrix(*this);
   this->GetData(ret.data);

   for(unsigned i = 0; i < 4; i++)
      for(unsigned j = i + 1; j < 4; j++)
         std::swap(ret.At(i, j), ret.At(j, i));

   return ret;
}

TransformationMatrix& TransformationMatrix::operator = (const TransformationMatrix& matx) {
   matx.GetData(this->data);
   return * this;
}
TransformationMatrix& TransformationMatrix::operator += (const TransformationMatrix& matx) {
   for(unsigned i = 0; i < 16; i++)
      this->data[i] += matx[i];

   return * this;
}
TransformationMatrix& TransformationMatrix::operator -= (const TransformationMatrix& matx) {
   for(unsigned i = 0; i < 16; i++)
      this->data[i] -= matx[i];

   return * this;
}
TransformationMatrix& TransformationMatrix::operator *= (const TransformationMatrix& matx) {
   double sum[16];
   for(unsigned i = 0; i < 4; i++){
      for(unsigned j = 0; j < 4; j++){
         sum[i + j * 4] = 0.0;
         for(unsigned k = 0; k < 4; k++)
            sum[i + j * 4] += this->At(i, k) * matx(k, j);
      }
   }

   for(unsigned i = 0; i < 16; i++)
      this->data[i] = sum[i];

   return * this;
}
TransformationMatrix& TransformationMatrix::operator *= (double scalar) {
   for(unsigned i = 0; i < 16; i++)
      this->data[i] *= scalar;

   return * this;
}

TransformationMatrix& TransformationMatrix::operator + (const TransformationMatrix& matx) const {
   TransformationMatrix& ret = * new TransformationMatrix();
   for(unsigned i = 0; i < 16; i++)
      ret[i] = this->data[i] + matx[i];

   return ret;
}
TransformationMatrix& TransformationMatrix::operator - (const TransformationMatrix& matx) const {
   TransformationMatrix& ret = * new TransformationMatrix();
   for(unsigned i = 0; i < 16; i++)
      ret[i] = this->data[i] - matx[i];

   return ret;
}
TransformationMatrix& TransformationMatrix::operator * (const TransformationMatrix& matx) const {
   TransformationMatrix& ret = * new TransformationMatrix();
   for(unsigned i = 0; i < 4; i++){
      for(unsigned j = 0; j < 4; j++){
         ret(i, j) = 0.0;
         for(unsigned k = 0; k < 4; k++)
            ret(i, j) += this->At(i, k) * matx(k, j);
      }
   }

   return ret;
}
Point3d& TransformationMatrix::operator * (const Point3d& p) const {
   Point3d& ret = * new Point3d();
   // this * [x, y, z]^(-1)
   ret.x = this->At(0, 0) * p.x + this->At(0, 1) * p.y + this->At(0, 2) * p.z + this->At(0, 3);
   ret.y = this->At(1, 0) * p.x + this->At(1, 1) * p.y + this->At(1, 2) * p.z + this->At(1, 3);
   ret.z = this->At(2, 0) * p.x + this->At(2, 1) * p.y + this->At(2, 2) * p.z + this->At(2, 3);
   return ret;
}
TransformationMatrix& TransformationMatrix::operator * (double scalar) const {
   TransformationMatrix& ret = * new TransformationMatrix();
   for(unsigned i = 0; i < 16; i++)
      ret[i] = this->data[i] * scalar;

   return ret;
}

double& TransformationMatrix::operator [] (uint8_t index) {
   return this->data[index];
}
const double& TransformationMatrix::operator [] (uint8_t index) const {
   return this->data[index];
}
