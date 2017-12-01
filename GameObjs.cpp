#include "GameObjs.hpp"
#include "InputComponent.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Entity
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Entity::Entity() :
   world_xform_(glm::mat4(1.0)),
   input_(nullptr),
   physics_(nullptr),
   graphics_(nullptr)
{ }

Entity::~Entity()
{

}

Entity::Entity(vec3_t position,
               std::unique_ptr<InputComponent> input,
               std::unique_ptr<PhysicsComponent> physics, 
               std::unique_ptr<GraphicsComponent> graphics) :
   p(position),
   world_xform_(glm::translate(glm::mat4(), glm::vec3(p.x, p.y, p.z))),
   input_(std::move(input)),
   physics_(std::move(physics)),
   graphics_(std::move(graphics))
{ }

Entity::Entity( Entity&& other )
{
   *this = std::move(other);
}


Entity& Entity::operator=( Entity&& other )
{
   if (this != &other)
   {
      // pilfer other's resources
      p = other.p;
      v = other.v;
      world_xform_ = other.world_xform_;
      input_ = std::move(other.input_); 
      physics_ = std::move(other.physics_);
      graphics_ = std::move(other.graphics_);

      // reset other
      other.input_ = nullptr;
      other.physics_ = nullptr;
      other.graphics_ = nullptr;
   }
   
   return *this;
}

void Entity::update(Node& world, Renderer_t& renderer, glm::mat4 view)
{
   if (input_) {
      input_->update(*this);
   }

   if (physics_) {
      physics_->update(*this, world);
   }

   if (graphics_) {
      world_xform_ = 
         glm::translate(glm::mat4(1.0), glm::vec3(p.x, p.y, 0.0f));

      graphics_->update(renderer, _projection * view * world_xform_);
   }
}

void Entity::resolveCollision(Manifold& m)
{
   if (physics_)
   {
      m.b = &(physics_->getVolume());
      m.b_p = p;
   
      if (collide(m))
      {
         m.resolve = true;
      }
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Node::Node() :
   Entity(),
   parent_(nullptr),
   local_xform_(glm::mat4(1.0))
{ }

Node::Node(vec3_t position,
           std::unique_ptr<PhysicsComponent> physics,
           glm::mat4 local_xform) :
   Entity(position, nullptr, std::move(physics), nullptr),
   parent_(nullptr),
   local_xform_(local_xform)
{ }

Node::Node(vec3_t position,
           std::unique_ptr<GraphicsComponent> graphics,
           glm::mat4 local_xform) :
   Entity(position, nullptr, nullptr, std::move(graphics)),
   parent_(nullptr),
   local_xform_(local_xform)
{ }

Node::Node(vec3_t position,
           std::unique_ptr<PhysicsComponent> physics,
           std::unique_ptr<GraphicsComponent> graphics,
           glm::mat4 local_xform) :
   Entity(position, nullptr, std::move(physics), std::move(graphics)),
   parent_(nullptr),
   local_xform_(local_xform)
{ }

Node::Node(vec3_t position,
           std::unique_ptr<InputComponent> input,
           std::unique_ptr<PhysicsComponent> physics,
           std::unique_ptr<GraphicsComponent> graphics,
           glm::mat4 local_xform) :
   Entity(position, std::move(input), std::move(physics), std::move(graphics)),
   parent_(nullptr),
   local_xform_(local_xform)
{ }

Node::Node( Node&& other )
{
   *this = std::move(other);
}

Node& Node::operator=( Node&& other )
{
   if (this != &other)
   {
      Entity::operator=( std::move(other) );

      // pilfer other's resources
      parent_ = other.parent_;
      local_xform_ = other.local_xform_;
      children_ = std::move(other.children_);
   }
   
   return *this;
}

void Node::addChild(std::unique_ptr<Node> node)
{
   children_.push_back(std::move(node)); // equivalent to emplace_back?
   children_.back()->parent_ = this;
}


void Node::update(Node& world, Renderer_t& renderer, glm::mat4 view)
{
   if (parent_) {
      // this->world_xform_ = parent_->world_xform_ * local_xform_;
   }
   else { // root node
      // this->world_xform_ = local_xform_;
   }

   Entity::update(world, renderer, view);

   std::vector<std::unique_ptr<Node> >::iterator it;
   for (it = children_.begin(); it != children_.end(); ++it)
   {
      (*it)->update(world, renderer, view);
   }
}

void Node::resolveCollision(Manifold& m)
{
   if (physics_)
   {
      m.b = &(physics_->getVolume());
      m.b_p = p;

      // if an obj collides with this Node, project that object out of the collision
      if (collide(m))
      {
         m.resolve = true;
      }
   }
   else 
   {
      std::vector<std::unique_ptr<Node> >::iterator it;
      for (it = children_.begin(); it != children_.end(); ++it)
      {
         (*it)->resolveCollision(m);
      }
   }
}

/* relevant:
 * https://stackoverflow.com/questions/17118256/implementing-move-constructor-by-calling-move-assignment-operator
 * https://stackoverflow.com/questions/22977230/move-constructors-in-inheritance-hierarchy
 */