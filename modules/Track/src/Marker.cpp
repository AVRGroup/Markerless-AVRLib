#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <avr/track/Algorithms.hpp>
#include <avr/track/Marker.hpp>

namespace avr {

using std::string;

// static
size_t Marker::counter = 0;

Marker::Marker(const Size2i& size, const Coords2D& keys, const cv::Mat descs, const SPtr<Model>& model)
: id(counter++), lost(true), descs(descs), keys(keys), world(Coords2D(4)), model(model) {
   this->world[0] = cv::Point2f(0.0, 0.0);
   this->world[1] = cv::Point2f(size.width, 0.0);
   this->world[2] = cv::Point2f(size.width, size.height);
   this->world[3] = cv::Point2f(0.0, size.height);
}

Size2i Marker::GetSize() const {
   return Size2i(this->world[2].x, this->world[2].y);
}

/*
Marker MarkerSetup::GetMarker(const SystemAlgorithms& methods) const {
   Mat image = cv::imread(this->path, cv::IMREAD_GRAYSCALE);
   if(image.empty()) {
      AVR_ERROR(Cod::Undefined, "It did not read the image file to build the marker");
   }
   vector<cv::KeyPoint> keys; Mat descs;
   methods.Detect(image, keys);
   methods.Extract(image, keys, descs);

   vector<Point2f> points;
   cv::KeyPoint::convert(keys, points);

   return Marker(points, descs, image.size(), this->draw);
}
*/

} // namespace avr
