#ifndef AVR_TRACKERS_HPP
#define AVR_TRACKERS_HPP

#include <avr/core/Core.hpp>

#include "Features.hpp"
#include "Tracking.hpp"
#include "Marker.hpp"

namespace avr {

using std::vector;

typedef vector<Point2f> Corners;

class TrackingSystem {
public:
   virtual ~TrackingSystem() {/* dtor */}

   virtual void Track(Marker& target, Frame& scene, Corners& out) = 0;

protected:
   static Frame prev;
   static vector<Point2f> fTracked;
   static vector<Point2f> mTracked;
};

//! @enum Optimaze flags
enum OptFlag { OPT_PERFORMANCE, OPT_QUALITY, OPT_BALANCING };

class FeatureTracker : public TrackingSystem {
public:
   FeatureTracker(OptFlag = OPT_BALANCING);
   FeatureTracker(const SPtr<FeatureDetector>&,
                  const SPtr<DescriptorExtractor>&,
                  const SPtr<DescriptorMatcher>&);

   void Track(Marker& target, Frame& scene, Corners& out);

private:
   SPtr<FeatureDetector>     detector;
   SPtr<DescriptorExtractor> extractor;
   SPtr<DescriptorMatcher>   matcher;
};

class MotionTracker : public TrackingSystem {
public:
   MotionTracker(OptFlag = OPT_BALANCING);
   MotionTracker(const SPtr<OpticFlowAlgorithm>&);

   void Track(Marker& target, Frame& scene, Corners& out);

private:
   SPtr<OpticFlowAlgorithm> tracker;
};

} // namespace avr

#endif // AVR_TRACKERS_HPP
