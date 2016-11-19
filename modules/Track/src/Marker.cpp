#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <avr/track/Algorithms.hpp>
#include <avr/track/Marker.hpp>

namespace avr {

using std::string;

// static
size_t Marker::counter = 0;

Marker::Marker(const Size2i& size, const Coords2D& keys, const cv::Mat& descs, const SPtr<Model>& model)
: id(counter++), lost(true), world(Coords2D(4)), keys(keys), descriptor(descs), model(model) {
   this->world[0] = cv::Point2f(0.0, 0.0);
   this->world[1] = cv::Point2f(size.width, 0.0);
   this->world[2] = cv::Point2f(size.width, size.height);
   this->world[3] = cv::Point2f(0.0, size.height);
   this->model->Scale(0.60 * size.width);
}

Size2i Marker::GetSize() const {
   return Size2i(this->world[2].x, this->world[2].y);
}

} // namespace avr
