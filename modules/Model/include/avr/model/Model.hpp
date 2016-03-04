#ifndef AVR_MODEL_HPP
#define AVR_MODEL_HPP

#include <avr/core/Core.hpp>

namespace avr {

class Property {
private:
   Scalarf ambient;
   Scalarf diffuse;
   Scalarf specular;
   // textura, sombreamento, ...
};

class Model {
public:
   virtual ~Model() {/* dtor */}

private:
   std::string name;
   Property property;
   TMatx pose;
   // ...
};

/* // Mudança no Renderer
   class Renderer {
      vector<Model> models;
      void Render() const;
   }

*/

/* // Opção Futura
   class StaticModel : public Model {
      // objetos estáticos
   }

   class AnimationModel : public Model {
      // objetos animados
   }
*/

} // namespace avr

#endif // AVR_MODEL_HPP
