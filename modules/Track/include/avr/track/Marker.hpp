#ifndef AVR_MARKER_HPP
#define AVR_MARKER_HPP

#include <functional>
#include <avr/core/Core.hpp>

namespace avr {

using std::vector;

// TEMP
class Model {
public:
   virtual ~Model() {/* dtor */}
   virtual Draw(const TMatx&) = 0;
};

/*
   REMODELAGEM
   - somente SystemTracking como friend de Marker
   - em SystemTracking cria métodos pack e unpack para as propriedades do marker necessárias
*/

typedef vector<Point2f> Coords2D;
typedef vector<Point3f> Coords3D;

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
   Marker(const Size2i&, const Coords2D&, const cv::Mat, const SPtr<Model>&);

private:
   size_t id;
   bool lost;
   cv::Mat descs;
   Coords2D keys;
   Coords2D world;
   SPtr<Model> model;

   static size_t counter;

   friend class TrackingSystem;
};

} // namespace avr

#endif // AVR_MARKER_HPP
