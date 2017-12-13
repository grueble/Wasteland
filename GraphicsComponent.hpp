#ifndef _GRAPHICS_COMPONENT_HPP_
#define _GRAPHICS_COMPONENT_HPP_

/* If I end up doing a DirectX prototype, here would be the 
 * place to handle hardware specfic #defines or #includes... */
#include "OpenGLGraphics.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - GraphicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class GraphicsComponent
{
public:
   GraphicsComponent(Renderer_t& renderer, 
                     int shader_index,
                     int texture_index, 
                     std::vector<float> vertices, 
                     std::vector<unsigned int> indices);

   ~GraphicsComponent();

   GraphicsComponent( GraphicsComponent && other );
   GraphicsComponent & operator=( GraphicsComponent && other );

   void update(Renderer_t& renderer, glm::mat4 mvp);

private:
   Mesh_t mesh_;
   int texture_index_;

   GraphicsComponent( const GraphicsComponent & other ) = delete;
   GraphicsComponent & operator=( const GraphicsComponent & other ) = delete;
};

#endif