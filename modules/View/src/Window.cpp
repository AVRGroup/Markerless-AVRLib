#include <avr/view/Window.hpp>

namespace avr {

using std::vector;

static const uint64_t MASK = 0xF;

////////////////////////////////////////////////// Window ///////////////////////////////////////////////////////
void Window::AddInKey(size_t index, size_t pos) {
   // Armazena o �ndice onde ser� inserido o Listener na chave
   // O �ndice � incrementado em 1 para uso do 0 como flag
   index += 1;
   key = key | (index << (pos << 2));
}

SPtr<Renderer> Window::GetRenderer() const {
   return this->renderer;
}

SPtr<EventListener> Window::GetListener(size_t code) const {
   // Recupera o �ndice do Listener dado seu c�digo
   // O c�digo representa a posi��o do �ndice na chave
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

} // namespace avr
