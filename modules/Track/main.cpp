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
   cv::VideoCapture cap("../data/rabin_scene.mp4");
   if(!cap.isOpened()) {
      cerr << "Camera/Video was not opened\n";
      return 1;
   }

   //SystemAlgorithms algorithms(new SIFTDetector(500), new SIFTExtractor, new BruteForceMatcher(cv::NORM_L2), new LucasKanadeAlgorithm);
   SystemAlgorithms algorithms = SystemAlgorithms::Create(true, false);

   FeatureTracker featureTracker(algorithms);
   MotionTracker motionTracker(Mat(), TrackResult(), algorithms);

   Marker target = * featureTracker.Unpack(PreMarker("../data/rabin_object.jpg", nullptr));
   Size2i targetSize = target.GetSize();

   vector<cv::Point2f> targetCorner(4);
   targetCorner[0] = cv::Point2f(0.0, 0.0);
   targetCorner[3] = cv::Point2f(0.0, targetSize.height);
   targetCorner[2] = cv::Point2f(targetSize.width, targetSize.height);
   targetCorner[1] = cv::Point2f(targetSize.width, 0.0);

   uint numFrames = 0;
   double time = (double) cv::getTickCount();

   //Mat captured(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FORMAT));

   while(true) {
      Mat scene;
      cap >> scene;

      if(scene.empty() and numFrames == 0) continue;
      if(scene.empty()) break;

      numFrames++;

      TrackResult trackResult;
      if(target.Lost()) {
         cout << "lost\n";
         trackResult = featureTracker.Track(target, scene);
         motionTracker.Set(scene, trackResult);
      } else {
         trackResult = motionTracker.Track(target, scene);
      }

      if(trackResult.scenePoints.size() > 20) target.SetLost(false);
      else target.SetLost(true);

      if(trackResult.scenePoints.size() >= 4) {
         vector<Point2f> corners;
         Mat homography = cv::findHomography(trackResult.targetPoints, trackResult.scenePoints, cv::RANSAC, 4);
         cv::perspectiveTransform(targetCorner, corners, homography);

         if(!corners.empty()) {
            cv::line(scene, corners[0], corners[1], cv::Scalar(0, 255, 0), 4);
            cv::line(scene, corners[1], corners[2], cv::Scalar(0, 255, 0), 4);
            cv::line(scene, corners[2], corners[3], cv::Scalar(0, 255, 0), 4);
            cv::line(scene, corners[3], corners[0], cv::Scalar(0, 255, 0), 4);
         }
      }

      for(auto p : trackResult.scenePoints) {
         cv::circle(scene, p, 3, cv::Scalar(0, 255, 0), 1);
      }

      cv::imshow("Tracker Test", scene);
      char key = cv::waitKey(1);
      if(key == 0x1B) // ESC
         break;
   }

   time = (double)(cv::getTickCount() - time) / cv::getTickFrequency();
   cout << (numFrames/time) << " fps\n";

   return 0;
}
