#ifndef AVR_FEATURES_HPP
#define AVR_FEATURES_HPP

#include <avr/core/Core.hpp>

// Detectors
#define DBRISK    0x0001
#define DFAST     0x0002
#define DORB      0x0004
#define DSTAR     0x0008
#define DSIFT     0x0010
#define DSURF     0x0020
// Extractors
#define EBRIEF    0x0040
#define EBRISK    0x0080
#define EFREAK    0x0100
#define EORB      0x0200
#define ESIFT     0x0400
#define ESURF     0x0800
// Matchers
#define MBFL1     0x1000
#define MBFL2     0x2000
#define MBFHAMM   0x4000
#define MFLANN    0x8000

#ifndef __OPENCV_FEATURES_2D_HPP__
   namespace cv { class KeyPoint; }
#endif // __OPENCV_FEATURES_2D_HPP__

namespace avr {

using std::vector;

enum OptFlag { OPT_PERFORMANCE, OPT_QUALITY, OPT_BALANCING };

class Features {
public:
   Features();
   Features(int flag);
   Features(OptFlag flag);

   void OptimizeFor(OptFlag flag);

   static void Match(const Features& query, const Features& train,
                     vector<Point2f>& queryPoints, vector<Point2f>& trainPoints);
   static Features Extract(const Mat& image);

private:
   static int flag;
   Mat descriptors;
   vector<cv::KeyPoint> keypoints;
};

} // namespace avr

#endif // AVR_FEATURES_HPP
