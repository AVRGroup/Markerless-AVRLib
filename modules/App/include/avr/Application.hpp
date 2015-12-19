#ifndef AVR_APPLICATION_HPP
#define AVR_APPLICATION_HPP

#include <avr/camera/Camera.hpp>
#include <avr/track/Trackers.hpp>
#include <avr/view/Window.hpp>

namespace avr {

class Application {
public:
   virtual ~Application() {/* dtor */}

   static SPtr<Application> New(size_t videoDevice = 0);
   static SPtr<Application> New(const std::string& videoFile);

   void SetWindow(const Window::Builder& win)
   { this->win = WindowManager::Create(win); }

   void SetCameraFile(const std::string& file)
   { this->cam = new Camera(file); }

   void RegistryMarker(const SPtr<Marker>&);

   void Start();
   void Stop();

   void Pause();
   void Restart();

private:
   class Capturer;
   Application(Capturer&);

   Capturer&    cap;
   SPtr<Window> win;
   SPtr<Camera> cam;

   Set<Marker> markers;

   static SPtr<Application> instance;

};

} // namespace avr

#endif // AVR_APPLICATION_HPP
