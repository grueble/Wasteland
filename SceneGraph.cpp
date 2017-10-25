#include "SceneGraph.hpp"
#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Node::Node(PhysicsComponent* physics, 
           GraphicsComponent* graphics, 
           vec3_t position) :
   parent_(NULL),
   physics_(physics),
   graphics_(graphics),
   position_(position),
   local_xform_(glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)))
{ 

}

Node::~Node()
{
   if (physics_) { delete physics_; }
   if (graphics_) { delete graphics_; }

   std::vector<Node*>::iterator it;
   for (it = children_.begin(); it != children_.end(); ++it)
   {
      delete *it;
   }
}

void Node::addChild(Node* node)
{
   children_.push_back(node); // need a move constructor / assignment operator
   node->parent_ = this;
}

void Node::update(Renderer_t& renderer, glm::mat4 view)
{
   if (parent_) {
      world_xform_ = parent_->world_xform_ * local_xform_;
   }
   else { // root node
      world_xform_ = local_xform_;
   }
   if (physics_)
   {
      physics_->update();//world);
   }

   if (graphics_)
   {
      graphics_->update(renderer, projection_ * view);// * world_xform_);
   }

   std::vector<Node*>::iterator it;
   for (it = children_.begin(); it != children_.end(); ++it)
   {
      (*it)->update(renderer, view);
   }
}