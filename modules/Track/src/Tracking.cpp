#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <avr/track/Tracking.hpp>

#define WINDOWS_BORDER_SIZE   50

namespace avr {

Marker HybridTracker::Registry(const PreMarker& mk) const {
   Mat image = cv::imread(mk.path, cv::IMREAD_GRAYSCALE);
   if(image.empty()) {
      AVR_ERROR(Cod::Undefined, "It did not read the image file to build the marker");
   }
   vector<cv::KeyPoint> keys; Mat descs;
   methods.Detect(image, keys);
   methods.Extract(image, keys, descs);

   vector<Point2f> points;
   cv::KeyPoint::convert(keys, points);

   return Marker(image.size(), points, descs, mk.model);
}

bool HybridTracker::Update(Frame& frame) {
   if(!frame.image.empty() && this->oneLost) {
      vector<cv::KeyPoint> keys;
      methods.Detect(frame.image, keys);
      methods.Extract(frame.image, keys, frame.descriptor);
      cv::KeyPoint::convert(keys, frame.keys);

      this->oneLost = false;
      return true;
   }
   return false;
}

Matches HybridTracker::Find(const Marker& target, const Frame& scene) {
   bool found = (target.lost) ? this->Localize(target, scene, target.lastMatches)
                              : this->Track(target, scene, target.lastMatches);

   this->oneLost = !found;

   // backup and exit
   scene.image.copyTo(this->prevScene);
   return target.lastMatches;
}

bool HybridTracker::Localize(const Marker& target, const Frame& scene, Matches& out) {
   vector<cv::DMatch> matches;
   methods.Match(target.descriptor, scene.descriptor, matches);

   out.clear();
   for(auto& it : matches) {
      out._targetPts.push_back(target.keys[it.queryIdx]);
      out._scenePts.push_back(scene.keys[it.trainIdx]);
      out._error.push_back(it.distance);
   }

   return matches.size() > 20;
}

bool HybridTracker::Track(const Marker& target, const Frame& scene, Matches& inout) {
   vector<Point2f> currPoints; vector<float> error;
   this->methods.Track(this->prevScene, inout._scenePts, scene.image, currPoints, error);

   // Filtra os pontos que foram rastreados pelo status
   size_t k = 0;
   for(size_t i = 0; i < error.size(); i++) {
      if(0.0f <= error[i]) {
         inout._targetPts[k] = inout._targetPts[i];
         inout._scenePts[k] = currPoints[i];
         inout._error[k++] = error[i];
      }
   }
   inout._scenePts.resize(k);
   inout._targetPts.resize(k);
   inout._error.resize(k);

   return k > 20;
}

} // namespace avr
