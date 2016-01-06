#include <opencv2/highgui/highgui.hpp>

#include <avr/Application.hpp>

namespace avr {

using std::string;

SPtr<Application> Application::instance = nullptr;

class Application::Capturer : public cv::VideoCapture {
public:
   Capturer(size_t device) : cv::VideoCapture(device) { }
   Capturer(const std::string& file) : cv::VideoCapture(file) { }
};

Application::Application(Capturer& cap) : cap(cap) { /* ctor */ }

void Application::Start() {

}

void Application::Stop() {

}

void Application::Pause() {

}

void Application::Restart() {

}

void Application::RegistryMarker(const SPtr<Marker>& mk) {
   this->markers.push_back(mk);
}

SPtr<Application> Application::New(size_t device) {
   if(not Application::instance.Null()) {
      AVR_ERROR(Cod::Undefined, "Recreate the application is invalid! Use Application::New() strictly once");
   }
   Capturer& cap = * new Capturer(device);
   Application::instance = new Application(cap);
   return Application::instance;
}

SPtr<Application> Application::New(const string& file) {
   if(not Application::instance.Null()) {
      AVR_ERROR(Cod::Undefined, "Recreate the application is invalid! Use Application::New() strictly once");
   }
   Capturer& cap = * new Capturer(file);
   Application::instance = new Application(cap);
   return Application::instance;
}

} //
