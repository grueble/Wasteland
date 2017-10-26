#ifndef _GRAPHICS_COMPONENT_HPP_
#define _GRAPHICS_COMPONENT_HPP_

/* If I end up doing a DirectX prototype, here would be the 
 * place to handle hardware specfic #defines or #includes... */
#include "OpenGLGraphics.hpp"

class GraphicsComponent
{
public:
   // NOTE: Deleting a derived class object using a pointer to a base class 
   //       that has a non-virtual destructor results in undefined behavior
   virtual ~GraphicsComponent() {}

   /* NOTE: Currently, the Renderer owns and manages all OpenGLMesh objects.
    *       If this changes, and this class (or a derived class) comes to own
    *       any resources, I will need to implement move semantics here
    */  

   void update(Renderer_t& renderer, glm::mat4 mvp);

private:
   virtual void draw(Renderer_t& renderer, glm::mat4 mvp) = 0;
};

class WorldGraphicsComponent : public GraphicsComponent
{
public:
   WorldGraphicsComponent(int mesh_index);

private:
   void draw(Renderer_t& renderer, glm::mat4 mvp) override;

   int mesh_index_;
};

#endif