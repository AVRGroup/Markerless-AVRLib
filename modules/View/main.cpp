#include <algorithm>
#include <iostream>
#include <vector>

#include <avr/view/GLUT.hpp>

using namespace avr;

using std::cout;
using std::endl;

float var = 0;
float dir = 1;
bool idleOn[] = {true, false, false};

void init (void);
void idle();
void draw();

namespace avr {
namespace GLUT {

class KeyListener : public AsciiKeyListener {
public:
   void Listen(const SPtr<Event>& e) const {
      SPtr<AsciiKeyEvent> evt = static_cast<SPtr<AsciiKeyEvent> >(e);
      cout << evt->pressed << endl;
      if(evt->pressed) {
         switch(evt->key) {
         case 27:
            idleOn[evt->SOURCE_ID-1] = false;
            WindowManager::Destroy(evt->SOURCE_ID);
            if(WindowManager::NumberOfWindows() == 0) {
               glutLeaveMainLoop();
            }
         }
      }
   }
};

class GLRenderer : public Renderer {
public:
   void Render() {
      // Limpar todos os pixels
      glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      glColor3f (1.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
         glVertex2f (-1.0, -1.0);
         glVertex2f (1.0, -1.0);
         glColor3f (1.0, 1.0, 0.0);
         glVertex2f (-1.0, var);
      glEnd();
      glutSwapBuffers ();
   }
};

} } // avr::GLUT

int main(int argc, char* argv[]) {
   for(uint i = 1; i <= 1; i++) {
      SPtr<Window> window = WindowManager::Create(GLUT::Window::Builder(avr::format("Janela %d", i)));
      window->AddListener(new GLUT::KeyListener);
      window->SetRenderer(new GLUT::GLRenderer);
   }

   glutIdleFunc(idle);
   init();

   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
   glutMainLoop();

   return 0;
}

void idle()
{
   if(var <= -1.0 || var >= 1.0) dir *= -1;
   var += dir * 0.005;

   for(size_t i = 1; i <= 3; i++) {
      if(!idleOn[i-1]) continue;
      glutSetWindow(i);
      glutPostRedisplay();
   }
}

void init (void)
{
   // selecionar cor de fundo (preto)
   glClearColor (0.0, 0.0, 0.0, 0.0);

   // inicializar sistema de viz.
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 20.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
