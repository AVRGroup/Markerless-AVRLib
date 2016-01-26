#include <algorithm>
#include <iostream>
#include <vector>

#include <avr/view/ogl/Window.hpp>
#include <avr/view/ogl/Listeners.hpp>

using namespace avr;

using std::cout;
using std::endl;

namespace avr {
namespace GLUT {

class KeyListener : public AsciiKeyListener {
public:
   void Listen(const AsciiKeyEvent& evt) const {
      if(evt.pressed) {
         switch(evt.key) {
         case 27:
            WindowManager::Destroy(evt.SOURCE_ID);
            break;
         }
      }
   }
};

class GLRenderer : public Renderer {
private:
   float var, dir;
public:
   GLRenderer() : Renderer(), var(0), dir(1) {/* ctor */}

   void Render() const {
      // Limpar todos os pixels
      glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      glColor3f (1.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
         glVertex2f (-1.0, -1.0);
         glVertex2f (1.0, -1.0);
         glColor3f (1.0, 1.0, 0.0);
         glVertex2f (-1.0, this->var);
      glEnd();
      glutSwapBuffers ();
   }

   void Update() {
      if(this->var <= -1.0 || this->var >= 1.0) this->dir *= -1;
      this->var += this->dir * 0.005;
   }

   void Initialize() {
      // selecionar cor de fundo (preto)
      glClearColor (0.0, 0.0, 0.0, 0.0);

      // inicializar sistema de viz.
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 20.0);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
   }

   void Release() {/* rel */}
};

} } // avr::GLUT

int main(int argc, char* argv[]) {
   for(uint i = 1; i <= 1; i++) {
      SPtr<Window> window = WindowManager::Create(GLUT::Window::Builder(avr::format("Janela %d", i)));
      window->AddListener(new GLUT::KeyListener);
      window->SetRenderer(new GLUT::GLRenderer);
   }

   GLUT::EnterMainLoop();

   return 0;
}
