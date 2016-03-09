#include <iostream>
#include <typeinfo>

#include <avr/Application.hpp>
#include <avr/view/ogl/Window.hpp>

using namespace avr;

using std::cout;
using std::endl;

class CubeModel : public Model {
public:
   //! @overwrite
   void Draw(const TMatx& pose) {
      glColor3d(0, 0.6, 1.0);
      glPushMatrix();
         glEnable(GL_LIGHTING);
         glEnable(GL_LIGHT0);
         setupLight(pose.Inv());

         glTranslatef(0.0f, 0.0f, -25.0f);
         glutSolidCube(50);

         glDisable(GL_LIGHTING);
      glPopMatrix();
   }

   void setupLight(const TMatx& pose){
      static int  mat_f = 1;
      GLfloat     mat_amb_diff[]  = {0.9, 0.9, 0.0, 1.0};
      GLfloat     mat_specular[]  = {0.5, 0.5, 0.5, 1.0};
      GLfloat     mat_shininess[] = {10.0};
      GLfloat     light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
      GLfloat     light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat     light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat     light_position[] = { float(pose(0, 3)), float(pose(1, 3)), float(pose(2, 3)), 1.0 };

      if( mat_f )
      {
         glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
         glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
         glEnable(GL_COLOR_MATERIAL);
         mat_f = 0;
      }

      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   }
};

int main() {
   // configure application and build it
   SPtr<Application> app = Application::Builder()
               // define um vídeo pré-gravado para a aplicação
               // caso essa propriedade não seja definida usa-se a webcam
               .video("../data/rabin_scene.mp4")
               // define o arquivo de configuração da câmera
               .camera("../data/camera.yml")
               // define a imagem do marcador e o renderer associado
               .marker("../data/rabin_object.jpg", new CubeModel)
               // habilita modos de otimização na ordem performance, qualidade
               // se ambas flags são definidas, faz-se otimização de balanceamento
               .optimaze(true, false)
               // constroi a aplicação
               .build();

   app->Start();

   return 0;
}
