#include <iostream>
#include <time.h>

#include "Interface.hpp"

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/video/tracking.hpp>   // optflow
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#undef near
#undef far

#define DATA   "../data/"

using namespace cv;
using namespace std;

void setupLight(const Matx44f&);
template <typename Tp> void JoinMatx(const Matx<Tp, 3, 3>& r, const Matx<Tp, 3, 1>& t, Matx<Tp, 4, 4>& out);
template <typename Tp> Point3_<Tp> operator * (const Matx<Tp, 4, 4>& m, const Point_<Tp>& p);

struct Marker {
   string name;
   vector<Point3f> world;
   Point3f center;

   Marker(const string& name, const Size& size) : name(name), world(vector<Point3f>(4)) {
      center = Point3f(size.width/2.0f, size.height/2.0f, 0.0f);
      world[0] = Point3f(-center.x, -center.y, 0.0f);
      world[1] = Point3f(-center.x, center.y, 0.0f);
      world[2] = Point3f(center.x, center.y, 0.0f);
      world[3] = Point3f(center.x, -center.y, 0.0f);
   }

   string GetVideoPath() const {
      return DATA + name + "_scene.mp4";
   }
   string GetImagePath() const {
      return DATA + name + "_object.jpg";
   }
};

static Marker cormem = Marker("cormem", Size(212, 278));
static Marker rabin = Marker("rabin", Size(176, 254));
static Marker guitar = Marker("guitar", Size(120, 120));

class App : public Interface {
private:
   // Application
   TermCriteria termcrit;
   cv::VideoCapture cap;
   int numFrames;
   double start, time;

   // Camera
   Matx<float, 3, 3> intrinsecs;
   vector<float> coefficients;

   // Tracker
   Mat prev;
   vector<Point2f> prevCorners, objCorners;
   vector<Point2f> prevVertex, objVertex;

   Marker marker;

   typedef Interface super;

public:
   App() : numFrames(0), start(0.0), time(0.0), prevVertex(vector<Point2f>(4)), objVertex(vector<Point2f>(4)), marker(rabin) {
      this->termcrit = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
      this->cap = cv::VideoCapture(marker.GetVideoPath());

      this->intrinsecs = Matx<float, 3, 3>(8.0626068834051375e+02, 0.0, 3.0466336181680953e+02,
                                             0.0, 8.0565309661667538e+02, 2.4111010439780640e+02,
                                             0.0, 0.0, 1.0);
      this->coefficients = vector<float>( {2.8916956217369554e-02,
                                          -3.4686981332013622e-01,
                                          -4.4929499748999320e-03,
                                          -4.5424789303775905e-04,
                                          1.7749986582664894e+00} );
   } // Ctor

   int Start(int argc, char* argv[]) {
      // Inicializa, habilita método de desenho exclusivo para primeiro frame e pausa neste
      super::Init(argc, argv, true);
      glClearColor(0.1, 0.1, 0.1, 1.0);

      start = (double) cv::getTickCount();
      super::Start();

      return 0;
   } // Start

