#ifndef AVR_EVENT_LISTENER_HPP
#define AVR_EVENT_LISTENER_HPP

#include <avr/core/Core.hpp>

namespace avr {

class Event {
public:
   Event(size_t id) : SOURCE_ID(id) { }
   virtual ~Event() {/* dtor */};

   const size_t SOURCE_ID;
};

class EventListener {
public:
   virtual ~EventListener() {/* dtor */}

   virtual void Listen(const SPtr<Event>&) const = 0;
   virtual size_t GetCode() const = 0;
};

} // namespace avr

#endif // AVR_EVENT_LISTENER_HPP
