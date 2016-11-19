#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <avr/view/ogl/Window.hpp>
#include <avr/Application.hpp>

#include <GL/glext.h>

#include <ctime>
#include <chrono>
#include <sstream>

namespace avr {

using std::cout;
using std::cerr;
using std::string;

class Application::AppRenderer : public avr::Renderer {
public:
   AppRenderer(const SPtr<Camera>& cam, const SystemAlgorithms& methods, const vector<PreMarker>& setup, const std::string& video)
   : id(0), frame(), cap(), cam(cam), markers(), run(false), pause(false), count(0), time(0.0) {
      this->cap = (video != "") ? cv::VideoCapture(video) : cv::VideoCapture(0);
      this->frame = cv::Mat(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);

      this->tracker = new avr::HybridTracker(methods);

      this->markers.reserve(setup.size());
      for(auto it : setup) {
         Marker marker = this->tracker->Registry(it);
         this->markers.push_back(marker);
      }
   }

   void Initialize();
   void Render() const;
   void Update();
   void Release();

   void SetID(size_t id) { this->id = id; };

private:
   void ProjectFrame(const GLvoid* image, GLsizei width, GLsizei height) const;

   enum MODE { LOST, TRACKING };

   std::string GetLabel(const MODE& mode) const {
      double fps = double(this->count)/((cv::getTickCount() - this->time) / cv::getTickFrequency());
      std::stringstream stream;
      stream.precision(5);
      stream << (mode == LOST ? "LOST " : "TRACKING ") << fps << "fps";
      return stream.str();
   }

private:
   // TODO: Arrumar essa zona!!
   size_t id;
   mutable cv::Mat frame;
   mutable cv::VideoCapture cap;

   mutable SPtr<Camera> cam;
   mutable SPtr<HybridTracker> tracker;

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
   this->app->SetID(this->id);
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
   WindowManager::Destroy(this->id);
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

void Application::AddListener(const SPtr<EventListener>& ltn) {
   SPtr<Window> win = WindowManager::Get(this->id);
   win->AddListener(ltn);
}

cv::Mat Application::Screenshot() {
   char outname[64];
   time_t rawtime = std::time(nullptr);
   std::strftime(outname, 64, "screenshot_%Y%m%d_%H%M%S.jpg", std::localtime(&rawtime));

   Mat screen(480, 640, CV_8UC3);
   glReadPixels(0, 0, 640, 480, GL_BGR, GL_UNSIGNED_BYTE, screen.data);
   cv::flip(screen, screen, 0);

   bool ok = cv::imwrite(string(outname), screen);
   if(ok) cout << "Screenshot captured\n";
   else   cerr << "Did not capture screenshot\n";

   return screen;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*\
*                                                                  Renderer                                                                   *
\*-------------------------------------------------------------------------------------------------------------------------------------------*/

//! Main Loop
void Application::AppRenderer::Render() const {
   // render if I can
   if(this->run && !this->pause) {
      Frame scene;
      this->cap >> scene.image;
      if(scene.image.empty()) return;
      this->frame = scene.image;

      this->count++;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glDepthFunc(GL_LEQUAL);
      glEnable(GL_DEPTH_TEST);

      TMatx proj = this->cam->Projection(0.01, 1000.0);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixd(proj.T().Get().val);

      // computer visio process //
      this->tracker->Update(scene);
      for(auto& marker : this->markers) {
         Matches result = this->tracker->Find(marker, scene);

         if(marker.Lost())
            WindowManager::Get(this->id)->SetLabel(GetLabel(LOST));
         else
            WindowManager::Get(this->id)->SetLabel(GetLabel(TRACKING));

         if(result.size() > 20)
            marker.SetLost(false);
         else marker.SetLost(true);

         if(result.size() >= 4) {
            Coords2D sceneCorners;
            const Coords2D& markerCorners = marker.GetWorld();

            Mat homography = cv::findHomography(result.targetPts(), result.scenePts(), cv::RANSAC, 4);
            cv::perspectiveTransform(markerCorners, sceneCorners, homography);

            Coords3D world = Coords3D(4);
            Point3f center(marker.GetSize().width/2.0, marker.GetSize().height/2.0, 0.0f);
            world[0] = Point3f(-center.x, -center.y, 0.0f);
            world[3] = Point3f(-center.x, +center.y, 0.0f);
            world[2] = Point3f(+center.x, +center.y, 0.0f);
            world[1] = Point3f(+center.x, -center.y, 0.0f);

            TMatx pose = this->cam->Pose(world, sceneCorners, marker.Lost());
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixd(pose.T().Get().val);

            TMatx ipose = pose.Inv();
            float position[] = { float(ipose(0, 3)), float(ipose(1, 3)), float(ipose(2, 3)), 1.0 };
            glLightfv(GL_LIGHT0, GL_POSITION, position);

            glEnable(GL_LIGHTING);
            glEnable(GL_CULL_FACE);
               SPtr<Model> model = marker.GetModel();
               Size3D dims = model->GetDims();

               glTranslatef(0.0f, 0.0f, -dims.height / 2);
               glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
               model->Draw();
            glDisable(GL_LIGHTING);
            glDisable(GL_CULL_FACE);

            if(!sceneCorners.empty()) {
               cv::line(scene.image, sceneCorners[0], sceneCorners[1], cv::Scalar(0, 255, 0), 4);
               cv::line(scene.image, sceneCorners[1], sceneCorners[2], cv::Scalar(0, 255, 0), 4);
               cv::line(scene.image, sceneCorners[2], sceneCorners[3], cv::Scalar(0, 255, 0), 4);
               cv::line(scene.image, sceneCorners[3], sceneCorners[0], cv::Scalar(0, 255, 0), 4);
            }
         }

         for(auto p : result.scenePts()) {
            cv::circle(scene.image, p, 3, cv::Scalar(0, 255, 0), 1);
         }

         Mat flipped;
         cv::flip(scene.image, flipped, 0);
         ProjectFrame(flipped.ptr<GLubyte>(0), (GLsizei) flipped.cols, (GLsizei) flipped.rows);
      }
      // computer visio process end //

      glDisable(GL_DEPTH_TEST);
      glutSwapBuffers ();
   }
}

void Application::AppRenderer::Update() {

}

void Application::AppRenderer::Initialize() {
   // selecionar cor de fundo (preto)
   glClearColor (0.0, 0.0, 0.0, 0.0);

   // setup texture to view video frame
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

   // setup light and material
	glEnable(GL_COLOR_MATERIAL);  // Utiliza cor do objeto como material
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glEnable(GL_LIGHT0);
   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,0.0f);
   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION , 1.0f);
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
