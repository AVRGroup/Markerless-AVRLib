#ifndef GL_INTERFACE_HPP
#define GL_INTERFACE_HPP

#include <GL/glut.h>
#include "Constants.hpp"

class Interface{
   protected:
      int         mainWindow;

   public:
      Interface();
      virtual ~Interface();

      // Inicializa a API GLUT, cria a janela e inicializa callbacks
      void     Init(int argc, char* argv[]);
      // Inicia o loop principal
      void     MainLoop();

      // Get
      inline int GetMainWindow() { return this->mainWindow; }

   protected:
      // Métodos de gerenciamento dos eventos de mouse e teclado
      virtual void   MouseEvent(int button, int state, int x, int y);
      virtual void   MotionEvent(int x, int y);
      virtual void   PassiveMotionEvent(int x, int y);
      virtual void   KeyBoardEvent(unsigned char key, int x, int y);
      virtual void   SpecialKeyEvent(int key, int x, int y);
      virtual void   KeyBoardUpEvent(unsigned char key, int x, int y);
      virtual void   SpecialKeyUpEvent(int key, int x, int y);
      // Métodos de gerenciamento da janela e renderização
      virtual void   Reshape(int width, int height) = 0;
      virtual void   Draw() = 0;

   private:
      // Instância para a subclasse
      static Interface*  instance;

      // Mouse Callbacks
      static void MouseCallback(int, int, int, int);
      static void MotionCallback(int, int);
      static void PassiveMotionCallback(int, int);
      // KeyBoard Callbacks
      static void KeyBoardCallback(unsigned char, int, int);
      static void SpecialKeyCallback(int, int, int);
      static void KeyBoardUpCallback(unsigned char, int, int);
      static void SpecialKeyUpCallback(int, int, int);
      // Display and Window Callbacks
      static void DisplayCallback();
      static void ReshapeCallback(int, int);
};

#endif // GL_INTERFACE_HPP
