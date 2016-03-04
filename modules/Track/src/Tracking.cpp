#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <avr/track/Tracking.hpp>

#define WINDOWS_BORDER_SIZE   50

namespace avr {

SPtr<Marker> TrackingSystem::Unpack(const PreMarker& mk) const {
   Mat image = cv::imread(mk.path, cv::IMREAD_GRAYSCALE);
   if(image.empty()) {
      AVR_ERROR(Cod::Undefined, "It did not read the image file to build the marker");
   }
   vector<cv::KeyPoint> keys; Mat descs;
   methods.Detect(image, keys);
   methods.Extract(image, keys, descs);

   vector<Point2f> points;
   cv::KeyPoint::convert(keys, points);

   return new Marker(image.size(), points, descs, mk.model);
}

/**-----------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                        FeatureTracker                                                                             *
\**-----------------------------------------------------------------------------------------------------------------------------------------------**/

TrackResult FeatureTracker::Track(const Marker& target, const Mat& scene, float trackedThreshold) {
   cv::Mat sceneDescriptors;
   vector<cv::KeyPoint> sceneKeypoints;
   vector<cv::DMatch> matches;

   methods.Detect(scene, sceneKeypoints);
   methods.Extract(scene, sceneKeypoints, sceneDescriptors);
   methods.Match(this->UnpackDescs(target), sceneDescriptors, matches);

   const Coords2D& targetKeypoints = this->UnpackKeys(target);

   TrackResult result;
   for(auto it : matches) {
      result.targetPoints.push_back(targetKeypoints[it.queryIdx]);
      result.scenePoints.push_back(sceneKeypoints[it.trainIdx].pt);
   }

   //double rate = matches.size() / (double) targetKeypoints.size();

#ifdef DEBUG_
   for(auto p : result.scenePoints) {
      cv::circle(scene, p, 3, cv::Scalar(0, 255, 0), 1);
   }
#endif // DEBUG_

   return result;
}

/**-----------------------------------------------------------------------------------------------------------------------------------------------**\
*                                                         MotionTracker                                                                             *
\**-----------------------------------------------------------------------------------------------------------------------------------------------**/

TrackResult MotionTracker::Track(const Marker& target, const Mat& scene, float trackedThreshold) {
   auto& scenePoints = this->prevResults.scenePoints;
   auto& targetPoints = this->prevResults.targetPoints;

   vector<Point2f> currPoints; vector<float> error;
   this->methods.Track(this->prevScene, scenePoints, scene, currPoints, error);

   // Filtra os pontos que foram rastreados pelo status
   size_t k = 0;
   for(size_t i = 0; i < error.size(); i++) {
      if(0.0f <= error[i]) {
         currPoints[k] = currPoints[i];
         targetPoints[k++] = targetPoints[i];
      }
   }
   currPoints.resize(k);
   targetPoints.resize(k);

   //const Coords2D& targetKeypoints = this->UnpackDescs(target);
   //double rate = targetPoints.size() / (double) targetKeypoints.size();

#ifdef DEBUG_
   for(auto p : scenePoints) {
      cv::circle(scene, p, 3, cv::Scalar(0, 255, 0), 1);
   }
#endif // DEBUG_

   scene.copyTo(this->prevScene);
   scenePoints.clear();
   scenePoints.assign(currPoints.begin(), currPoints.end());

   return this->prevResults;
}

} // namespace avr
