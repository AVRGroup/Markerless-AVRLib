#ifndef AVR_MARKER_HPP
#define AVR_MARKER_HPP

#include <avr/core/Core.hpp>
#include <avr/model/Model.hpp>

namespace avr {

using std::vector;
typedef vector<Point2f> Coords2D;
typedef vector<Point3f> Coords3D;

struct Frame {
   Mat image;
   Mat descriptor;
   Coords2D keys;

   Frame(const Mat& img, const Mat& descs, const Coords2D& pnts) :
      image(img), descriptor(descs), keys(pnts) {/* ctor */}

   Frame() {/* default */}

   Frame& operator = (const Frame& frm) {
      image = frm.image;
      descriptor = frm.descriptor;
      keys.assign(frm.keys.begin(), frm.keys.end());
      return * this;
   }
};

struct Matches {
   Matches(const Coords2D& target, const Coords2D& scene, const vector<float>& error) :
      _targetPts(target), _scenePts(scene), _error(error) {/* ctor */}

   Matches() {/* default */}

   const Coords2D& targetPts() const { return this->_targetPts; }
   const Coords2D& scenePts() const { return this->_scenePts; }
   const vector<float>& error() const { return this->_error; }

   const Point2f& targetPts(size_t i) const { return this->_targetPts[i]; }
   const Point2f& scenePts(size_t i) const { return this->_scenePts[i]; }
   const float& error(size_t i) const { return this->_error[i]; }

   const size_t size() const { return this->_scenePts.size(); }

   Matches& operator = (const Matches& mtc) {
      _targetPts.assign(mtc._targetPts.begin(), mtc._targetPts.end());
      _scenePts.assign(mtc._scenePts.begin(), mtc._scenePts.end());
      _error.assign(mtc._error.begin(), mtc._error.end());
      return * this;
   }

   void clear() {
      _targetPts.clear();
      _scenePts.clear();
      _error.clear();
   }

private:
   Coords2D _targetPts;
   Coords2D _scenePts;
   vector<float> _error;

   friend class HybridTracker;
};

struct PreMarker {
   PreMarker(const std::string& path, const SPtr<Model>& model) : path(path), model(model) {/* ctor */}

   std::string path;
   SPtr<Model> model;
};

class Marker {
public:
   virtual ~Marker() {/* dtor */};

   Size2i GetSize() const;

   const Coords2D& GetWorld() const { return this->world; }
   SPtr<Model> GetModel() const     { return this->model; }
   size_t GetID() const             { return this->id; }
   bool Lost() const                { return this->lost; }

   void SetLost(bool lost) { this->lost = lost; }

private:
   Marker(const Size2i&, const Coords2D&, const cv::Mat&, const SPtr<Model>&);

private:
   size_t id;
   bool lost;
   Coords2D world;
   Coords2D keys;
   cv::Mat descriptor;

   mutable Matches lastMatches;
   mutable SPtr<Model> model;

   static size_t counter;

   friend class HybridTracker;
};

} // namespace avr

#endif // AVR_MARKER_HPP
