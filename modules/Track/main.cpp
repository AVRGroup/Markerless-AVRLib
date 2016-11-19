#include <iostream>
#include <time.h>

//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/video/tracking.hpp>   // optflow
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>

#include <avr/track/Marker.hpp>
#include <avr/track/Algorithms.hpp>
#include <avr/track/Tracking.hpp>

using namespace avr;
using namespace std;

int main() {
   cv::VideoCapture cap("../data/guitar_scene.mp4");
   if(!cap.isOpened()) {
      cerr << "Camera/Video was not opened\n";
      return 1;
   }

   //SystemAlgorithms algorithms(new SIFTDetector(500), new SIFTExtractor, new BruteForceMatcher(cv::NORM_L2), new LucasKanadeAlgorithm);
   SystemAlgorithms algorithms = SystemAlgorithms::Create(false, true);

   HybridTracker tracker(algorithms);

   Marker target = tracker.Registry(PreMarker("../data/guitar_object.jpg", nullptr));
   Size2i targetSize = target.GetSize();

   vector<cv::Point2f> targetCorner(4);
   targetCorner[0] = cv::Point2f(0.0, 0.0);
   targetCorner[3] = cv::Point2f(0.0, targetSize.height);
   targetCorner[2] = cv::Point2f(targetSize.width, targetSize.height);
   targetCorner[1] = cv::Point2f(targetSize.width, 0.0);

   uint numFrames = 0;
   double time = (double) cv::getTickCount();

   Frame frame;
   Matches matches;

   while(true) {
      cap >> frame.image;

      if(frame.image.empty() and numFrames == 0) continue;
      if(frame.image.empty()) break;

      numFrames++;

      tracker.Update(frame);
      matches = tracker.Find(target, frame);

      if(matches.size() > 20) target.SetLost(false);
      else target.SetLost(true);

      if(matches.size() >= 4) {
         vector<Point2f> corners;
         Mat homography = cv::findHomography(matches.targetPts(), matches.scenePts(), cv::RANSAC, 4);
         cv::perspectiveTransform(targetCorner, corners, homography);

         if(!corners.empty()) {
            cv::line(frame.image, corners[0], corners[1], cv::Scalar(0, 255, 0), 4);
            cv::line(frame.image, corners[1], corners[2], cv::Scalar(0, 255, 0), 4);
            cv::line(frame.image, corners[2], corners[3], cv::Scalar(0, 255, 0), 4);
            cv::line(frame.image, corners[3], corners[0], cv::Scalar(0, 255, 0), 4);
         }
      }

      for(auto p : matches.scenePts()) {
         cv::circle(frame.image, p, 3, cv::Scalar(0, 255, 0), 1);
      }

      cv::imshow("Tracker Test", frame.image);
      char key = cv::waitKey(1);
      if(key == 0x1B) // ESC
         break;
   }

   time = (double)(cv::getTickCount() - time) / cv::getTickFrequency();
   cout << (numFrames/time) << " fps\n";

   return 0;
}
