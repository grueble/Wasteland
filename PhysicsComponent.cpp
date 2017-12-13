#include "PhysicsComponent.hpp"
#include "GameObjs.hpp"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PhysicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PhysicsComponent::PhysicsComponent() :
   volume_(nullptr)
{ }

PhysicsComponent::PhysicsComponent(PhysObj* volume) :
   volume_(volume)
{ }

PhysicsComponent::~PhysicsComponent()
{
   if (volume_)
   {
      delete volume_;
   }
}

PhysicsComponent::PhysicsComponent( PhysicsComponent && other ) //:
   // volume_(nullptr)
{
   *this = std::move(other);
}

PhysicsComponent& PhysicsComponent::operator=( PhysicsComponent && other )
{
   if (this != &other)
   {
      // free any current resources
      delete volume_;

      // pilfer other's resources
      volume_ = other.volume_;

      // reset other
      other.volume_ = nullptr;
   }

   return *this;
}

PhysObj& PhysicsComponent::getVolume()
{
   // need a check here, this function would appear to error for no reason...
   return *volume_;
}

// use this update for bounding nodes??
void PhysicsComponent::update(Entity& obj, std::vector<BoundingNode>& world)
{ }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - ActorPhysicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ActorPhysicsComponent::ActorPhysicsComponent() :
   PhysicsComponent()
{ }

ActorPhysicsComponent::ActorPhysicsComponent(PhysObj* volume) :
   PhysicsComponent(volume)
{ }

ActorPhysicsComponent::ActorPhysicsComponent( ActorPhysicsComponent && other ) //:
   // volume_(nullptr)
{
   *this = std::move(other);
}

ActorPhysicsComponent& ActorPhysicsComponent::operator=( ActorPhysicsComponent && other )
{
   if (this != &other)
   {
      PhysicsComponent::operator=( std::move(other) );
   }

   return *this;
}

void ActorPhysicsComponent::update(Entity& obj, std::vector<BoundingNode>& world)
{
   // add gravity
   if (obj.v.y > -1.0f) // terminal velocity cap
   {
      obj.v.y += _G; // gravitational acceleration
   }

   // add velocity
   obj.p.x += obj.v.x;
   obj.p.y += obj.v.y;

   std::vector<BoundingNode>::iterator it;
   for (it = world.begin(); it != world.end(); ++it)
   {
      std::vector<Manifold> collisions = it->resolveCollision(this->getVolume(), obj.p);

      std::vector<Manifold>::iterator collide_it;
      for (collide_it = collisions.begin(); collide_it != collisions.end(); ++collide_it)
      {
         if (collide_it->resolve)
         {
            obj.v.y = 0.0f;
            obj.p.x += collide_it->proj.x;
            obj.p.y += collide_it->proj.y;
         }
      }
   }
   
   // keep an array of collisions for max # collisions/colliding objects, array of bool? array of manifolds?
   // --> choose the right data to use
}


