#include <iostream>
#include <typeinfo>

#include <avr/Application.hpp>

using namespace avr;

using std::cout;
using std::endl;

//class CubeModel : public Model {
//public:
//   void Draw(const TMatx& pose) {
////      Matx44f camPose = pose.inv();
////      glMatrixMode(GL_MODELVIEW);
////      glLoadMatrixf(pose.t().val);
//   }
//};

int main() {
   // configure application and build it
   SPtr<Application> app = Application::Builder()
               .video("../data/rabin_scene.mp4")
               .camera("../data/camera.yml")
               .marker(PreMarker("../data/rabin_object.jpg", nullptr))
               .optimaze(true, false)
               .build();

   app->Start();

   return 0;
}
