#ifndef AVR_RENDERER_HPP
#define AVR_RENDERER_HPP

namespace avr {

class Renderer {
public:
   virtual ~Renderer() {/* dtor */}

   /**
    * @brief Performs initial configurations.
    * It is called before to start the renderization loop
    */
   virtual void Initialize() = 0;

   /**
    * @brief Performs the renderization
    * It is called in each iteration of the renderization loop
    */
   virtual void Render() const = 0;

   /**
    * @brief Performs updates in the renderer's state during the renderization loop
    * This method is may be used to create animations. It is called after the Render()
    */
   virtual void Update() = 0;

   /**
    * @brief Performs the cleanup
    * It is called after to stop the renderization loop
    */
   virtual void Release() = 0;
};

} // namespace avr

#endif // AVR_RENDERER_HPP
