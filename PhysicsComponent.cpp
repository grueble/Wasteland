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

void PhysicsComponent::update(Entity& obj, Node& world)
{
   
}

// WorldPhysicsComponent::WorldPhysicsComponent(PhysObj* volume) :
//    PhysicsComponent(volume)
// { }

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

void ActorPhysicsComponent::update(Entity& obj, Node& world)
{
   // add gravity
   if (obj.v.y > -1.0f) // terminal velocity cap
   {
      obj.v.y += _G; // gravitational acceleration
   }

   // add velocity
   obj.p.x += obj.v.x;
   obj.p.y += obj.v.y;

   Manifold m;
   m.a = &(this->getVolume());
   m.a_p = obj.p;

   world.resolveCollision(m);

   if (m.resolve)
   {
      obj.v.y = 0.0f;
   }

   obj.p.x += m.proj.x;
   obj.p.y += m.proj.y;
}