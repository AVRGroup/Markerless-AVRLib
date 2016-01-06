#ifndef AVR_EVENT_LISTENER_HPP
#define AVR_EVENT_LISTENER_HPP

#include <avr/core/Core.hpp>

namespace avr {

/**
 * @class EventListener Listeners.hpp <avr/view/Listeners.hpp>
 * @brief Base class for event listeners such as keyboard, mouse and others
 * Each derivated listener must have a method to listen its event
 */
class EventListener {
public:
   virtual ~EventListener() {/* dtor */}

   /**
    * @brief Returns the identification code of the listener
    * Each derivated listener must have a unique integer code in the inteval [1, 15], so one application does not have more than 15 listeners
    */
   virtual size_t GetCode() const = 0;
};

} // namespace avr

#endif // AVR_EVENT_LISTENER_HPP
