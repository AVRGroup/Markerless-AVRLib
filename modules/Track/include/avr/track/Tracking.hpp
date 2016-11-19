#ifndef AVR_TRACKING_HPP
#define AVR_TRACKING_HPP

#include <avr/core/Core.hpp>

#include "Algorithms.hpp"
#include "Marker.hpp"

namespace avr {

using std::vector;

class HybridTracker {
public:
   explicit HybridTracker(const SystemAlgorithms& methods) : oneLost(true), methods(methods) {/* ctor */}

   Matches  Find(const Marker&, const Frame&);

   Marker   Registry(const PreMarker&) const;
   bool     Update(Frame& frm);

private:
   // Methods to find marker in scene, Localize for lost mode and Track for tracking mode
   bool Localize(const Marker&, const Frame&, Matches&);
   bool Track(const Marker&, const Frame&, Matches&);

   Mat prevScene;
   bool oneLost;
   const SystemAlgorithms& methods;
};

} // namespace avr

#endif // AVR_TRACKING_HPP
