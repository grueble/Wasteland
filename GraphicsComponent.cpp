#include "GraphicsComponent.hpp"
#include "GameObjs.hpp"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - GraphicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GraphicsComponent::GraphicsComponent(Renderer_t& renderer, int shader_index, int texture_index,
                                     std::vector<float> vertices, std::vector<unsigned int> indices)
{
   renderer.attachMesh(mesh_, shader_index, vertices, indices);
   texture_index_ = texture_index;
}

GraphicsComponent::~GraphicsComponent()
{
   glDeleteBuffers(1, &(mesh_.vbo_));
   glDeleteBuffers(1, &(mesh_.ebo_));
   glDeleteVertexArrays(1, &(mesh_.vao_));
}

GraphicsComponent::GraphicsComponent( GraphicsComponent && other )
{
   *this = std::move(other);
}

GraphicsComponent & GraphicsComponent::operator=( GraphicsComponent && other )
{
   if (this != &other)
   {
      // free any current resources
      // glDeleteBuffers(1, &(mesh_it->vbo_)); // invariant: don't overwrite with move
      // glDeleteBuffers(1, &(mesh_it->ebo_)); // might be safer to do this... but slower
      // glDeleteVertexArrays(1, &(mesh_it->vao_));

      // pilfer other's resources
      mesh_ = other.mesh_;
      texture_index_ = other.texture_index_;

      // reset other
      other.mesh_ = OpenGLMesh();
   }

   return *this;
}

void GraphicsComponent::update(Renderer_t& renderer, glm::mat4 mvp)
{
   renderer.render(mesh_, texture_index_, mvp);
}