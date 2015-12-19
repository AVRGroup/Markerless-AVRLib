#include <iostream>
#include <time.h>

//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/video/tracking.hpp>   // optflow
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>

#include <avr/track/Marker.hpp>
#include <avr/track/Features.hpp>
#include <avr/track/Trackers.hpp>

#define TEST 1

using namespace avr;
using namespace std;

int main() {
#if TEST
   cv::VideoCapture cap("../data/rabin_scene.mp4");

   Marker* cormemmk = new NaturalMarker("../data/rabin_object.jpg");
   FeatureTracker* trackerF = new FeatureTracker(new SIFTDetector(150),
                                                 new SIFTExtractor,
                                                 new BruteForceMatcher(cv::NORM_L2));
   MotionTracker*  trackerM = new MotionTracker();

   uint numFrames = 0, prev = numFrames;
   double time = (double) cv::getTickCount();

   while(true) {
      Mat captured;
      cap >> captured;

      if(captured.empty() and numFrames == 0) continue;
      if(captured.empty()) break;

      numFrames++;

      Frame frame(captured);

      Corners corners;
      if(cormemmk->IsLost())
      #ifdef DEBUG_
      { cout << numFrames - prev << endl; prev = numFrames;
      #endif // DEBUG_
         trackerF->Track(*cormemmk, frame, corners);
      #ifdef DEBUG_
      }
      #endif // DEBUG_
      else
         trackerM->Track(*cormemmk, frame, corners);

      if(not corners.empty()) {
         cv::line(frame.image, corners[0], corners[1], cv::Scalar(0, 255, 0), 4);
         cv::line(frame.image, corners[1], corners[2], cv::Scalar(0, 255, 0), 4);
         cv::line(frame.image, corners[2], corners[3], cv::Scalar(0, 255, 0), 4);
         cv::line(frame.image, corners[3], corners[0], cv::Scalar(0, 255, 0), 4);
      }

      cv::imshow("Tracker Test", frame.image);
      char key = cv::waitKey(1);
      if(key == 0x1B) // ESC
         break;
   }

   time = (double)(cv::getTickCount() - time) / cv::getTickFrequency();
   cout << (numFrames/time) << " fps\n";

   delete trackerF;
   delete trackerM;
   delete cormemmk;
#endif // TEST

   return 0;
}

