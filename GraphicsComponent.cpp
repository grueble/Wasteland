#include "GraphicsComponent.hpp"

void GraphicsComponent::update(Renderer_t& renderer, glm::mat4 mvp)
{
   this->draw(renderer, mvp);
}

WorldGraphicsComponent::WorldGraphicsComponent(int mesh_index) :
   mesh_index_(mesh_index)
{ }

void WorldGraphicsComponent::draw(Renderer_t& renderer, glm::mat4 mvp)
{
   renderer.render(mesh_index_, mvp);
}