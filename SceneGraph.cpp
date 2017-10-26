#include "SceneGraph.hpp"
#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Node::Node() :
   parent_(nullptr),
   physics_(nullptr),
   graphics_(nullptr),
   local_xform_(glm::mat4(1.0))
{ }

Node::Node(std::unique_ptr<PhysicsComponent> physics,
           glm::mat4 local_xform) :
   parent_(nullptr),
   physics_(std::move(physics)),
   graphics_(nullptr),
   local_xform_(local_xform)
{ }

Node::Node(std::unique_ptr<GraphicsComponent> graphics,
           glm::mat4 local_xform) :
   parent_(nullptr),
   physics_(nullptr),
   graphics_(std::move(graphics)),
   local_xform_(local_xform)
{ }

Node::Node(std::unique_ptr<PhysicsComponent> physics, 
           std::unique_ptr<GraphicsComponent> graphics, 
           glm::mat4 local_xform) :
   parent_(nullptr),
   physics_(std::move(physics)),
   graphics_(std::move(graphics)),
   local_xform_(local_xform) // glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)))
{ }

Node::~Node()
{
//    if (physics_) { delete physics_; }
//    if (graphics_) { delete graphics_; }

//    std::vector<Node*>::iterator it;
//    for (it = children_.begin(); it != children_.end(); ++it)
//    {
//       delete *it;
//    }
}

Node::Node( Node&& other ) :
   parent_(nullptr),
   physics_(nullptr),
   graphics_(nullptr),
   local_xform_(glm::mat4(1.0)),
   world_xform_(glm::mat4(1.0))
   // NOTE: update needs to be called to populate world_xform_
{
   *this = std::move(other);
}

// https://msdn.microsoft.com/en-us/library/dd293665.aspx
Node& Node::operator=( Node&& other )
{
   if (this != &other)
   {
      // free any current resources
      // delete physics_; // unnecessary due to std::unique_ptr::operater=
      // delete graphics_; // ^

      // pilfer other's resources
      parent_ = other.parent_;
      physics_ = std::move(other.physics_); // http://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D
      graphics_ = std::move(other.graphics_);
      local_xform_ = other.local_xform_;
      world_xform_ = other.world_xform_;

      children_ = std::move(other.children_); // does this work? O(1)?
   
      other.parent_ = nullptr;
      other.physics_ = nullptr;
      other.graphics_ = nullptr;
      other.local_xform_ = glm::mat4(1.0);
      other.world_xform_ = glm::mat4(1.0);
   }
   
   return *this;
}

void Node::addChild(std::unique_ptr<Node> node)
{
   // node->parent_ = this;
   children_.push_back(std::move(node)); // emplace_back?
   children_.back()->parent_ = this;
}

void Node::update(Renderer_t& renderer, glm::mat4 view)
{
   if (parent_) {
      // this is cool, but possibly unwieldy and unclear (positions all relative)
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
      // one option, generate the world_xform here, but only store the position
      graphics_->update(renderer, projection_ * view);// * world_xform_);
   }

   std::vector<std::unique_ptr<Node> >::iterator it;
   for (it = children_.begin(); it != children_.end(); ++it)
   {
      (*it)->update(renderer, view);
   }
}

WorldNode::WorldNode(std::unique_ptr<PhysicsComponent> physics,
                     glm::mat4 local_xform) :
   Node(std::move(physics), local_xform)
{ }

WorldNode::WorldNode(std::unique_ptr<GraphicsComponent> graphics,
                     glm::mat4 local_xform) :
   Node(std::move(graphics), local_xform)
{ }

WorldNode::WorldNode(std::unique_ptr<PhysicsComponent> physics,
                     std::unique_ptr<GraphicsComponent> graphics,
                     glm::mat4 local_xform) :
   Node(std::move(physics), std::move(graphics), local_xform)
{ }

void WorldNode::resolveCollision(PhysObj* volume, int x, int y, int velocity)
{

}