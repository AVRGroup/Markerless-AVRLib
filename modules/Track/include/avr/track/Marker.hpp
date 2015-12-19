#ifndef AVR_MARKER_HPP
#define AVR_MARKER_HPP

#include <avr/core/Core.hpp>
#include "feature_structures.hpp"

namespace avr {

using std::vector;

class Marker {
public:
   Marker();
   virtual ~Marker() = 0;

   inline const TMatx& GetPose() const { return this->pose; }
   inline ubyte GetID() const { return this->id; }
   inline bool IsLost() const { return this->lost; }

protected:
   inline void SetPose(const TMatx& _pose) { this->pose = _pose; }
   inline void SetLost(bool _lost) { this->lost = _lost; }

   bool lost;
   TMatx pose;

private:
   ubyte        id;
   static ubyte counter;
};

class NaturalMarker : public Marker {
public:
   explicit NaturalMarker(const std::string& imagePath);

   inline const Size2i& GetSize() const { return this->size; }
   inline const BRect<int>& GetRegion() const { return this->region; }

   friend class FeatureTracker;
   friend class MotionTracker;

private:
   inline void Get(avr::Mat& out) const { out = this->descriptors; }
   inline void Get(vector<cv::KeyPoint>& out) const { out = this->keypoints; }
   inline void SetRegion(const BRect<int>& _region) { this->region = _region; }

   vector<cv::KeyPoint> keypoints;
   avr::Mat descriptors;
   BRect<int> region;
   Size2i size;
};

struct Frame {
   vector<cv::KeyPoint> keypoints;
   avr::Mat descriptors;
   avr::Mat image;

   Frame() {/* ctor */} //< TODO
   Frame(const Mat& _image) : image(_image) {/* ctor */}
};

} // namespace avr

#endif // AVR_MARKER_HPP
