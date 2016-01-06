#include <avr/view/Window.hpp>

namespace avr {

using std::vector;

static const uint64_t MASK = 0xF;

////////////////////////////////////////////////// Window ///////////////////////////////////////////////////////
void Window::SetRenderer(const SPtr<Renderer>& renderer) {
   this->renderer = renderer;
   this->renderer->Initialize();
   this->RegistryRenderer();
}

SPtr<Renderer> Window::GetRenderer() const {
   return this->renderer;
}

void Window::AddListener(const SPtr<EventListener>& listener) {
   // Armazena o índice onde será inserido o Listener na chave
   // O índice é incrementado em 1 para uso do 0 como flag
   size_t code  = listener->GetCode();
   size_t index = this->listeners.size() + 1;
   this->key = this->key | (index << (code << 2));

   this->listeners.push_back(listener);
   this->RegistryListener(code);
}

SPtr<EventListener> Window::GetListener(size_t code) const {
   // Recupera o índice do Listener dado seu código
   // O código representa a posição do índice na chave
   int index = (key >> (code << 2)) & MASK;
   if(index == 0)
      return nullptr;
   return this->listeners[index-1];
}


////////////////////////////////////////////// WindowManager ////////////////////////////////////////////////////

WindowManager::Initializer WindowManager::initialize;
vector<SPtr<Window> > WindowManager::windows;
size_t WindowManager::count;

WindowManager::Initializer::Initializer() {
   windows.reserve(8);
   count = 0;
}

SPtr<Window> WindowManager::Create(const Window::Builder& win) {
   SPtr<Window> obj = win.build();
   if(not obj.Null()) {
      windows.push_back(obj);
      count++;
   }
   return obj;
}

void WindowManager::Destroy(SPtr<Window>& win) {
   if(not win.Null()) {
      win->Destroy();
      windows[win->GetID()-1] = nullptr; count--;
   }
}

void WindowManager::Destroy(size_t id) {
   SPtr<Window> win = windows[id-1];
   if(not win.Null()) {
      win->Destroy();
      windows[id-1] = nullptr; count--;
   }
}

void WindowManager::Iterates(const std::function<void(const Window&)>& func) {
   for(const auto& win : windows) {
      if(not win.Null())
         func(*win);
   }
}

} // namespace avr
