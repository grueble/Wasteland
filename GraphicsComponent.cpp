#include "GraphicsComponent.hpp"
#include "GameObjs.hpp"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - GraphicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

ActorGraphicsComponent::ActorGraphicsComponent(int mesh_index) :
   mesh_index_(mesh_index)
{ }

void ActorGraphicsComponent::draw(Renderer_t& renderer, glm::mat4 mvp)
{
   // will want a flag here, and corresponding array of flags in Entity
   // obj.world_xform_ = glm::translate(glm::mat4(1.0), )
   // obj.world_xform_ = glm::translate(glm) * obj.world_xform_;

   renderer.render(mesh_index_, mvp);
}