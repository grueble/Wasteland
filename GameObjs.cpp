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
   // input_(nullptr),
   physics_(nullptr),
   graphics_(nullptr)
{ }

Entity::~Entity()
{ }

Entity::Entity(vec3_t position,
               // InputComponent* input,
               PhysicsComponent* physics, 
               GraphicsComponent* graphics) :
   p(position),
   world_xform_(glm::translate(glm::mat4(), glm::vec3(p.x, p.y, p.z))),
   // input_(input),
   physics_(physics),
   graphics_(graphics)
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
      // input_ = other.input_; 
      physics_ = other.physics_;
      graphics_ = other.graphics_;

      // reset other
      // other.input_ = nullptr;
      other.physics_ = nullptr;
      other.graphics_ = nullptr;
   }
   
   return *this;
}

void Entity::update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4& view)
{
   // k, so this should probably not even update input, allow me to access actor directly.
   // if (input_) {
   //    input_->update(*this);
   // }

   if (physics_) {
      physics_->update(*this, world);
   }

   if (graphics_) {
      world_xform_ = 
         glm::translate(glm::mat4(1.0), glm::vec3(p.x, p.y, 0.0f));

      graphics_->update(renderer, _projection * view * world_xform_);
   }
}

std::vector<Manifold> Entity::resolveCollision(PhysObj& other_volume, vec3_t& other_pos)
{
   std::vector<Manifold> collisions;

   if (physics_)
   {
      // create a Manifold for this collision
      Manifold m(&other_volume, &(physics_->getVolume()), other_pos, p);
   
      if (collide(m))
      {
         collisions.push_back(m);
      }
   }

   return collisions;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Actor
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Actor::Actor(vec3_t position,
             InputComponent* input,
             PhysicsComponent* physics, 
             GraphicsComponent* graphics) :
   Entity(position, physics, graphics),
   input_(input)
{ }

void Actor::update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4& view)
{
   if (input_) {
      input_->update(*this);
   }

   Entity::update(world, renderer, view);
}

void Actor::pushAction(const SDL_Event& e)
{
   input_->pushCommand(e);
}

void Actor::accelerate(float acceleration)
{
   if (std::abs(v.x) < X_VEL_MAX)
   {
      v.x += acceleration;
   }
}

void Actor::jump(float jump_velocity)
{
   if (v.y < Y_VEL_MAX)
   {
      v.y += jump_velocity; 
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - BoundingNode
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BoundingNode::BoundingNode(vec3_t position, 
                           float width, float height) :
   Entity(position, 
          // nullptr, 
          new PhysicsComponent(
            new AABB_t((vec2_t){ width, 0.0f }, (vec2_t){ 0.0f, height })), 
          nullptr)
{ }

BoundingNode::BoundingNode( BoundingNode && other )
{
   *this = std::move(other);
}

BoundingNode & BoundingNode::operator=( BoundingNode && other )
{
   if (this != &other)
   {
      Entity::operator=( std::move(other) );

      // pilfer other's resources
      children_ = other.children_;
   }
   
   return *this;
}

void BoundingNode::update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4& view)
{
   Entity::update(world, renderer, view);

   std::vector<Entity*>::iterator it;
   for (it = children_.begin(); it != children_.end(); ++it)
   {
      (*it)->update(world, renderer, view);
   }
}

std::vector<Manifold> BoundingNode::resolveCollision(PhysObj& other_volume, vec3_t& other_pos)
{
   // no need to check for physics_, BoundingNodes must be constructed with a PhysicsComponent

   std::vector<Manifold> collisions;

   // create a Manifold for this collision
   Manifold m(&other_volume, &(physics_->getVolume()), other_pos, p);

   if (collide(m)) 
   {
      // on a collision with the BB, detect and store collisions for all children 
      std::vector<Entity*>::iterator it;
      for (it = children_.begin(); it != children_.end(); ++it)
      {
         std::vector<Manifold> child_collisions = (*it)->resolveCollision(other_volume, other_pos);

         if (child_collisions.size() > 0)
         {
            std::move(child_collisions.begin(), child_collisions.end(), std::back_inserter(collisions));
         }
      }
   }

   return collisions;
}

void BoundingNode::addChild(Entity* child)
{
   children_.push_back(child);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Camera
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Camera::Camera(vec3_t position, Actor& player) : // can we use a ref to plyr's position?
   Entity(position, 
          // new CameraInputComponent(),
          new PhysicsComponent(new Circle(15.0f)),
          nullptr),
   eye_((vec3_t){ p.x, p.y, p.z + 20.0f },
        (vec3_t){ p.x, p.y, p.z },        
        (vec3_t){ 0.0f, 1.0f, 0.0f }),
   player_(player)
{ 
   // eye_ = eye_point(
   //    vec3_t(p.x, p.y, p.z + 20.0f), // camera position
   //    vec3_t(p.x, p.y, p.z),         // look at point
   //    vec3_t(0.0f, 1.0f, 0.0f)       // up dir
   // );

   // updateView();
}

// void Camera::updateView()
// {
//    view_ = glm::lookAt(
//       glm::vec3(eye_.camera.x, eye_.camera.y, eye_.camera.z),
//       glm::vec3(eye_.look_at.x, eye_.look_at.y, eye_.look_at.z),
//       glm::vec3(eye_.up_dir.x, eye_.up_dir.y, eye_.up_dir.z)
//    );
// }

void Camera::update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4& view)
{
   eye_.camera.x = player_.p.x; 
   eye_.camera.y = player_.p.y; 
   eye_.look_at.x = player_.p.x;
   eye_.look_at.y = player_.p.y;

   view = glm::lookAt(
      glm::vec3(eye_.camera.x, eye_.camera.y, eye_.camera.z),
      glm::vec3(eye_.look_at.x, eye_.look_at.y, eye_.look_at.z),
      glm::vec3(eye_.up_dir.x, eye_.up_dir.y, eye_.up_dir.z)
   );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - SceneNode
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SceneNode::SceneNode(vec3_t position,
//                      std::unique_ptr<InputComponent> input, 
//                      std::unique_ptr<GraphicsComponent> graphics, 
//                      glm::mat4 local_xform) :
//    Entity(
//       position, std::move(input), nullptr, std::move(graphics)),
//    local_xform_(local_xform)
// {

// }

// void SceneNode::update(std::vector<Entity>& world, Renderer_t& renderer, glm::mat4& view)
// {
//    if (parent_) {
//       this->world_xform_ = parent_->world_xform_ * local_xform_;
//    }
//    else { // root node
//       this->world_xform_ = local_xform_;
//    }

//    Entity::update(world, renderer, view);
// }

// void SceneNode::addChild(std::unique_ptr<Entity> child)
// {
//    children_.push_back(std::move(child));
//    children_.back()->parent_ = this;
// }

/* relevant:
 * https://stackoverflow.com/questions/17118256/implementing-move-constructor-by-calling-move-assignment-operator
 * https://stackoverflow.com/questions/22977230/move-constructors-in-inheritance-hierarchy
 */