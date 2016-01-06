#ifndef AVR_WINDOW_HPP
#define AVR_WINDOW_HPP

#include <functional>

#include <avr/core/Core.hpp>
#include <avr/view/Listener.hpp>
#include <avr/view/Renderer.hpp>

namespace avr {

//! classes in this file !//
class Window;
class WindowManager;
//!/////////////////////!//

class Window {
public:
   /**
    * @class Window::Builder
    * @brief interface to build Window's objects
    */
   class Builder : public avr::Builder<Window> {
   public:
      virtual ~Builder() {/* dtor */}
      virtual SPtr<Window> build() const = 0;
   };

   virtual ~Window() {/* dtor */}

   virtual Size2i GetSize() const = 0;
   virtual Point2i GetPosition() const = 0;
   virtual void SetSize(const Size2i&) = 0;
   virtual void SetPosition(const Point2i&) = 0;

   void AddListener(const SPtr<EventListener>& listener);
   void SetRenderer(const SPtr<Renderer>& renderer);

   SPtr<Renderer> GetRenderer() const;
   SPtr<EventListener> GetListener(size_t code) const;

   size_t GetID() const { return id; }
   bool hasListener() const { return (not listeners.empty()); }
   bool hasRenderer() const { return (not renderer.Null()); }

protected:
   Window(size_t id) : renderer(0x0), id(id), key(0x0) {/* ctor */}

   virtual void Destroy() = 0;
   virtual void RegistryListener(size_t code) = 0;
   virtual void RegistryRenderer() = 0;

   SPtr<Renderer> renderer;
   Set<EventListener> listeners;

private:
   size_t id;
   uint64_t key;

   friend class WindowManager;
};

class WindowManager {
public:
   static SPtr<Window> Create(const Window::Builder&);
   static void Destroy(SPtr<Window>&);
   static void Destroy(size_t id);

   static size_t NumberOfWindows()     { return count; }
   static SPtr<Window> Get(size_t id)  { return windows[id-1]; }

   /**
    * @brief Iterates the set of windows, call the function passed by parameter for each active window in the set
    * @param func Handle function that receives the current window of the iteration. Possible parameters are: \
    *    \li Pointer to a function \
    *    \code {.cpp}
    *       void myFunc(const Window& w) {\/\* ... \*\/}
    *       // ...
    *       WindowManager::Iterates(myFunc);
    *    \endcode
    *    \li Functor (function object) \
    *    \code {.cpp}
    *       class Functor { void operator()(const Window& w) {\/\* ... \*\/} };
    *       // ...
    *       WindowManager::Iterates(Functor());
    *    \endcode
    *    \li Lambda expression \
    *    \code
    *       WindowManager::Iterates([] (const Window& w) {\/\* ... \*\/});
    *    \endcode
    */
   static void Iterates(const std::function<void(const Window&)>& func);

   WindowManager() = delete;
   WindowManager(const WindowManager&) = delete;
   WindowManager& operator = (const WindowManager&) = delete;

private:
   static size_t count;
   static Set<Window> windows;

   static struct Initializer {
      Initializer();
   } initialize;
};

} // namespace avr

#endif // AVR_WINDOW_HPP
