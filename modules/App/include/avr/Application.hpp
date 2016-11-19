#ifndef AVR_APPLICATION_HPP
#define AVR_APPLICATION_HPP

#include <atomic>

#include <avr/core/Core.hpp>
#include <avr/view/Window.hpp>
#include <avr/camera/Camera.hpp>
#include <avr/track/Tracking.hpp>

namespace avr {

/**
 * @class Application Application.hpp <avr/Application.hpp>
 * @brief Manages the augmented reality application for
 */
class Application {
public:
   /**
    * @class Application::Builder Application.hpp <avr/Application.hpp>
    * @brief Configures an avr::Application before build it
    */
   class Builder : avr::Builder<Application> {
   public:
      Builder() : avr::Builder<Application>(), cam(nullptr), path(""), label("AVR Application"), methods(nullptr) {/* ctor */}
      ~Builder() { cam = nullptr; path.clear(); label.clear(); methods = nullptr; markers.clear(); }

      //! sets the avr::Camera object
      Builder& camera(const Camera& cam) {
         this->cam = new Camera(cam);
         return * this;
      }
      //! sets the avr::Camera object via configuration file path
      Builder& camera(const std::string& path) {
         this->cam = new Camera(path);
         return * this;
      }
      //! sets the avr::SystemAlgorithms object
      Builder& algorithms(const SystemAlgorithms& alg) {
         this->methods = new SystemAlgorithms(alg);
         return * this;
      }
      //! sets the avr::SystemAlgorithms object via optimize flags
      Builder& optimize(bool performance, bool quality) {
         this->methods = new SystemAlgorithms(SystemAlgorithms::Create(performance, quality));
         return * this;
      }
      //! adds a prepared marker to build an avr::Marker
      Builder& marker(const PreMarker& mk) {
         this->markers.push_back(mk);
         return * this;
      }
      //! prepares a marker given its image path and renderer model and adds it
      Builder& marker(const std::string& path, const SPtr<Model> model) {
         this->markers.push_back(PreMarker(path, model));
         return * this;
      }
      //! [optional] sets a video file path (if it does not set then uses the web cam)
      Builder& video(const std::string& path) {
         this->path = path;
         return * this;
      }
      //! [optional] sets an application's name that will appear as a window's title
      Builder& name(const std::string& label) {
         this->label = label;
         return * this;
      }
      //! @overwrite
      //! @brief builds a new avr::Application instance
      SPtr<Application> build() const {
         if(!cam or !methods or markers.size() < 1)
            AVR_ERROR(Cod::Undefined, "missing some configurations");
         return new Application(*this);
      }

   private:
      Camera* cam;
      std::string path;
      std::string label;
      SystemAlgorithms* methods;
      std::vector<PreMarker> markers;

      friend class Application;
   };

   virtual ~Application();

   //! starts application's execution
   void Start();
   //! stops application's execution and performs a cleanup
   void Stop();

   //! pauses application's execution
   void Pause();
   //! resumes application's execution after a pause
   void Resume();

   //! adds a new event listener to application's window
   // TODO: Transferir para Builder
   void AddListener(const SPtr<EventListener>&);

   //! takes a screenshot of the application @return the screen image
   cv::Mat Screenshot();

private:
   Application(const Builder&);

private:
   // TODO: Remover essa classe interna
   class AppRenderer;

   size_t id;
   SPtr<AppRenderer> app;
};

} // namespace avr

#endif // AVR_APPLICATION_HPP
