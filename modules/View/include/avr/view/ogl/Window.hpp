#ifndef AVR_GLUT_WINDOW_HPP
#define AVR_GLUT_WINDOW_HPP

#include <GL/freeglut.h>

#include <avr/view/Window.hpp>

namespace avr {
namespace GLUT {

////////////////////////////////////////////////////////////////// Functions ////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Prepares the call to glutMainLoop() and calls it
 * @pre Some window must have been created
 */
void EnterMainLoop();
/**
 * @brief Stops main loop and leaves it, returning to point that StartMainLoop() was called
 * @pre EnterMainLoop() must have been called first
 */
void LeaveMainLoop();

//////////////////////////////////////////////////////////////// GLUT::Window ///////////////////////////////////////////////////////////////////////////////
/**
 * @class GLUT::Window
 */
class Window : public avr::Window {
public:
   /**
    * @class GLUT::Window::Builder
    * @brief Usage to build a GLUT::Window's object
    */
   class Builder : public avr::Window::Builder {
      public:
         Builder(const std::string& label) : used(false), label(label) {/* ctor */}
         SPtr<avr::Window> build() const {
            SPtr<avr::Window> obj = (!used) ? new GLUT::Window(label) : nullptr;
            used = true; return obj;
         }
      private:
         mutable bool used;
         const std::string label;
   };

   Size2i GetSize() const;
   Point2i GetPosition() const;
   std::string GetLabel() const
      { return label; }

   void SetSize(const Size2i&);
   void SetPosition(const Point2i&);
   void SetLabel(const std::string& label);

private:
   Window(const std::string& label);

   void RegistryListener(size_t code);
   void RegistryRenderer();
   void Destroy();

private:
   std::string label;

   static struct Initializer {
      Initializer();
   } initialize;

   typedef avr::Window Super;
};

} // namespace GLUT
} // namespace avr

#endif // AVR_GLUT_WINDOW_HPP
