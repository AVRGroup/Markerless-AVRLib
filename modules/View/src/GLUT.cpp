#include <avr/view/GLUT.hpp>

namespace avr {

using std::string;
using std::vector;

////////////////// Listerner's code ///////////////////
const size_t GLUT::AsciiKeyListener::CODE    = 1;
const size_t GLUT::NonAsciiKeyListener::CODE = 2;
const size_t GLUT::MouseListener::CODE       = 3;
const size_t GLUT::MotionListener::CODE      = 4;
const size_t GLUT::MouseWheelListener::CODE  = 5;
///////////////////////////////////////////////////////

///////////////////////////////////////// GLUT::Window //////////////////////////////////////////////////
GLUT::Window::Initializer GLUT::Window::initialize;

GLUT::Window::Initializer::Initializer() {
   int argc = 0;
   glutInit(&argc, 0x0);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
   glutInitWindowPosition(20, 20);
   glutInitWindowSize(640, 480);
   glClearColor(0.0, 0.0, 0.0, 0.0);
}

GLUT::Window::Window(const string& label) : Super(glutCreateWindow(label.c_str())), label(label) {
   /* ctor */
   glutSetWindow(Super::GetID());
   glutKeyboardFunc(GLUT::Window::DefaultKeyCallback);
   glutDisplayFunc(GLUT::Window::DefaultDisplayCallback);
}

void GLUT::Window::Destroy() {
   size_t id = Super::GetID();
   glutDestroyWindow(id);
}

void GLUT::Window::AddListener(const SPtr<EventListener>& listener) {
   Super::AddInKey(this->listeners.size(), listener->GetCode());
   Super::listeners.push_back(listener);
   // Define o callback dado o código do listener
   glutSetWindow(Super::GetID());
   switch(listener->GetCode()) {
   case AsciiKeyListener::CODE:
      glutIgnoreKeyRepeat(1);
      glutKeyboardFunc(GLUT::Window::KeyCallback);
      glutKeyboardUpFunc(GLUT::Window::KeyUpCallback);
      break;
   case NonAsciiKeyListener::CODE:
      glutSpecialFunc(GLUT::Window::SpecialCallback);
      glutSpecialUpFunc(GLUT::Window::SpecialUpCallback);
      break;
   case MouseListener::CODE:
      glutMouseFunc(GLUT::Window::MouseCallback);
      break;
   case MotionListener::CODE:
      glutMotionFunc(GLUT::Window::MotionCallback);
      glutPassiveMotionFunc(GLUT::Window::PassiveMotionCallback);
      break;
   case MouseWheelListener::CODE:
      glutMouseWheelFunc(GLUT::Window::MouseWheelCallback);
      break;
   }
}

void GLUT::Window::SetRenderer(const SPtr<Renderer>& renderer) {
   Super::renderer = renderer;
   // Define o callback
   glutSetWindow(Super::GetID());
   glutDisplayFunc(GLUT::Window::DisplayCallback);
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

// keyboard callbacks
void GLUT::Window::KeyCallback(unsigned char key, int, int) {
    int id = glutGetWindow();
    const SPtr<Window>& win = WindowManager::Get(id);
    const SPtr<EventListener>& ltn = win->GetListener(AsciiKeyListener::CODE);
    ltn->Listen(new AsciiKeyEvent(key, true, glutGetModifiers(), id));
}
void GLUT::Window::KeyUpCallback(unsigned char key, int, int) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(AsciiKeyListener::CODE);
   ltn->Listen(new AsciiKeyEvent(key, false, glutGetModifiers(), id));
}
void GLUT::Window::SpecialCallback(int keyCode, int, int) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(NonAsciiKeyListener::CODE);
   ltn->Listen(new NonAsciiKeyEvent(keyCode, true, glutGetModifiers(), id));
}
void GLUT::Window::SpecialUpCallback(int keyCode, int, int) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(NonAsciiKeyListener::CODE);
   ltn->Listen(new NonAsciiKeyEvent(keyCode, false, glutGetModifiers(), id));
}
// mouse callbacks
void GLUT::Window::MouseCallback(int buttom, int state, int x, int y) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(MouseListener::CODE);
   ltn->Listen(new MouseEvent(buttom, state == GLUT_DOWN, Point(x, y), id));
}
void GLUT::Window::MouseWheelCallback(int wheel, int direction, int x, int y) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(MouseWheelListener::CODE);
   ltn->Listen(new MouseWheelEvent(direction, glutGetModifiers(), id));
}
void GLUT::Window::MotionCallback(int x, int y) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(MotionListener::CODE);
   ltn->Listen(new MotionEvent(Point(x, y), true, id));
}
void GLUT::Window::PassiveMotionCallback(int x, int y) {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   const SPtr<EventListener>& ltn = win->GetListener(MotionListener::CODE);
   ltn->Listen(new MotionEvent(Point(x, y), false, id));
}
// others callbacks
void GLUT::Window::ReshapeCallback(int, int) {

}
void GLUT::Window::EntryCallback(int) {

}
void GLUT::Window::DisplayCallback() {
   int id = glutGetWindow();
   const SPtr<Window>& win = WindowManager::Get(id);
   SPtr<Renderer> rdr = win->GetRenderer();
   rdr->Render();
}
void GLUT::Window::IdleCallback() {

}
// default callbacks
void GLUT::Window::DefaultKeyCallback(unsigned char key, int, int) {
   if(key == 27) { //ESC
      int id = glutGetWindow();
      WindowManager::Destroy(id);
   }
}

void GLUT::Window::DefaultDisplayCallback() {
   //! CRIAR DEFUALT RENDERER
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
