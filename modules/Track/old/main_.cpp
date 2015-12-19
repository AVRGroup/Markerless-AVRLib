#include <iostream>
#include <vector>
#include <list>
#include <time.h>

//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/video/tracking.hpp>   // optflow
//#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <avr/core/Core.hpp>

using namespace avr;
using namespace std;

typedef vector<cv::Point3f> Coord3D;
typedef vector<cv::Point2f> Coord2D;

/**
 * Trackers
 *    Dado um marcador e uma cena, encontrar os pontos em coordenadas de mundo do marcador correspondentes na imagem da cena
 * Camera
 *    Dado as coordenadas de mundo do marcador e os correpondentes pontos na imagem, encontrar a tranformação entre estes espaços
 */

class Marker;
class RectangleMarker;
class CircularMarker;

class VisitorInterface {
public:
   virtual void Visit(RectangleMarker* mk) const = 0;
   virtual void Visit(CircularMarker* mk) const = 0;
};

class Visited {
public:
   virtual void Accept(const VisitorInterface& v) = 0;
};

class Marker : public Visited {
public:
   Marker() { }
   virtual ~Marker() { }

protected:
   // SetWorld(Coord3D& world) ...

private:
   size_t id;
   bool lost; // usar ponteiro? null = não visível
   Coord3D world;
   TMatx pose;
};

class RectangleMarker : public Marker {
public:
   RectangleMarker(/*Size*/) : Marker() { }

   void Accept(const VisitorInterface& v) {
      v.Visit(this);
   }

private:
   // size??
};

class CircularMarker : public Marker {
public:
   CircularMarker() : Marker() { }

   void Accept(const VisitorInterface& v) {
      v.Visit(this);
   }
};

class Tracker {
public:
   virtual ~Tracker() { };

   virtual void RegistryFrame(const Mat&) = 0;
protected:
   struct TrackState {
      virtual ~TrackState() = 0;
   };
   typedef list<TrackState>::iterator Iterator;

   static Iterator pushState(const TrackState& state);
   static void popState(const Iterator& it);

private:
   static list<TrackState> states;
};

class FeatureTracker : public Tracker {
private:
   // ...

public:
   FeatureTracker() { }

   void Track(Marker* mk) {
      mk->Accept(FeatureTrackerVisitor(this));
   }

   void RegistryFrame(const Mat& frame) {
      //
   }
private:
   struct FTState : public Tracker::TrackState {
   public:
      size_t markerID;
      Coord2D keypoints;
      Mat descriptors;
      BRect<int> region;

   };

   class FeatureTrackerVisitor : public VisitorInterface {
   private:
      FeatureTracker* instance;
   public:
      FeatureTrackerVisitor(FeatureTracker* instance) : instance(instance) { }

      void Visit(RectangleMarker* mk) const {
         /*
          *
          */
      }
      void Visit(CircularMarker* mk) const {

      }
   };
};

int main() {
   Marker* rect = new RectangleMarker;
   Marker* circ = new CircularMarker;

   FeatureTracker tracker;
   tracker.Track(rect);
   tracker.Track(circ);

   return 0;
}

Tracker::TrackState::~TrackState() { }
