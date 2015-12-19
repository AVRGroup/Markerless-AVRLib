#ifndef AVR_GLUT_HPP
#define AVR_GLUT_HPP

#include "Window.hpp"
#include <GL/freeglut.h>

namespace avr {
namespace GLUT {

//! classes in this file
// Window
class /* GLUT:: */Window;
// Events
class /* GLUT:: */AsciiKeyEvent;
class /* GLUT:: */NonAsciiKeyEvent;
class /* GLUT:: */MouseEvent;
class /* GLUT:: */MotionEvent;
class /* GLUT:: */MouseWheelEvent;
// Listeners
class /* GLUT:: */AsciiKeyListener;
class /* GLUT:: */NonAsciiKeyListener;
class /* GLUT:: */MouseListener;
class /* GLUT:: */MotionListener;
class /* GLUT:: */MouseWheelListener;

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

   void AddListener(const SPtr<EventListener>& listener);
   void SetRenderer(const SPtr<Renderer>& renderer);

private:
   void Destroy();
   Window(const std::string& label);

private:
   std::string label;

   static struct Initializer {
      Initializer();
   } initialize;

   typedef avr::Window Super;

private:
   // keyboard callbacks
   static void KeyCallback(unsigned char, int, int);
   static void KeyUpCallback(unsigned char, int, int);
   static void SpecialCallback(int, int, int);
   static void SpecialUpCallback(int, int, int);
   // mouse callbacks
   static void MouseCallback(int, int, int, int);
   static void MouseWheelCallback(int, int, int, int);
   static void MotionCallback(int, int);
   static void PassiveMotionCallback(int, int);
   // others callbacks
   static void ReshapeCallback(int, int);
   static void EntryCallback(int);
   static void DisplayCallback();
   static void IdleCallback();
   // default callbacks
   static void DefaultKeyCallback(unsigned char, int, int);
   static void DefaultDisplayCallback();
};

/////////////////////////////////////////////////// Events ///////////////////////////////////////////////////
//! Some Events !//

class AsciiKeyEvent : public Event {
public:
   const unsigned char key;
   const bool pressed;
   const int modifiers;

   AsciiKeyEvent(unsigned char key, bool pressed, int modifiers, size_t id) : Event(id), key(key), pressed(pressed), modifiers(modifiers) { }
};

class NonAsciiKeyEvent : public Event {
public:
   const int  keyCode;
   const bool pressed;
   const int  modifiers;

   NonAsciiKeyEvent(int keyCode, bool pressed, int modifiers, size_t id) : Event(id), keyCode(keyCode), pressed(pressed), modifiers(modifiers) { }
};

class MouseEvent : public Event {
public:
   const int buttom;
   const bool clicked;
   const Point cursor;

   MouseEvent(int buttom, bool clicked, const Point& cursor, size_t id) : Event(id), buttom(buttom), clicked(clicked), cursor(cursor) { }
};

class MotionEvent : public Event {
public:
   const Point cursor;
   const bool clicked;

   MotionEvent(const Point& cursor, bool clicked, size_t id) : Event(id), cursor(cursor), clicked(clicked) { }
};

class MouseWheelEvent : public Event {
public:
   const int direction;
   const int modifiers;

   MouseWheelEvent(int direction, int modifiers, size_t id) : Event(id), direction(direction), modifiers(modifiers) { }
};

/////////////////////////////////////////////////// Listeners ///////////////////////////////////////////////////
//! Some Listeners !//

class AsciiKeyListener : public EventListener {
public:
   virtual ~AsciiKeyListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   size_t GetCode() const { return CODE; }

   static const size_t CODE;
};

class NonAsciiKeyListener : public EventListener {
public:
   virtual ~NonAsciiKeyListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   size_t GetCode() const { return CODE; }

   static const size_t CODE;
};

class MouseListener : public EventListener {
public:
   virtual ~MouseListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   size_t GetCode() const { return CODE; }

   static const size_t CODE;
};

class MotionListener : public EventListener {
public:
   virtual ~MotionListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   size_t GetCode() const { return CODE; }

   static const size_t CODE;
};

class MouseWheelListener : public EventListener {
public:
   virtual ~MouseWheelListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   size_t GetCode() const { return CODE; }

   static const size_t CODE;
};

} // namespace GLUT
} // namespace avr

#endif // AVR_GLUT_HPP
