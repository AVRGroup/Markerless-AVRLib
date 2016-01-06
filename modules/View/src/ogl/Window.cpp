#include <thread>

#include <avr/view/ogl/Window.hpp>
#include <avr/view/ogl/Listeners.hpp>

namespace avr {

using std::string;
using std::vector;

namespace GLUT {
   // keyboard callbacks
   static void KeyCallback(unsigned char, int, int);
   static void KeyUpCallback(unsigned char, int, int);
   static void SpecialCallback(int, int, int);
   static void SpecialUpCallback(int, int, int);
   // mouse callbacks
   static void MouseCallback(int, int, int, int);
   static void MouseWheelCallback(int, int, int, int);
   static void MotionCallback(int, int);
   static void PassiveMotionCallback(int, int);
   // others callbacks
   static void ReshapeCallback(int, int);
//   static void EntryCallback(int);
   static void DisplayCallback();
   static void IdleCallback();
   // default callbacks
   static void DefaultKeyCallback(unsigned char, int, int);
   static void DefaultDisplayCallback();

   void EnterMainLoop() {
      glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
      glutMainLoop();
   }
   void LeaveMainLoop() {
      glutLeaveMainLoop();
   }
} // namespace GLUT

///////////////////////////////////////// GLUT::Window //////////////////////////////////////////////////
GLUT::Window::Initializer GLUT::Window::initialize;

GLUT::Window::Initializer::Initializer() {
   int argc = 0;
   glutInit(&argc, 0x0);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
   glutInitWindowPosition(20, 20);
   glutInitWindowSize(640, 480);
   glutIdleFunc(GLUT::IdleCallback);
}

GLUT::Window::Window(const string& label) : Super(glutCreateWindow(label.c_str())), label(label) {
   /* ctor */
   glutSetWindow(Super::GetID());
   glutKeyboardFunc(GLUT::DefaultKeyCallback);
   glutDisplayFunc(GLUT::DefaultDisplayCallback);
}

void GLUT::Window::Destroy() {
   size_t id = Super::GetID();
   glutDestroyWindow(id);
}

void GLUT::Window::RegistryListener(size_t code) {
   // Define o callback dado o código do listener
   glutSetWindow(Super::GetID());
   switch(code) {
   case AsciiKeyListener::CODE:
      glutKeyboardFunc(GLUT::KeyCallback);
      glutKeyboardUpFunc(GLUT::KeyUpCallback);
      break;
   case NonAsciiKeyListener::CODE:
      glutSpecialFunc(GLUT::SpecialCallback);
      glutSpecialUpFunc(GLUT::SpecialUpCallback);
      break;
   case MouseListener::CODE:
      glutMouseFunc(GLUT::MouseCallback);
      break;
   case MotionListener::CODE:
      glutMotionFunc(GLUT::MotionCallback);
      glutPassiveMotionFunc(GLUT::PassiveMotionCallback);
      break;
   case MouseWheelListener::CODE:
      glutMouseWheelFunc(GLUT::MouseWheelCallback);
      break;
   case ReshapeListener::CODE:
      glutReshapeFunc(GLUT::ReshapeCallback);
      break;
   }
}

void GLUT::Window::RegistryRenderer() {
   // Define o callback
   glutSetWindow(Super::GetID());
   glutDisplayFunc(GLUT::DisplayCallback);
}

Size2i GLUT::Window::GetSize() const {
   return Size2i(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
Point2i GLUT::Window::GetPosition() const {
   return Point2i(glutGet(GLUT_WINDOW_X), glutGet(GLUT_WINDOW_Y));
}

void GLUT::Window::SetSize(const Size2i& s) {
   glutReshapeWindow(s.width, s.height);
}
void GLUT::Window::SetPosition(const Point2i& p) {
   glutPositionWindow(p.x, p.y);
}

void GLUT::Window::SetLabel(const std::string& label) {
   this->label = label;
   glutSetWindowTitle(label.c_str());
}

///////////////////////////////////////////////////////// Callbacks implementation //////////////////////////////////////////////////////////////////
// keyboard callbacks
void GLUT::KeyCallback(unsigned char key, int, int) {
    int id = glutGetWindow();
    const auto& win = WindowManager::Get(id);
    const auto& ltn = SPtr<AsciiKeyListener>(win->GetListener(AsciiKeyListener::CODE));
    ltn->Listen(AsciiKeyEvent(key, true, glutGetModifiers(), id));
}
void GLUT::KeyUpCallback(unsigned char key, int, int) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<AsciiKeyListener>(win->GetListener(AsciiKeyListener::CODE));
   ltn->Listen(AsciiKeyEvent(key, false, glutGetModifiers(), id));
}
void GLUT::SpecialCallback(int keyCode, int, int) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<NonAsciiKeyListener>(win->GetListener(NonAsciiKeyListener::CODE));
   ltn->Listen(NonAsciiKeyEvent(keyCode, true, glutGetModifiers(), id));
}
void GLUT::SpecialUpCallback(int keyCode, int, int) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<NonAsciiKeyListener>(win->GetListener(NonAsciiKeyListener::CODE));
   ltn->Listen(NonAsciiKeyEvent(keyCode, false, glutGetModifiers(), id));
}
// mouse callbacks
void GLUT::MouseCallback(int buttom, int state, int x, int y) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<MouseListener>(win->GetListener(MouseListener::CODE));
   ltn->Listen(MouseEvent(buttom, state == GLUT_DOWN, Point(x, y), id));
}
void GLUT::MouseWheelCallback(int wheel, int direction, int x, int y) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<MouseWheelListener>(win->GetListener(MouseWheelListener::CODE));
   ltn->Listen(MouseWheelEvent(direction, glutGetModifiers(), id));
}
void GLUT::MotionCallback(int x, int y) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<MotionListener>(win->GetListener(MotionListener::CODE));
   ltn->Listen(MotionEvent(Point(x, y), true, id));
}
void GLUT::PassiveMotionCallback(int x, int y) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<MotionListener>(win->GetListener(MotionListener::CODE));
   ltn->Listen(MotionEvent(Point(x, y), false, id));
}
// others callbacks
void GLUT::ReshapeCallback(int w, int h) {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& ltn = SPtr<ReshapeListener>(win->GetListener(ReshapeListener::CODE));
   ltn->Listen(ReshapeEvent(Size2i(w, h), id));
}
void GLUT::DisplayCallback() {
   int id = glutGetWindow();
   const auto& win = WindowManager::Get(id);
   const auto& rdr = win->GetRenderer();
   rdr->Render();
}
void GLUT::IdleCallback() {
   WindowManager::Iterates([] (const avr::Window& win) {
      win.GetRenderer()->Update();
      glutSetWindow(win.GetID());
      glutPostRedisplay();
   });
}
// default callbacks
void GLUT::DefaultKeyCallback(unsigned char key, int, int) {
   if(key == 27) { //ESC
      int id = glutGetWindow();
      WindowManager::Destroy(id);
   }
}

