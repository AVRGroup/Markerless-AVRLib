#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <opencv2/calib3d/calib3d.hpp>       // findHomografy e RANSAC
#include <opencv2/core/core.hpp>             // Mat, Point, Scalar, ...
#include <opencv2/features2d/features2d.hpp> // Orb
#include <opencv2/highgui/highgui.hpp>       // imshow
#include <opencv2/imgproc/imgproc.hpp>       // cvtColor
#include <opencv2/nonfree/features2d.hpp>    // SURF

#include "Camera.hpp"
#include "TransformationMatrix.hpp"

#define ESC             0x1B
#define TH_MATCH        0.7
#define TH_GOOD_MATCH   20
#define DATA_PATH       "../data/"

#define ORB 1

using namespace std;
using namespace avr;

int main()
{
   cv::Mat colorImage = imread(DATA_PATH "cormem_livro.jpg", cv::IMREAD_COLOR);
   cv::Mat grayImage = imread(DATA_PATH "cormem_livro.jpg", cv::IMREAD_GRAYSCALE);

   if(!colorImage.data or !grayImage.data ){
      cerr << "Falha ao carregar imagem" << endl;
      return EXIT_FAILURE;
   }

   #if ORB
      cv::OrbFeatureDetector      detector(1500); // Quanto maior => melhor qualidade, menor velocidade
      cv::OrbDescriptorExtractor  extractor;
      cv::BFMatcher               matcher;
   #else
      cv::SurfFeatureDetector     detector(1500); // Quanto maior => pior qualidade, maior velocidade
      cv::SurfDescriptorExtractor extractor;
      cv::FlannBasedMatcher       matcher;
   #endif //

   vector<cv::KeyPoint> imageKeys;
   detector.detect(grayImage, imageKeys);

   cv::Mat imageDescriptor;
   extractor.compute(grayImage, imageKeys, imageDescriptor);

   vector<cv::Point2f> imageCorner(4);
   imageCorner[0] = cv::Point2f(0, 0);
   imageCorner[1] = cv::Point2f(grayImage.cols, 0);
   imageCorner[2] = cv::Point2f(grayImage.cols, grayImage.rows);
   imageCorner[3] = cv::Point2f(0, grayImage.rows);

   cv::VideoCapture cap(DATA_PATH "cormem_scene.mp4");
   //VideoCapture cap(1);
   if (!cap.isOpened()){
      cerr << "Falha ao abrir video" << endl;
      return EXIT_FAILURE;
   }

   short tecla = 0x0, numFrames = 0;
   while(true){
      cv::Mat colorFrame;
      cap >> colorFrame;
      if(!colorFrame.data and numFrames == 0) continue;
      if(!colorFrame.data) break;

      numFrames++;

      cv::Mat grayFrame(colorFrame.size(), CV_8UC1);
      cvtColor(colorFrame, grayFrame, cv::COLOR_BGR2GRAY);

      vector<cv::KeyPoint> frameKeys;
      detector.detect(grayFrame, frameKeys);

      cv::Mat frameDescriptor;
      extractor.compute(grayFrame, frameKeys, frameDescriptor);

      vector< vector<cv::DMatch> > matches;
      matcher.knnMatch(imageDescriptor, frameDescriptor, matches, 2);

      vector<cv::DMatch> goodMatches;
      for(unsigned i = 0; i < matches.size(); i++)
         if(matches[i][0].distance < TH_MATCH * matches[i][1].distance)
            goodMatches.push_back(matches[i][0]);

      cv::Mat imageOut;
      cv::drawMatches(colorImage, imageKeys, colorFrame, frameKeys, goodMatches, imageOut,
                  cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(),
                  cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

      vector<cv::Point2f> frameCorner(4);
      if(goodMatches.size() >= TH_GOOD_MATCH){
         vector<cv::Point2f> imagePoints, framePoints;
         for(unsigned int i = 0; i < goodMatches.size(); i++ ){
            //Get the keypoints from the good matches
            imagePoints.push_back(imageKeys[goodMatches[i].queryIdx].pt);
            framePoints.push_back(frameKeys[goodMatches[i].trainIdx].pt);
         }
         cv::Mat homography = cv::findHomography(imagePoints, framePoints, cv::RANSAC, 4);
         cv::perspectiveTransform(imageCorner, frameCorner, homography);

         //Draw lines between the corners (the mapped object in the frame image )
         cv::line(imageOut, frameCorner[0] + cv::Point2f(grayImage.cols, 0), frameCorner[1] + cv::Point2f(grayImage.cols, 0), cv::Scalar(0, 255, 0), 4);
         cv::line(imageOut, frameCorner[1] + cv::Point2f(grayImage.cols, 0), frameCorner[2] + cv::Point2f(grayImage.cols, 0), cv::Scalar(0, 255, 0), 4);
         cv::line(imageOut, frameCorner[2] + cv::Point2f(grayImage.cols, 0), frameCorner[3] + cv::Point2f(grayImage.cols, 0), cv::Scalar(0, 255, 0), 4);
         cv::line(imageOut, frameCorner[3] + cv::Point2f(grayImage.cols, 0), frameCorner[0] + cv::Point2f(grayImage.cols, 0), cv::Scalar(0, 255, 0), 4);
      }

      /// Teste da Camera
      Camera cam(DATA_PATH "camera.yml");
      TransformationMatrix pose;
      cam.Pose(imageCorner, frameCorner, pose);
//      cout << pose;
//      cout << "M^-1\n" << pose.inverse();
      /// Fim teste da Camera

      imshow("Teste Camera", imageOut);
      tecla = cv::waitKey(1);
      if(tecla == ESC) break;
   }

   return EXIT_SUCCESS;
}
