#ifndef AVR_EVENT_HPP
#define AVR_EVENT_HPP

namespace avr {

/**
 * @class Event Event.hpp <avr/view/Event.hpp>
 * @brief Base class to represent events that occur in a window such as key pressed, mouse clicked and others
 */
class Event {
public:
   Event(size_t id) : SOURCE_ID(id) {/* ctor */}
   virtual ~Event() {/* dtor */};

   const size_t SOURCE_ID;
};

} // namespace avr

#endif // AVR_EVENT_HPP
