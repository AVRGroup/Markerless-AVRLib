#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <avr/track/Marker.hpp>

namespace avr {

using std::string;

// static
ubyte Marker::counter = 0;

Marker::Marker() : lost(true), id(counter++) {/* ctor */}
Marker::~Marker() {/* dtor */}

NaturalMarker::NaturalMarker(const string& path) : Marker() {
   cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
   this->size = avr::Size2i(image.cols, image.rows);

   cv::SiftFeatureDetector detector;
   detector.detect(image, this->keypoints);

   cv::SiftDescriptorExtractor extractor;
   extractor.compute(image, this->keypoints, this->descriptors);
}

} // namespace avr