   //! @override
   void FirstDraw() {
      Mat show;
      while(show.empty()) cap >> show;
      cvtColor(show, prev, CV_BGR2GRAY, 1);

      /// FIRST TRACK /////////////////////////////////////////
//      cv::goodFeaturesToTrack(prev, prevCorners, 25, 0.01, 20);

      Mat obj = cv::imread(marker.GetImagePath(), IMREAD_GRAYSCALE);
      if(obj.empty()) exit(1);

      cv::SIFT sift(750);
      Mat descObj, descFrame;
      vector<cv::KeyPoint> keysObj, keysFrame;

      sift.detect(obj, keysObj);
      sift.detect(prev, keysFrame);

      sift.compute(obj, keysObj, descObj);
      sift.compute(prev, keysFrame, descFrame);

      cv::BFMatcher matcher;
      vector<cv::DMatch> matches;
      matcher.match(descObj, descFrame, matches);

      for(auto it : matches) {
         this->objCorners.push_back(keysObj[it.queryIdx].pt);
         this->prevCorners.push_back(keysFrame[it.trainIdx].pt);
      }
      ///////////////////////////////////////////////////////////////////////

      /// HOMOGRAFIA ////////////////////////////////////////////////////////
      objVertex[0] = Point2f(0, 0);
      objVertex[1] = Point2f(0, obj.rows);
      objVertex[2] = Point2f(obj.cols, obj.rows);
      objVertex[3] = Point2f(obj.cols, 0);

      Mat H = cv::findHomography(this->objCorners, this->prevCorners, RANSAC, 4);
      cv::perspectiveTransform(this->objVertex, this->prevVertex, H);
      ///////////////////////////////////////////////////////////////////////

      /// CAMERA ////////////////////////////////////////////////////////////
//      vector<Point3f> objectPoints (this->objVertex.begin(), this->objVertex.end());
      vector<Point2f>& imagePoints = this->prevVertex;

      Mat _rvec, _tvec; Matx33f rvec; Matx31f tvec;
      cv::solvePnPRansac(marker.world, imagePoints, intrinsecs, coefficients, _rvec, _tvec, false, 72, 8.0, 100, noArray(), cv::EPNP);

      Rodrigues(_rvec, _rvec);
      _rvec.copyTo(rvec);
      _tvec.copyTo(tvec);

      Matx44f pose;
      JoinMatx(rvec, tvec, pose);
      //////////////////////////////////////////////////////////////////////

      /// DESENHOS INICIAIS ////////////////////////////////////////////////
#ifdef DEBUG_
      for(auto p : prevCorners)
         cv::circle(show, p, 5, Scalar(255, 0, 0), 1);
#endif // DEBUG_

      // HOMOGRAFIA
      cv::line(show, prevVertex[0], prevVertex[1], Scalar(0, 255, 0), 3);
      cv::line(show, prevVertex[1], prevVertex[2], Scalar(0, 255, 0), 3);
      cv::line(show, prevVertex[2], prevVertex[3], Scalar(0, 255, 0), 3);
      cv::line(show, prevVertex[3], prevVertex[0], Scalar(0, 255, 0), 3);
      //
      this->Render(show, pose);
      //////////////////////////////////////////////////////////////////////
   }

   //! @override
   void Draw() {
      Mat curr, show;
      cap >> show;

      if(show.empty() and numFrames == 0) return;
      if(show.empty()) { this->Fps(); super::Stop(); }
      cvtColor(show, curr, CV_BGR2GRAY, 1);

      numFrames++;

      vector<Point2f> currCorners; vector<uchar> status; vector<float> err;
      cv::calcOpticalFlowPyrLK(prev, curr, prevCorners, currCorners, status, err, Size(31, 31), 3, termcrit, 0, 1e-3);

      size_t k = 0;
      for(size_t i = 0; i < currCorners.size(); i++)
         if(status[i]) {
            currCorners[k] = currCorners[i];
            objCorners[k++] = objCorners[i];
         }
      currCorners.resize(k);
      objCorners.resize(k);

#ifdef DEBUG_
      cout << k << endl;
#endif // DEBUG_
      if(k < 4) return;
      vector<Point2f> currVertex;
      Mat H = cv::findHomography(this->objCorners, currCorners, RANSAC, 4);
      cv::perspectiveTransform(this->objVertex, currVertex, H);
      // HOMOGRAFIA
      cv::line(show, currVertex[0], currVertex[1], Scalar(0, 255, 0), 3);
      cv::line(show, currVertex[1], currVertex[2], Scalar(0, 255, 0), 3);
      cv::line(show, currVertex[2], currVertex[3], Scalar(0, 255, 0), 3);
      cv::line(show, currVertex[3], currVertex[0], Scalar(0, 255, 0), 3);
      //

      /// CAMERA //////////////////////////////////////////////////////////
//      vector<Point3f> objectPoints (this->objVertex.begin(), this->objVertex.end());
      vector<Point2f>& imagePoints = currVertex;

      Mat rvec, tvec; Matx33f rmatx; Matx31f tmatx;
      // Retorna (em rvec e tvec) a tranformação que leva o sistema de coordenadas do objeto ao sistema da câmera
      // Em outras palavras, posiciona o objeto em coordenadas da câmera
      cv::solvePnP(marker.world, imagePoints, intrinsecs, coefficients, rvec, tvec, false, ITERATIVE);

      Rodrigues(rvec, rvec);
      rvec.copyTo(rmatx);
      tvec.copyTo(tmatx);

      Matx44f pose;
      JoinMatx(rmatx, tmatx, pose);
      /////////////////////////////////////////////////////////////////////
#ifdef DEBUG_
      for(size_t i = 0; i < currCorners.size(); i++) {
         cv::circle(show, prevCorners[i], 5, Scalar(255, 0, 0), 1);
         cv::circle(show, currCorners[i], 3, Scalar(255, 0, 0), -1);
         cv::line(show, prevCorners[i], currCorners[i], Scalar(255, 0, 0));
      }
#endif // DEBUG_
      this->Render(show, pose);

      curr.assignTo(prev);
      prevCorners.assign(currCorners.begin(), currCorners.end());
   } // Draw

