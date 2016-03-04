#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <avr/view/ogl/Window.hpp>
#include <avr/Application.hpp>

#include <GL/glext.h>

namespace avr {

using std::string;

class Application::AppRenderer : public avr::Renderer {
public:
   AppRenderer(const SPtr<Camera>& cam, const SystemAlgorithms& methods, const vector<PreMarker>& setup, const std::string& video)
   : frame(), cap(), cam(cam), markers(), run(false), pause(false), count(0), time(0.0) {
      this->cap = (video != "") ? cv::VideoCapture(video) : cv::VideoCapture(0);
      this->frame = cv::Mat(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);

      this->featureTracker = new avr::FeatureTracker(methods);
      this->motionTracker = new avr::MotionTracker(Mat(), TrackResult(), methods);

      this->markers.reserve(setup.size());
      for(auto it : setup) {
         SPtr<Marker> marker = this->featureTracker->Unpack(it);
         this->markers.push_back(* marker);
      }
   }

   void Initialize();
   void Render() const;
   void Update();
   void Release();

private:
   void ProjectFrame(const GLvoid* image, GLsizei width, GLsizei height) const;

private:
   mutable cv::Mat frame;
   mutable cv::VideoCapture cap;

   mutable SPtr<Camera> cam;
   mutable SPtr<FeatureTracker> featureTracker;
   mutable SPtr<MotionTracker> motionTracker;

   mutable vector<Marker> markers;

   mutable std::atomic<bool>    run;
   mutable std::atomic<bool>    pause;
   mutable std::atomic<int>     count;
   mutable std::atomic<double>  time;

   GLuint texture;

   friend class Application;
};

Application::Application(const Builder& builder) : id(0), app(nullptr) {
   this->app = new AppRenderer(builder.cam, *builder.methods, builder.markers, builder.path);

   SPtr<Window> win = WindowManager::Create(GLUT::Window::Builder(builder.label));
   win->SetSize(this->app->frame.size());
   win->SetRenderer(this->app);
   this->id = win->GetID();
}

Application::~Application() {
   if(this->app->run) {
      this->Stop();
   }
}

void Application::Start() {
   this->app->run = true;
   this->app->time = (double) cv::getTickCount();
   GLUT::EnterMainLoop();
}

void Application::Stop() {
   this->app->run = false;
   this->app->time = (double)(cv::getTickCount() - this->app->time) / cv::getTickFrequency();
   cout << (double(this->app->count)/this->app->time) << " fps\n";
   //GLUT::LeaveMainLoop();
}

void Application::Pause() {
   this->app->pause = true;
}

void Application::Resume() {
   this->app->pause = false;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*\
*                                                                  Renderer                                                                   *
\*-------------------------------------------------------------------------------------------------------------------------------------------*/

//! Main Loop
void Application::AppRenderer::Render() const {
   // render if I can
   if(this->run && !this->pause) {
      Mat scene;
      this->cap >> scene;
      if(scene.empty()) return;

      this->count++;

      // computer visio process //
      for(auto& marker : this->markers) {
         TrackResult result;
         if(marker.Lost()) {
            result = this->featureTracker->Track(marker, scene);
            this->motionTracker->Set(scene, result);
         } else {
            result = this->motionTracker->Track(marker, scene);
         }

         if(result.scenePoints.size() > 20)
            marker.SetLost(false);
         else marker.SetLost(true);

         if(result.scenePoints.size() >= 4) {
            Coords2D sceneCorners;
            const Coords2D& markerCorners = marker.GetWorld();

            Mat homography = cv::findHomography(result.targetPoints, result.scenePoints, cv::RANSAC, 4);
            cv::perspectiveTransform(markerCorners, sceneCorners, homography);

   //         TMatx pose = this->cam->Pose(markerCorners, sceneCorners, marker.Lost());
   //         marker.GetModel().Draw(pose);

            if(!sceneCorners.empty()) {
               cv::line(scene, sceneCorners[0], sceneCorners[1], cv::Scalar(0, 255, 0), 4);
               cv::line(scene, sceneCorners[1], sceneCorners[2], cv::Scalar(0, 255, 0), 4);
               cv::line(scene, sceneCorners[2], sceneCorners[3], cv::Scalar(0, 255, 0), 4);
               cv::line(scene, sceneCorners[3], sceneCorners[0], cv::Scalar(0, 255, 0), 4);
            }
         }

         for(auto p : result.scenePoints) {
            cv::circle(scene, p, 3, cv::Scalar(0, 255, 0), 1);
         }
      }
      // computer visio process end //

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Mat flipped;
      cv::flip(scene, flipped, 0);
      ProjectFrame(flipped.ptr<GLubyte>(0), (GLsizei) flipped.cols, (GLsizei) flipped.rows);

//      Matx44f proj;
//      GetProjection(proj);
//      glMatrixMode(GL_PROJECTION);
//      glLoadMatrixf(proj.t().val);

      glutSwapBuffers ();
   }
}

void Application::AppRenderer::Update() {

}

void Application::AppRenderer::Initialize() {
   // selecionar cor de fundo (preto)
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &this->texture);
   glBindTexture(GL_TEXTURE_2D, this->texture);
   // Application of image treatment filters by OpenGL
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Generates the image on the memory
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->frame.cols, this->frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, this->frame.ptr<GLubyte>(0));
   glDisable(GL_TEXTURE_2D);
}

void Application::AppRenderer::Release() {
   glDeleteTextures(1, &this->texture);
}

void Application::AppRenderer::ProjectFrame(const GLvoid* image, GLsizei width, GLsizei height) const {
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, this->texture);
   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image);

   glColor3d(1.0, 1.0, 1.0);
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

} // namespace avr
