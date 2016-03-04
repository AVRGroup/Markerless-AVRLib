#ifndef AVR_TRACKING_HPP
#define AVR_TRACKING_HPP

#include <avr/core/Core.hpp>

#include "Algorithms.hpp"
#include "Marker.hpp"

namespace avr {

using std::vector;

/**
 * @struct TrackResult Tracking.hpp <avr/track/Tracking.hpp>
 * @brief Structure for store results of the object's tracking
 */
struct TrackResult {
   vector<Point2f> targetPoints; //< target's points that are tracked
   vector<Point2f> scenePoints;  //< correspondent points to targetPoints on the scene

   TrackResult() = default;
   TrackResult(const TrackResult& tr) { tr.copyTo(*this); }

   void copyTo(TrackResult& tr) const {
      tr.scenePoints.clear(); tr.targetPoints.clear();
      tr.scenePoints.assign(this->scenePoints.begin(), this->scenePoints.end());
      tr.targetPoints.assign(this->targetPoints.begin(), this->targetPoints.end());
   }
};

/**
 * @class TrackingSystem Tracking.hpp <avr/track/Tracking.hpp>
 * @brief Base class for natural marker tracking techniques
 */
class TrackingSystem {
public:
   explicit TrackingSystem(const SystemAlgorithms& methods) : methods(methods) {/* ctor */}
   virtual ~TrackingSystem() {/* dtor */}

   /**
    * @brief Performs the tracking of a target object on a scene.
    *
    * @param target Target marker, the object that is sought on the scene
    * @param scene  Scene image, an image that target appears
    * @param trackedThreshold Desirable hit rate
    *
    * @return The tracked target points and its correspondent points on the scene.
    */
   virtual TrackResult Track(const Marker& target, const Mat& scene, float trackedThreshold) = 0;

   SPtr<Marker> Unpack(const PreMarker&) const;

protected:
   const Coords2D& UnpackKeys(const Marker& mk) const {
      return mk.keys;
   }
   const cv::Mat& UnpackDescs(const Marker& mk) const {
      return mk.descs;
   }

   const SystemAlgorithms& methods;
};

/**
 * @class FeatureTracker Tracking.hpp <avr/track/Tracking.hpp>
 * @brief Tracking system that uses the object and scene natural features in order to locate the object in the image
 */
class FeatureTracker : public TrackingSystem {
public:
   explicit FeatureTracker(const SystemAlgorithms& methods) : TrackingSystem(methods) {/* ctor */};

   /**
    * @override
    * @brief Extracts new scene features and matches it with target features.
    */
   TrackResult Track(const Marker& target, const Mat& scene, float trackedThreshold = 0.5);

private:

};

/**
 * @class MotionTracker Tracking.hpp <avr/track/Tracking.hpp>
 * @brief Tracking system that detects the motion of points between two different images of a same scene
 */
class MotionTracker : public TrackingSystem {
public:
   MotionTracker(const Mat& firstScene, const TrackResult& firstResults, const SystemAlgorithms& methods)
      : TrackingSystem(methods) { this->Set(firstScene, firstResults); }

   /**
    * @override
    * @brief Tries to find the correspondent points of a previous scene on a new scene.
    */
   TrackResult Track(const Marker& target, const Mat& scene, float trackedThreshold = 0.5);

   //! Sets a new previous scene and its points to track
   void Set(const Mat& scene, const TrackResult& result) {
      scene.copyTo(this->prevScene);
      result.copyTo(this->prevResults);
   }

private:
   Mat prevScene;
   TrackResult prevResults;
};

} // namespace avr

#endif // AVR_TRACKING_HPP
