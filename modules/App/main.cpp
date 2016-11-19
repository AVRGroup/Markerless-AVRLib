#include <iostream>
#include <typeinfo>

#include <avr/Application.hpp>
#include <avr/view/ogl/Window.hpp>
#include <avr/view/ogl/Listeners.hpp>

using namespace avr;

using std::cout;
using std::cerr;
using std::endl;

class Keyboard : public GLUT::AsciiKeyListener {
public:
   void Listen(const GLUT::AsciiKeyEvent& evt) const {
      static bool paused = true;

      if(evt.pressed) {
         switch(evt.key) {
            case Keyboard::ESQ:
               app->Stop();
               break;

            case 's': case 'S':
               app->Screenshot();
               break;

            case ' ':
               (paused) ? app->Resume() : app->Pause();
               paused = !paused;
               break;
         }
      }
   }

   Keyboard(const SPtr<Application>& _app) : app(_app) {/* ctor */}

   mutable SPtr<Application> app;
   static const int ESQ = 27;
};

int main() {
   // configure the augmentation model
   SPtr<Model> model = FactoryModel::OBJ("../data/obj/f-16.obj");
   model->ComputeFacetNormals();
   model->ComputeVertexNormals();

   SystemAlgorithms sift = SystemAlgorithms(new SIFTDetector(1000), new SIFTExtractor, new BruteForceMatcher(cv::NORM_L2), new LucasKanadeAlgorithm);
//   SystemAlgorithms surf = SystemAlgorithms(new SURFDetector, new SURFExtractor, new BruteForceMatcher(cv::NORM_L2), new LucasKanadeAlgorithm);
//   SystemAlgorithms orb  = SystemAlgorithms(new ORBDetector, new ORBExtractor, new BruteForceMatcher(cv::NORM_HAMMING), new LucasKanadeAlgorithm);

   // configure application and build it
   SPtr<Application> app = Application::Builder()
               // define um vídeo pré-gravado para a aplicação
               // caso essa propriedade não seja definida usa-se a webcam
               .video("../data/cormem_scene.mp4")
               // define o arquivo de configuração da câmera
               .camera("../data/camera.yml")
               // define a imagem do marcador e o renderer associado
               .marker("../data/cormem_object.jpg", model)
               // habilita modos de otimização na ordem performance, qualidade
               // se ambas flags são definidas, faz-se otimização de balanceamento
//               .optimize(false, true)
               .algorithms(sift)
               // constroi a aplicação
               .build();

   // compiles and saves the model's in graphic card memory (optimazes rendering)
   model->Compile();

   // set event listeners
   app->AddListener(new Keyboard(app));
   // starts the application
   app->Start();

   return 0;
}
