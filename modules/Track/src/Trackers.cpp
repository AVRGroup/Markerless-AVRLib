#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>

#include <avr/track/Trackers.hpp>

#define TH_GOOD_MATCHES       20
#define WINDOWS_BORDER_SIZE   50

avr::Frame avr::TrackingSystem::prev;
std::vector<cv::Point2f> avr::TrackingSystem::fTracked;
std::vector<cv::Point2f> avr::TrackingSystem::mTracked;

namespace avr {

/**-----------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                        FeatureTracker                                                                             *
\**-----------------------------------------------------------------------------------------------------------------------------------------------**/

FeatureTracker::FeatureTracker(OptFlag flag) : TrackingSystem() {
   switch(flag) {
   case avr::OPT_BALANCING:
      this->detector = new SIFTDetector;
      this->extractor = new BRISKExtractor;
      this->matcher = new BruteForceMatcher(cv::NORM_HAMMING);
      break;

   case avr::OPT_PERFORMANCE:
      this->detector = new STARDetector;
      this->extractor = new SURFExtractor;
      this->matcher = new BruteForceMatcher(cv::NORM_L1);
      break;

   case avr::OPT_QUALITY:
      this->detector = new SIFTDetector;
      this->extractor = new SIFTExtractor;
      this->matcher = new BruteForceMatcher(cv::NORM_L2);
      break;
   }
}

FeatureTracker::FeatureTracker(const SPtr<FeatureDetector>& d, const SPtr<DescriptorExtractor>& e, const SPtr<DescriptorMatcher>& m) : TrackingSystem() {
   this->detector = d;
   this->extractor = e;
   this->matcher = m;
}

void FeatureTracker::Track(Marker& _target, Frame& scene, Corners& out) {
   NaturalMarker& target = dynamic_cast<NaturalMarker&>(_target);

   vector<cv::Point2f>& targetPoints = TrackingSystem::mTracked;
   vector<cv::Point2f>& scenePoints = TrackingSystem::fTracked;
   targetPoints.clear(); scenePoints.clear();

   (*this->detector)(scene.image, scene.keypoints);
   (*this->extractor)(scene.image, scene.keypoints, scene.descriptors);

   vector<cv::DMatch> matches;
   (*this->matcher)(target.descriptors, scene.descriptors, matches);

   for(auto it : matches) {
      targetPoints.push_back(target.keypoints[it.queryIdx].pt);
      scenePoints.push_back(scene.keypoints[it.trainIdx].pt);
   }

#ifdef DEBUG_
   cout << "init: " << scenePoints.size() << endl;
#endif // DEBUG_

   // TODO Definir modo do marcador
   if(matches.size() >= TH_GOOD_MATCHES)
      target.SetLost(false);
   else {
      target.SetLost(true);
      if(matches.size() < 4)
         return;
   }

   vector<cv::Point2f> targetCorner(4);
   targetCorner[0] = cv::Point2f(0.0, 0.0);
   targetCorner[3] = cv::Point2f(0.0, target.size.height);
   targetCorner[2] = cv::Point2f(target.size.width, target.size.height);
   targetCorner[1] = cv::Point2f(target.size.width, 0.0);

   Mat H = cv::findHomography(targetPoints, scenePoints, cv::RANSAC, 4);
   cv::perspectiveTransform(targetCorner, out, H);

   for(auto p : scenePoints) {
      cv::circle(scene.image, p, 5, cv::Scalar(0, 0, 255), 1);
   }

   TrackingSystem::prev = scene;
}

/**-----------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                         MotionTracker                                                                             *
\**-----------------------------------------------------------------------------------------------------------------------------------------------**/

MotionTracker::MotionTracker(OptFlag) : TrackingSystem() {
   this->tracker = new LukasKanadeAlgorithm;
}

MotionTracker::MotionTracker(const SPtr<OpticFlowAlgorithm>& _tracker) : TrackingSystem(), tracker(_tracker) {/* ctor */}

void MotionTracker::Track(Marker& _target, Frame& scene, Corners& out) {
   NaturalMarker& target = dynamic_cast<NaturalMarker&>(_target);
   vector<cv::Point2f>& targetPoints = TrackingSystem::mTracked;
   vector<cv::Point2f>& scenePoints = TrackingSystem::fTracked;

   vector<cv::Point2f> currPoints;
   (*this->tracker)(TrackingSystem::prev.image, scenePoints, scene.image, currPoints, targetPoints);

   if(currPoints.size() >= TH_GOOD_MATCHES)
      target.SetLost(false);
   else {
      target.SetLost(true);
      if(currPoints.size() < 4)
         return;
   }

   vector<cv::Point2f> targetCorner(4);
   targetCorner[0] = cv::Point2f(0.0, 0.0);
   targetCorner[1] = cv::Point2f(target.size.width, 0.0);
   targetCorner[2] = cv::Point2f(target.size.width, target.size.height);
   targetCorner[3] = cv::Point2f(0.0, target.size.height);

   Mat H = cv::findHomography(targetPoints, currPoints, cv::RANSAC, 4);
   cv::perspectiveTransform(targetCorner, out, H);

   for(auto p : currPoints) {
      cv::circle(scene.image, p, 3, cv::Scalar(0, 255, 0), 1);
   }

   TrackingSystem::prev = scene;
   scenePoints.clear();
   scenePoints.assign(currPoints.begin(), currPoints.end());
}

} // namespace avr