void GLUT::DefaultDisplayCallback() {
   //! CRIAR DEFAULT RENDERER
   SPtr<Window> win = WindowManager::Get(glutGetWindow());
   Size2i size = win->GetSize();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, size.width, size.height);
   gluPerspective(60.0, (double) size.width/size.height, 0.05, 1000.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(25.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glColor3d(1.0, 1.0, 1.0);
   glBegin(GL_QUADS);
      // Plano xy
      glVertex3f(-50.0, 0.0f, -50.0);
      glVertex3f(-50.0, 0.0f, 50.0);
      glVertex3f(50.0, 0.0f, 50.0);
      glVertex3f(50.0, 0.0f, -50.0);
   glEnd();


   glLineWidth(3);
   glColor3d(1.0, 0.6, 0.0);
   glBegin(GL_LINES);
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(100.0, 0.0, 0.0);
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(0.0, 100.0, 0.0);
      glColor3d(0.6, 0.2, 0.0);
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(0.0, 0.0, 100.0);
   glEnd();

   glPushMatrix();
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      const GLfloat lightPos[] = { 0.0, 500.0, 750.0, 1.0 };
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

      glTranslatef(0.0f, 25.0f, 0.0f);
      glutSolidCube(50);

      glDisable(GL_LIGHTING);
   glPopMatrix();

   glDisable(GL_NORMALIZE);
   glDisable(GL_DEPTH_TEST);

   glutSwapBuffers();

}

} // namespace avr
