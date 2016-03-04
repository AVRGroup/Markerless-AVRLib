#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <opencv2/core/core.hpp>             // Mat, Point, Scalar, ...
#include <opencv2/calib3d/calib3d.hpp>       // findHomografy e RANSAC
#include <opencv2/features2d/features2d.hpp> // Orb
#include <opencv2/highgui/highgui.hpp>       // imshow
#include <opencv2/imgproc/imgproc.hpp>       // cvtColor
#include <opencv2/nonfree/features2d.hpp>    // SURF

#include <avr/camera/Camera.hpp>
#include "Interface.hpp"

#define ESC             0x1B
#define TH_MATCH        0.7
#define TH_GOOD_MATCH   20
#define DATA_PATH       "../data/"

#define ORB 1

using namespace std;
using namespace avr;

static void setupLight();

const double pi = 4 * atan(1);

class CameraAxisView : Interface{
   private:
      cv::Mat colorImage;
      cv::Mat grayImage;

      #if ORB
         cv::OrbFeatureDetector      detector;
         cv::OrbDescriptorExtractor  extractor;
         cv::BFMatcher               matcher;
      #else
         cv::SurfFeatureDetector     detector;
         cv::SurfDescriptorExtractor extractor;
         cv::FlannBasedMatcher       matcher;
      #endif // ORB

      cv::Mat imageDescriptor;
      vector<cv::KeyPoint> imageKeys;
      vector<cv::Point2f>  imageCorner;

      cv::VideoCapture cap;

      struct Sphere {
         static bool leftButton;
         static int  lastx, lasty;  // mouse
         double theta, phi, range;  // params

         double x() const { return (range * sin(theta * pi/180.0) * sin(phi * pi/180.0)); }
         double y() const { return (range * cos(phi * pi/180.0)); }
         double z() const { return (range * cos(theta * pi/180.0) * sin(phi * pi/180.0)); }

         void Update(int x, int y){
            if(leftButton) {
               theta += x - lastx;
               phi   += y - lasty;

               theta = (theta < -85.0) ? -85.0 : (theta > 85.0) ? 85.0 : theta;
               phi   = (phi < 10.0) ? 10.0 : (phi > 170.0) ? 170.0 : phi;
            } else {
               range -= y - lasty;
               range = (range < 10.0) ? 10.0 : (range > 1500.0) ? 1500.0 : range;
            }

            lastx = x;
            lasty = y;
         }

         static void Reset(int x, int y) { lastx = x; lasty = y; }

         void operator() (double t, double p, double r) { theta = t; phi = p; range = r; }
      };

      Sphere sphere;

   public:
      void Init(int argc, char* argv[]){
         sphere(0.0, 90.0, 1100.0);

         colorImage = imread(DATA_PATH "cormem_object.jpg", cv::IMREAD_COLOR);
         grayImage = imread(DATA_PATH "cormem_object.jpg", cv::IMREAD_GRAYSCALE);

         if(!colorImage.data or !grayImage.data ){
            cerr << "Falha ao carregar imagem" << endl;
            exit(EXIT_FAILURE);
         }

         #if ORB
            detector = cv::OrbFeatureDetector(1500);
         #else
            detector = cv::SurfFeatureDetector(1500);
         #endif

         detector.detect(grayImage, imageKeys);
         extractor.compute(grayImage, imageKeys, imageDescriptor);

         imageCorner.resize(4);
         imageCorner[0] = Point2f(0.0f, 0.0f);
         imageCorner[1] = Point2f(grayImage.cols, 0.0f);
         imageCorner[2] = Point2f(grayImage.cols, grayImage.rows);
         imageCorner[3] = Point2f(0.0f, grayImage.rows);

         cap = cv::VideoCapture(DATA_PATH "cormem_scene.mp4");
//         cap = cv::VideoCapture(0);

         Super::Init(argc, argv);
         glClearColor(0.1, 0.1, 0.1, 1.0);
         Super::MainLoop();
      }

      void DrawCamera(Camera& cam, TMatx& pose){
         avr::Quaternion q = pose.GetQuaternion();
         avr::Point3d pos  = pose.GetPosition();
         Vec3d axis        = q.GetAxis();
         double angle      = q.GetAngle().Get();

         glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
            glLineWidth(2);
            glBegin(GL_LINES);
               glColor3d(1.0, 1.0, 0.0);
               glVertex3d(-pos.x, -pos.y, -pos.z);
               glColor3d(1.0, 0.3, 0.0);
               glVertex3d(0.0, 0.0, 0.0);
            glEnd();
            glLineWidth(1);

            setupLight();
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);

            glTranslatef(-pos.x, -pos.y, -pos.z);
            glRotated(angle, axis[0], axis[1], axis[2] );
            glPushMatrix();
               glColor4f(1.0, 1.0, 1.0, 1.0 );
               glTranslatef( 0.0, 0.0, -10.0 );
               glScalef( 10.0, 10.0, 20.0 );
               glRotatef(180.0, 1.0, 0.0, 0.0);
               glutSolidCone(1.0, 1.0, 10, 10);
            glPopMatrix();

            glColor4f( 0.4, 0.0, 0.0, 1.0 );
            glLineWidth(3);
            glPushMatrix();
               glTranslatef( 0.0, 0.0, -40.0 );
               glScalef( 30.0, 30.0, 40.0 );
               glutWireCube(1.0);
            glPopMatrix();
            glLineWidth(1);

            glColor4f( 0.5, 0.0, 0.0, 1.0 );
            glPushMatrix();
               glTranslatef( 0.0, 0.0, -40.0 );
               glScalef( 30.0, 30.0, 40.0 );
               glutSolidCube(1.0);
            glPopMatrix();

            glDisable(GL_LIGHTING);
         glPopMatrix();