   void Render(const Mat& image, const Matx44f& pose = Matx44f::eye()) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glDepthFunc(GL_LEQUAL);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_NORMALIZE);

      Mat flipped;
      cv::flip(image, flipped, 0);
      ProjectFrame(flipped.ptr<uchar>(0), (GLsizei) flipped.cols, (GLsizei) flipped.rows);

      Matx44f proj;
      GetProjection(proj);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf(proj.t().val);

      Matx44f camPose = pose.inv();
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(pose.t().val);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glColor4d(0.0, 0.0, 0.0, 0.5);
      glBegin(GL_QUADS);
         // Plano xy
         glVertex3f(-50.0, -50.0, 0.0f);
         glVertex3f(-50.0, 50.0, 0.0f);
         glVertex3f(50.0, 50.0, 0.0f);
         glVertex3f(50.0, -50.0, 0.0f);
      glEnd();

      glDisable(GL_BLEND);

      glColor3d(0, 0.6, 1.0);
      glPushMatrix();
         glEnable(GL_LIGHTING);
         glEnable(GL_LIGHT0);
         setupLight(camPose);

         glTranslatef(0.0f, 0.0f, -25.0f);
         glutSolidCube(50);

         glDisable(GL_LIGHTING);
      glPopMatrix();

      glLineWidth(3);
      glColor3d(1.0, 0.6, 0.0);
      glBegin(GL_LINES);
         glVertex3d(0.0, 0.0, 0.0);
         glVertex3d(100.0, 0.0, 0.0);

         glVertex3d(0.0, 0.0, 0.0);
         glVertex3d(0.0, 100.0, 0.0);
         glColor3d(0.6, 0.2, 0.0);
         glVertex3d(0.0, 0.0, 0.0);
         glVertex3d(0.0, 0.0, 100.0);
      glEnd();

      glDisable(GL_NORMALIZE);
      glDisable(GL_DEPTH_TEST);

      glutSwapBuffers();
   }

   void ProjectFrame(const GLvoid* image, GLsizei width, GLsizei height) {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

      GLuint textureID;

      glEnable (GL_TEXTURE_2D);
      glGenTextures (1, &textureID);

      glBindTexture (GL_TEXTURE_2D, textureID);

      // Application of image treatment filters by OpenGL
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      // Generates the image on the memory
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor3d(1.0, 1.0, 1.0);
      glBindTexture (GL_TEXTURE_2D, textureID);
      glBegin(GL_QUADS);
         glTexCoord2i(0, 0);
         glVertex3i(0, 0, -1);

         glTexCoord2i(0, 1);
         glVertex3i(0, height, -1);

         glTexCoord2i(1, 1);
         glVertex3i(width, height, -1);

         glTexCoord2i(1, 0);
         glVertex3i(width, 0, -1);
      glEnd();

      glDisable(GL_TEXTURE_2D);
   }

   //! @override
   void KeyBoardEvent(unsigned char key, int x, int y) {
      if(key == 0x1B) {// ESC
         this->Fps();
         this->cap.release();
         super::Stop();
      }
      else if(key == ' ') {
         if(isPaused()) {
            super::Restart();
            start = (double) cv::getTickCount();
         }
         else {
            this->Fps();
            super::Pause();
         }
      }

   } // KeyBoardEvent

   //! @override
   void Reshape(int w, int h) {
      glViewport(0, 0, w, h);

      float width = float(cap.get(CV_CAP_PROP_FRAME_WIDTH));
      float height = float(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glViewport(0, 0, (GLsizei) width, (GLsizei) height);
   } // Reshape

   void GetProjection(Matx44f& projection, float near = 0.05, float far = 1.e3) {
      float width = float(cap.get(CV_CAP_PROP_FRAME_WIDTH));
      float height = float(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

      float& fx = intrinsecs(0, 0);
      float& fy = intrinsecs(1, 1);
      float& cx = intrinsecs(0, 2);
      float& cy = intrinsecs(1, 2);

      projection(0, 0) = 2.0 * fx / width;
      projection(0, 2) = 1.0 - (2.0 * cx / width);
      projection(1, 1) = 2.0 * fy / height;
      projection(1, 2) = 1.0 - (2.0 * cy / height);
      projection(2, 2) = (near - far) / (far - near);
      projection(2, 3) = (-2.0 * far * near) / (far - near);
      projection(3, 2) = -1.0;
      projection(3, 3) = 0.0;

//      Matx44f reverseyz = Matx44f::eye();
//      reverseyz(1, 1) = reverseyz(2, 2) = -1;
//      projection = (reverseyz * projection);
   }

   bool GetRotation(const Matx44f& m, Vec3f& pos, float& ang) {
      Vec4f q;
      q[0] = m(0, 0) + m(1, 1) + m(2, 2) + 1.0;
      if(q[0] < 0.0) return false;

      q[0] = std::sqrt(q[0]);
      q[1] = (m(2, 1) - m(1, 2)) / (2.0 * q[0]);
      q[2] = (m(0, 2) - m(2, 0)) / (2.0 * q[0]);
      q[3] = (m(1, 0) - m(0, 1)) / (2.0 * q[0]);
      q[0] = q[0] / 2.0;

      ang = (2.0 * std::acos(q[0])) * (180.0 / CV_PI);
      if(q[0] == 1.0f) {
         pos = Vec3f(0.0f, 0.0f, 0.0f);
         return false;
      } else if(q[0] == 0.0f)
         pos = Vec3f(q[1], q[2], q[3]);
      else {
         float sin = std::sin(std::acos(q[0]));
         pos = Vec3f(q[1]/sin, q[2]/sin, q[3]/sin);
      }

      return true;
   }

   inline void Fps() {
      time += (double)(cv::getTickCount() - start) / cv::getTickFrequency();
      cout << (numFrames/time) << " fps\n";
   }
};

int main(int argc, char* argv[])
{
   App app = App();
   int state = app.Start(argc, argv);
   return state;
}

void setupLight(const Matx44f& pose){
   static int  mat_f = 1;
   GLfloat     mat_amb_diff[]  = {0.9, 0.9, 0.0, 1.0};
   GLfloat     mat_specular[]  = {0.5, 0.5, 0.5, 1.0};
   GLfloat     mat_shininess[] = {10.0};
   GLfloat     light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
   GLfloat     light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat     light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat     light_position[] = { pose(0, 3), pose(1, 3), pose(2, 3), 1.0 };

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

template <typename Tp>
void JoinMatx(const Matx<Tp, 3, 3>& r, const Matx<Tp, 3, 1>& t, Matx<Tp, 4, 4>& out) {
   out = Matx<Tp, 4, 4>::eye();
   for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++)
         out(i, j) = r(i, j);
      out(i, 3) = t(i, 0);
   }
   // OpenGL has inverted y and z coords
   Matx<Tp, 4, 4> reverseyz = Matx<Tp, 4, 4>::eye();
   reverseyz(1, 1) = reverseyz(2, 2) = -1;
   out = (reverseyz * out);
}

template <typename Tp>
Point3_<Tp> operator * (const Matx<Tp, 4, 4>& m, const Point_<Tp>& p) {
   return Point3_<Tp>(m(0,0) * p.x + m(0,1) * p.y + m(0,3),
                      m(1,0) * p.x + m(1,1) * p.y + m(1,3),
                      m(2,0) * p.x + m(2,1) * p.y + m(2,3));
}
