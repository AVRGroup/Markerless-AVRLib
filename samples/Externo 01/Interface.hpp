#ifndef GL_INTERFACE_HPP
#define GL_INTERFACE_HPP

#include <GL/freeglut.h>
#include <GL/glext.h>
#include "Constants.hpp"

class Interface{
   protected:
      int   mainWindow;
      bool  pause;

   public:
      Interface();
      virtual ~Interface();

      // Inicializa a API GLUT, cria a janela e inicializa callbacks
      // @param enableFirstDraw habilita chamada de método exclusivo para o primeiro frame
      //       neste caso pause é setado para true por padrão
      void     Init(int argc, char* argv[], bool enableFirstDraw = false);
      // Inicia o loop principal
      void     Start();
      // Encerra o loop
      void     Stop();

      // Set
      inline void Pause() { instance->pause = true; }
      inline void Restart() { instance->pause = false; }

      // Get
      inline int GetMainWindow() const { return this->mainWindow; }
      inline bool isPaused() const { return this->pause; }

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
      virtual void   FirstDraw() = 0;
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
      static void FirstDisplayCallback();
      static void ReshapeCallback(int, int);
};

#endif // GL_INTERFACE_HPP
