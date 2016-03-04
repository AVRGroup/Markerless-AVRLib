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

/* // Mudan�a no Renderer
   class Renderer {
      vector<Model> models;
      void Render() const;
   }

*/

/* // Op��o Futura
   class StaticModel : public Model {
      // objetos est�ticos
   }

   class AnimationModel : public Model {
      // objetos animados
   }
*/

} // namespace avr

#endif // AVR_MODEL_HPP