         // draws text
         stringstream stream;
         stream << "Distance: " << setiosflags(ios::fixed) << setprecision(2) <<
                     sqrt( pow(pos.x, 2.0) + pow(pos.y, 2.0) + pow(pos.z, 2.0) ) << "mm";
         string str(stream.str());

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0);
         glColor3f(1.0, 1.0, 1.0);
         glRasterPos2f(7.2, 0.3);
         for(uint8_t i = 0; i < str.length(); i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      }

      void DrawInFrame(cv::Mat& frame){
         cv::Mat glPixels = cv::Mat(frame.size(), frame.type());
         glReadPixels(0, 0, frame.cols, frame.rows, GL_BGR , GL_UNSIGNED_BYTE, glPixels.ptr<uchar>(0));

         cv::Mat flipy = cv::Mat::zeros(glPixels.size(), glPixels.type());
         for(int b = glPixels.rows-1, c = 0; b >= 0; b--, c++){
            double* lb = glPixels.ptr<double>(b);
            double* lc = flipy.ptr<double>(c);
            memcpy(lc, lb, glPixels.cols * sizeof(double));
         }

         frame = frame + flipy;
         for(int b = frame.rows-1, c = 0; b >= 0; b--, c++){
            double* lb = frame.ptr<double>(b);
            double* lc = flipy.ptr<double>(c);
            memcpy(lc, lb, frame.cols * sizeof(double));
         }

//         glDrawPixels(flipy.cols, flipy.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, flipy.ptr<uchar>(0));
      }

      void Draw(){
         static unsigned numFrames = 0;

         cv::Mat colorFrame;
         cap >> colorFrame;
         if(!colorFrame.data and numFrames == 0) return;
         if(!colorFrame.data) exit(EXIT_SUCCESS);

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

         vector<Point2f> frameCorner(4);
         if(goodMatches.size() >= TH_GOOD_MATCH){
            vector<cv::Point2f> imagePoints, framePoints;
            for(unsigned int i = 0; i < goodMatches.size(); i++ ){
               //Get the keypoints from the good matches
               imagePoints.push_back(imageKeys[goodMatches[i].queryIdx].pt);
               framePoints.push_back(frameKeys[goodMatches[i].trainIdx].pt);
            }
            cv::Mat homography = cv::findHomography(imagePoints, framePoints, cv::RANSAC, 4);
            cv::perspectiveTransform(imageCorner, frameCorner, homography);
         }

         /// Teste da Camera AQUI
         Camera cam(DATA_PATH "camera.yml");
         TMatx pose = cam.Pose(imageCorner, frameCorner, true);
         /// Fim teste da Camera

         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         glDepthFunc(GL_LEQUAL);
         glEnable(GL_DEPTH_TEST);
         glEnable(GL_NORMALIZE);

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(60.0, 1.0, 0.05, 2000.0);
         gluLookAt(sphere.x(), sphere.y(), sphere.z(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

         // draw the plan with axis and cube
         glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            setupLight();

            for(short j = -300; j <= 200; j+= 100) {
               for(short i = -300; i <= 200; i+= 100) {
                  glBegin(GL_QUADS);
                     glNormal3f(0.0, 0.0, 1.0);
                     if((j/100 + i/100) % 2)
                        glColor4f(0.6, 0.6, 0.6, 1.0);
                     else
                        glColor4f(0.0, 0.0, 0.1, 1.0);
                     glVertex3f(i,       j,       0.0);
                     glVertex3f(i,       j + 100, 0.0);
                     glVertex3f(i + 100, j + 100, 0.0);
                     glVertex3f(i + 100, j,       0.0);
                  glEnd();
               }
            }

            glColor4f(0.7, 0.4, 0.0, 1.0);
            //glTranslatef(37.5, -62.5, 13.0);
            glScalef(1.5, 2.5, 0.5);
            glutSolidCube(50.0);

            glDisable( GL_LIGHTING );
         glPopMatrix();

         this->DrawCamera(cam, pose);

         glDisable(GL_NORMALIZE);
         glDisable(GL_DEPTH_TEST);

//         this->DrawInFrame(colorFrame);

         cv::imshow("Video", colorFrame);
         cv::waitKey(1);

         glutSwapBuffers();
      }

      void MouseEvent(int button, int state, int x, int y){
         y = $WINDOW_HEIGHT - y;

         Sphere::leftButton = button == GLUT_LEFT_BUTTON;
         sphere.Update(x, y);
      }

      void MotionEvent(int x, int y){
         y = $WINDOW_HEIGHT - y;
         sphere.Update(x, y);
      }

      void PassiveMotionEvent(int x, int y){
         y = $WINDOW_HEIGHT - y;
         Sphere::Reset(x, y);
      }

      void Reshape(int w, int h){
         glViewport(0, 0, w, h);

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(45.0, 1.0, 0.05, 50.0);
         gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
      }

   private:
      typedef class Interface Super;
};

int    CameraAxisView::Sphere::lastx = 0;
int    CameraAxisView::Sphere::lasty = 0;
bool   CameraAxisView::Sphere::leftButton = true;

void setupLight(){
   static int  mat_f = 1;
   GLfloat     mat_amb_diff[]  = {0.9, 0.9, 0.0, 1.0};
   GLfloat     mat_specular[]  = {0.5, 0.5, 0.5, 1.0};
   GLfloat     mat_shininess[] = {10.0};
   GLfloat     light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
   GLfloat     light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat     light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat     light_position[] = { 100.0, 300.0, 700.0, 1.0 };

   if( mat_f )
   {
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      mat_f = 0;
   }

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

int main(int argc, char** argv){
   CameraAxisView app;
   app.Init(argc, argv);

   return 0;
}
