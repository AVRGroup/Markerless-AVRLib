#ifndef AVR_GLUT_LISTENERS_HPP
#define AVR_GLUT_LISTENERS_HPP

#include <avr/view/Listener.hpp>
#include <avr/view/ogl/Events.hpp>

namespace avr {
namespace GLUT {

//! classes in this file
class /* GLUT:: */AsciiKeyListener;
class /* GLUT:: */NonAsciiKeyListener;
class /* GLUT:: */MouseListener;
class /* GLUT:: */MotionListener;
class /* GLUT:: */MouseWheelListener;
class /* GLUT:: */ReshapeListener;

/////////////////////////////////////////////////// Listeners ///////////////////////////////////////////////////
class AsciiKeyListener : public EventListener {
public:
   virtual ~AsciiKeyListener() {/* dtor */}

   virtual void Listen(const AsciiKeyEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x1;
};

class NonAsciiKeyListener : public EventListener {
public:
   virtual ~NonAsciiKeyListener() {/* dtor */}

   virtual void Listen(const NonAsciiKeyEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x2;
};

class MouseListener : public EventListener {
public:
   virtual ~MouseListener() {/* dtor */}

   virtual void Listen(const MouseEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x3;
};

class MotionListener : public EventListener {
public:
   virtual ~MotionListener() {/* dtor */}

   virtual void Listen(const MotionEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x4;
};

class MouseWheelListener : public EventListener {
public:
   virtual ~MouseWheelListener() {/* dtor */}

   virtual void Listen(const MouseWheelEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x5;
};

class ReshapeListener : public EventListener {
public:
   virtual ~ReshapeListener() {/* dtor */}

   virtual void Listen(const ReshapeEvent&) const = 0;

   /**
    * @override
    * @brief Returns the identification code of this listener
    */
   size_t GetCode() const { return CODE; }

   static const size_t CODE = 0x6;
};

} // namespace GLUT
} // namespace avr

#endif // AVR_GLUT_LISTENERS_HPP
