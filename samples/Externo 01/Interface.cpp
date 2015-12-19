#include <iostream>
#include <cstdlib>
#include "Interface.hpp"

using namespace std;

Interface* Interface::instance = NULL;

Interface::Interface() : pause(false) {}
Interface::~Interface(){}

// Inicializa o GLUT
void Interface::Init(int argc, char* argv[], bool first){
   instance = this;
   instance->pause = first;

   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize($WINDOW_WIDTH, $WINDOW_HEIGHT);
   glutInitWindowPosition($WINDOW_POSX, $WINDOW_POSY);

   mainWindow = glutCreateWindow($WINDOW_LABEL.c_str());

   // Mouse
   glutMouseFunc(MouseCallback);
   glutMotionFunc(MotionCallback);
   glutPassiveMotionFunc(PassiveMotionCallback);
   // Teclado
   glutKeyboardFunc(KeyBoardCallback);
   glutKeyboardUpFunc(KeyBoardUpCallback);
   glutSpecialFunc(SpecialKeyCallback);
   glutSpecialUpFunc(SpecialKeyUpCallback);
   // Janela e Renderização
   if(first) {
      glutDisplayFunc(FirstDisplayCallback);
      glutIdleFunc(FirstDisplayCallback);
   } else {
      glutDisplayFunc(DisplayCallback);
      glutIdleFunc(DisplayCallback);
   }
   glutReshapeFunc(ReshapeCallback);
}

void Interface::Start(){
   glutMainLoop();
}

void Interface::Stop() {
   glutDestroyWindow(this->mainWindow);
   exit(EXIT_SUCCESS);
}

// Mouse Callbacks
void Interface::MouseCallback(int button, int state, int x, int y){
   instance->MouseEvent(button, state, x, y);
}
void Interface::MotionCallback(int x, int y){
   instance->MotionEvent(x, y);
}
void Interface::PassiveMotionCallback(int x, int y){
   instance->PassiveMotionEvent(x, y);
}

// KeyBoard Callbacks
void Interface::KeyBoardCallback(unsigned char key, int x, int y){
   instance->KeyBoardEvent(key, x, y);
}
void Interface::SpecialKeyCallback(int key, int x, int y){
   instance->SpecialKeyEvent(key, x, y);
}
void Interface::KeyBoardUpCallback(unsigned char key, int x, int y){
   instance->KeyBoardUpEvent(key, x, y);
}
void Interface::SpecialKeyUpCallback(int key, int x, int y){
   instance->SpecialKeyUpEvent(key, x, y);
}
// Display and Window Callbacks
void Interface::FirstDisplayCallback(){
   instance->FirstDraw();
   glutDisplayFunc(DisplayCallback);
   glutIdleFunc(DisplayCallback);
}

void Interface::DisplayCallback(){
   if(!instance->pause)
      instance->Draw();
}
void Interface::ReshapeCallback(int w, int h){
   instance->Reshape(w, h);
}

// Métodos de gerenciamento dos eventos de mouse
void Interface::MouseEvent(int button, int state, int x, int y) {}
void Interface::MotionEvent(int x, int y) {}
void Interface::PassiveMotionEvent(int x, int y) {}

// Métodos de gerenciamento dos eventos de teclado
void Interface::KeyBoardEvent(unsigned char key, int x, int y){
   switch(key){
      case $ESC:
         glutDestroyWindow(mainWindow);
         exit(EXIT_SUCCESS);
      break;
   }
}
void Interface::SpecialKeyEvent(int key, int x, int y) {}
void Interface::KeyBoardUpEvent(unsigned char key, int x, int y) {}
void Interface::SpecialKeyUpEvent(int key, int x, int y) {}