/** Usando cortes do frame **/
/**
cv::SiftFeatureDetector detector;
   cv::SiftDescriptorExtractor extractor;
   cv::FlannBasedMatcher matcher;

   vector<cv::Point2f> targetPoints, scenePoints;
   vector<cv::KeyPoint> targetKeys, sceneKeys;
   vector<cv::DMatch> matches;
   cv::Mat targetDesc, sceneDesc;
   cv::Mat draw, cut;
   cv::Point2f min, max;
   cv::Rect roi;
   double start, time;

   /// INIT
   cv::Mat target = cv::imread("../data/cormem_livro.jpg", cv::IMREAD_GRAYSCALE);
   if(target.empty()) return -1;

   cv::imshow("Target", target);

   detector.detect(target, targetKeys);
   extractor.compute(target, targetKeys, targetDesc);

   vector<cv::Point2f> targetCorner(4), sceneCorner(4);
   targetCorner[0] = cv::Point2f(0.0, 0.0);
   targetCorner[1] = cv::Point2f(target.cols, 0.0);
   targetCorner[2] = cv::Point2f(target.cols, target.rows);
   targetCorner[3] = cv::Point2f(0.0, target.rows);

   cv::Mat scene = cv::imread("../data/cormem_scene(2).jpg", cv::IMREAD_GRAYSCALE);
   if(scene.empty()) return -1;

   /// START
   start = (double) clock();

   detector.detect(scene, sceneKeys);
   extractor.compute(scene, sceneKeys, sceneDesc);
   matcher.match(targetDesc, sceneDesc, matches);

   for(auto it : matches) {
      targetPoints.push_back(targetKeys[it.queryIdx].pt);
      scenePoints.push_back(sceneKeys[it.trainIdx].pt);
   }

   cv::Mat H = cv::findHomography(targetPoints, scenePoints, cv::RANSAC, 4);
   cv::perspectiveTransform(targetCorner, sceneCorner, H);

   //Draw lines between the corners (the mapped target in the frame image )
   scene.copyTo(draw);
   cv::line( draw, sceneCorner[0], sceneCorner[1], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[1], sceneCorner[2], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[2], sceneCorner[3], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[3], sceneCorner[0], cv::Scalar(0, 255, 0), 4 );

   min = cv::Point2f (9999999999, 999999999);
   max = cv::Point2f (-1, -1);
   for(auto it : sceneCorner){
      if(it.x < min.x) min.x = it.x;
      else if(it.x > max.x) max.x = it.x;

      if(it.y < min.y) min.y = it.y;
      else if(it.y > max.y) max.y = it.y;
   }

   roi = cv::Rect (min.x - 10, min.y - 10, (max.x - min.x) + 20, (max.y - min.y) + 20);
   cut = scene(roi);

   time = ((double) clock() - start) / CLOCKS_PER_SEC;
   cout << "Time 1 = " << time << endl;
   /// END

   cv::imshow("Scene", draw);
   cv::imshow("Scene Cut", cut);
   cv::waitKey();

   /// RE-INIT
   sceneKeys.clear(); matches.clear();
//   targetPoints.clear(); scenePoints.clear();
   sceneDesc.release(); sceneCorner.clear();
   H.release(); draw.release();

   /// RESTART
   start = (double) clock();

   detector.detect(cut, sceneKeys);
   extractor.compute(cut, sceneKeys, sceneDesc);
   matcher.match(targetDesc, sceneDesc, matches);

   for(auto it : matches) {
      targetPoints.push_back(targetKeys[it.queryIdx].pt);
      scenePoints.push_back(sceneKeys[it.trainIdx].pt);
   }

   H = cv::findHomography(targetPoints, scenePoints, cv::RANSAC, 4);
   cv::perspectiveTransform(targetCorner, sceneCorner, H);

   cut.copyTo(draw);
   cv::line( draw, sceneCorner[0], sceneCorner[1], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[1], sceneCorner[2], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[2], sceneCorner[3], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[3], sceneCorner[0], cv::Scalar(0, 255, 0), 4 );

   min = cv::Point2f (9999999999, 999999999);
   max = cv::Point2f (-1, -1);
   for(auto it : sceneCorner){
      if(it.x < min.x) min.x = it.x;
      else if(it.x > max.x) max.x = it.x;

      if(it.y < min.y) min.y = it.y;
      else if(it.y > max.y) max.y = it.y;
   }

   roi = cv::Rect (min.x - 10, min.y - 10, (max.x - min.x) + 20, (max.y - min.y) + 20);
   cut = cut(roi);

   time = ((double) clock() - start) / CLOCKS_PER_SEC;
   cout << "Time 2 = " << time << endl;
   /// END

   cv::imshow("Scene Cut 2", draw);
   cv::imshow("Scene New Cut", cut);
   cv::waitKey();

   return 0;
**/

/** Usando Optical Flow **/
/**
   vector<unsigned char> status; vector<float> err;
   vector<cv::Point2f> newScenePoints;
   cv::calcOpticalFlowPyrLK(scene, scene, scenePoints, newScenePoints, status, err);

//   cout << targetPoints.size() << "  " << scenePoints.size() << endl;
//   cout << newTargetPoints.size() << "  " << newScenePoints.size() << "  " << status.size() << endl;

   H = cv::findHomography(targetPoints, newScenePoints, cv::RANSAC, 4);
   cv::perspectiveTransform(targetCorner, sceneCorner, H);

   //Draw lines between the corners (the mapped target in the frame image )
   scene.copyTo(draw);
   cv::line( draw, sceneCorner[0], sceneCorner[1], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[1], sceneCorner[2], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[2], sceneCorner[3], cv::Scalar(0, 255, 0), 4 );
   cv::line( draw, sceneCorner[3], sceneCorner[0], cv::Scalar(0, 255, 0), 4 );

   min = cv::Point2f (9999999999, 999999999);
   max = cv::Point2f (-1, -1);
   for(auto it : sceneCorner){
      if(it.x < min.x) min.x = it.x;
      else if(it.x > max.x) max.x = it.x;

      if(it.y < min.y) min.y = it.y;
      else if(it.y > max.y) max.y = it.y;
   }

   roi = cv::Rect (min.x - 10, min.y - 10, (max.x - min.x) + 20, (max.y - min.y) + 20);
   cut = scene(roi);
**/
