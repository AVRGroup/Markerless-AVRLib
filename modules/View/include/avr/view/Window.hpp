#ifndef AVR_WINDOW_HPP
#define AVR_WINDOW_HPP

#include <avr/core/Core.hpp>

#include "EventListener.hpp"
#include "Renderer.hpp"

namespace avr {

class Window;
class WindowManager;

template <class T>
using Set = std::vector<SPtr<T> >;

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

   virtual void AddListener(const SPtr<EventListener>& listener) = 0;
   virtual void SetRenderer(const SPtr<Renderer>& renderer) = 0;

   SPtr<Renderer> GetRenderer() const;
   SPtr<EventListener> GetListener(size_t code) const;

   size_t GetID() const { return id; }
   bool hasListener() const { return (not listeners.empty()); }
   bool hasRenderer() const { return (not renderer.Null()); }

protected:
   Window(size_t id) : renderer(0x0), id(id), key(0x0) {/* ctor */}

   virtual void Destroy() = 0;
   void AddInKey(size_t idx, size_t pos);

   SPtr<Renderer> renderer;
   Set<EventListener> listeners;

private:
   size_t id;
   uint64_t key;

   friend class WindowManager;
};

class WindowManager {
public:
   typedef std::vector<SPtr<Window> >::iterator Iterator;

   static SPtr<Window> Create(const Window::Builder&);
   static void Destroy(SPtr<Window>&);
   static void Destroy(size_t id);

   static size_t NumberOfWindows()        { return count; }
   static SPtr<Window> Get(size_t id)  { return windows[id-1]; }

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
