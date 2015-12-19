#ifndef AVR_RENDERER_HPP
#define AVR_RENDERER_HPP

namespace avr {

class Renderer {
public:
   virtual ~Renderer() {/* dtor */}
   virtual void Render() = 0;
};

} // namespace avr

#endif // AVR_RENDERER_HPP
