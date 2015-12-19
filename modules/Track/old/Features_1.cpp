#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>    // SIFT and SURF

#include "Features.hpp"

#ifndef NULL
   #define NULL 0x0
#endif // NULL

using namespace std;

namespace avr {

static cv::FeatureDetector* GetDetector(int flag);
static cv::DescriptorExtractor* GetExtractor(int flag);
static cv::DescriptorMatcher* GetMatcher(int flag);

int Features::flag = 0;

Features::Features() { }
Features::Features(int flag) { Features::flag = flag; }
Features::Features(OptFlag flag) { this->OptimizeFor(flag); }

void Features::Match(const Features& query, const Features& train, vector<Point2f>& queryPoints, vector<Point2f>& trainPoints) {
   cv::DescriptorMatcher* matcher = GetMatcher(Features::flag);

   std::vector<std::vector<cv::DMatch> > matches;
   matcher->knnMatch(query.descriptors, train.descriptors, matches, 2);

   std::vector<cv::DMatch> goods;
   for(unsigned i = 0; i < matches.size(); i++)
      if(matches[i][0].distance < 0.7 * matches[i][1].distance)
         goods.push_back(matches[i][0]);

   for(unsigned i = 0; i < goods.size(); i++ ){
      //Get the keypoints from the good matches
      queryPoints.push_back(query.keypoints[goods[i].queryIdx].pt);
      trainPoints.push_back(train.keypoints[goods[i].trainIdx].pt);
   }
}

Features Features::Extract(const Mat& image) {
   Features out;

   cv::FeatureDetector* detector = GetDetector(Features::flag);
   detector->detect(image, out.keypoints);

   cv::DescriptorExtractor* extractor = GetExtractor(Features::flag);
   extractor->compute(image, out.keypoints, out.descriptors);

   return out;
}

void Features::OptimizeFor(OptFlag flag) {
   switch(flag) {
   case OPT_PERFORMANCE:
      Features::flag = DSTAR | ESURF | MBFL1;
      break;

   case OPT_QUALITY:
      Features::flag = DSIFT | ESIFT | MFLANN;
      break;

   case OPT_BALANCING:
      Features::flag = DSIFT | EBRISK | MBFHAMM;
      break;
   }
}

} // namespace avr

cv::FeatureDetector* avr::GetDetector(int flag) {
   if(flag & DBRISK)       return new cv::BRISK();
   else if(flag & DFAST)   return new cv::FastFeatureDetector();
   else if(flag & DORB)    return new cv::ORB(50000);
   else if(flag & DSIFT)   return new cv::SiftFeatureDetector();
   else if(flag & DSURF)   return new cv::SurfFeatureDetector();
   return NULL;
}

cv::DescriptorExtractor* avr::GetExtractor(int flag) {
   if(flag & EBRIEF)       return new cv::BriefDescriptorExtractor();
   else if(flag & EBRISK)  return new cv::BRISK();
   else if(flag & EFREAK)  return new cv::FREAK();
   else if(flag & EORB)    return new cv::OrbDescriptorExtractor();
   else if(flag & ESIFT)   return new cv::SiftDescriptorExtractor();
   else if(flag & ESURF)   return new cv::SurfDescriptorExtractor();
   return NULL;
}

cv::DescriptorMatcher* avr::GetMatcher(int flag) {
   if(flag & MBFL1)        return new cv::BFMatcher(cv::NORM_L1);
   else if(flag & MBFL2)   return new cv::BFMatcher(cv::NORM_L2);
   else if(flag & MBFHAMM) return new cv::BFMatcher(cv::NORM_HAMMING);
   else if(flag & MFLANN)  return new cv::FlannBasedMatcher();
   return NULL;
}
