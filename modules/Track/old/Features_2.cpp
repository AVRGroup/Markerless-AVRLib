#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>    // SIFT and SURF

#include "Features.hpp"

#ifndef NULL
   #define NULL 0x0
#endif // NULL

using namespace std;

namespace avr {

static cv::FeatureDetector* GetDetector(int flag) {
   if(flag & DBRISK)       return new cv::BRISK();
   else if(flag & DFAST)   return new cv::FastFeatureDetector();
   else if(flag & DORB)    return new cv::ORB(50000);
   else if(flag & DSIFT)   return new cv::SiftFeatureDetector();
   else if(flag & DSURF)   return new cv::SurfFeatureDetector();
   return NULL;
}

static cv::DescriptorExtractor* GetExtractor(int flag) {
   if(flag & EBRIEF)       return new cv::BriefDescriptorExtractor();
   else if(flag & EBRISK)  return new cv::BRISK();
   else if(flag & EFREAK)  return new cv::FREAK();
   else if(flag & EORB)    return new cv::OrbDescriptorExtractor();
   else if(flag & ESIFT)   return new cv::SiftDescriptorExtractor();
   else if(flag & ESURF)   return new cv::SurfDescriptorExtractor();
   return NULL;
}

static cv::DescriptorMatcher* GetMatcher(int flag) {
   if(flag & MBFL1)        return new cv::BFMatcher(cv::NORM_L1);
   else if(flag & MBFL2)   return new cv::BFMatcher(cv::NORM_L2);
   else if(flag & MBFHAMM) return new cv::BFMatcher(cv::NORM_HAMMING);
   else if(flag & MFLANN)  return new cv::FlannBasedMatcher();
   return NULL;
}

Features::Features() : flag(0) { }
Features::Features(int flag) : flag(flag) { }
Features::Features(OptFlag flag) : flag(0) { this->OptimizeFor(flag); }

void Features::Detect(const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::FeatureDetector* detector = GetDetector(this->flag);
   detector->detect(image, keys);
}

void Features::Extract(const Mat& image, vector<cv::KeyPoint>& keys, Mat& descriptors) const {
   cv::DescriptorExtractor* extractor = GetExtractor(this->flag);
   extractor->compute(image, keys, descriptors);
}

void Features::Match(const Mat& descTarget, const Mat& descScene, vector<Point2f>& target, vector<Point2f> scene) const {
   cv::DescriptorMatcher* matcher = GetMatcher(this->flag);

   std::vector<std::vector<cv::DMatch> > all_matches;
   matcher->knnMatch(descTarget, descScene, all_matches, 2);

   std::vector<cv::DMatch> goods;
   for(unsigned i = 0; i < all_matches.size(); i++)
      if(all_matches[i][0].distance < 0.7 * all_matches[i][1].distance)
         goods.push_back(all_matches[i][0]);

//   for(unsigned i = 0; i < goods.size(); i++ ){
//      //Get the keypoints from the good matches
//      target.push_back(imageKeys[goods[i].queryIdx].pt);
//      scene.push_back(frameKeys[goods[i].trainIdx].pt);
//   }
}

void Features::OptimizeFor(OptFlag flag) {
   switch(flag) {
   case OPT_PERFORMANCE:
      this->flag = DSTAR | ESURF | MBFL1;
      break;

   case OPT_QUALITY:
      this->flag = DSIFT | ESIFT | MFLANN;
      break;

   case OPT_BALANCING:
      this->flag = DSIFT | EBRISK | MBFHAMM;
      break;
   }
}

} // namespace avr

