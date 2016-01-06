#ifndef AVR_GLUT_EVENTS_HPP
#define AVR_GLUT_EVENTS_HPP

#include <avr/view/Event.hpp>

namespace avr {
namespace GLUT {

//! classes in this file
class /* GLUT:: */AsciiKeyEvent;
class /* GLUT:: */NonAsciiKeyEvent;
class /* GLUT:: */MouseEvent;
class /* GLUT:: */MotionEvent;
class /* GLUT:: */MouseWheelEvent;

/////////////////////////////////////////////////// Events ///////////////////////////////////////////////////
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

class ReshapeEvent : public Event {
public:
   const Size2i size;
   const double ratio;

   ReshapeEvent(const Size2i size, size_t id) : Event(id), size(size), ratio(size.width/(double)size.height) { }
};

} // namespace GLUT
} // namespace avr

#endif // AVR_GLUT_EVENTS_HPP
