#ifndef AVR_TRANSFORMATION_MATRIX_HPP
#define AVR_TRANSFORMATION_MATRIX_HPP

#ifdef __cplusplus

#include <vector>
#include <ostream>

#include "Point.hpp"

namespace cv {
template <typename _Tp, int m, int n> class Matx;
template <typename _Tp, int cn> class Vec;

typedef Matx<double, 3, 3> Matx33d;
typedef Vec<double, 3>     Vec3d;
}

namespace avr {

typedef class TransformationMatrix TMat;

class Quaternion;

class TransformationMatrix {
   public:
      TransformationMatrix(const TransformationMatrix&);
      TransformationMatrix();
      ~TransformationMatrix();

      Quaternion& GetQuaternion() const;
      Point3d&    GetPosition() const;
      void        GetData(double [16]) const;

      double   Determinant() const;
      TMat&    Transpose() const;
      TMat&    Inverse() const;

      TMat&    operator = (const TMat&);
      TMat&    operator += (const TMat&);
      TMat&    operator -= (const TMat&);
      TMat&    operator *= (const TMat&);
      TMat&    operator *= (double scalar);

      TMat&    operator + (const TMat&) const;
      TMat&    operator - (const TMat&) const;
      TMat&    operator * (const TMat&) const;
      TMat&    operator * (double scalar) const;
      Point3d& operator * (const Point3d&) const;

      inline double& At(uint8_t row, uint8_t col) { return this->data[row + 4 * col]; }
      inline const double& At(uint8_t row, uint8_t col) const  { return this->data[row + 4 * col]; }

      //! Access a particular matrix cell
      inline double& operator () (uint8_t row, uint8_t col) { return this->At(row, col); }
      inline const double& operator () (uint8_t row, uint8_t col) const { return this->At(row, col); }

      friend std::ostream& operator << (std::ostream& out, const TMat& m){
         for(unsigned i = 0; i < 4; i++){
            for(unsigned j = 0; j < 4; j++)
               out << m(i, j) << " ";
            out << std::endl;
         }
         out << std::endl;
         return out;
      }

      friend class Camera;
      friend class Quaternion;

   private:
      double   data[16];

      TransformationMatrix(std::vector<double>& rotation, std::vector<double>& translation);
      TransformationMatrix(cv::Matx33d& rotation, cv::Vec3d& translation);

      double& operator [] (uint8_t);
      const double& operator [] (uint8_t) const;
};

} // namespace avr

#endif // __cplusplus
#endif // AVR_TRANSFORMATION_MATRIX_HPP
