#include "PhysicsComponent.hpp"
#include <iostream>

PhysicsComponent::PhysicsComponent(PhysObj* volume) :
   volume_(volume)
{ }

PhysicsComponent::~PhysicsComponent()
{
   delete volume_;
}

PhysicsComponent::PhysicsComponent( PhysicsComponent && other ) :
   volume_(nullptr)
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
   return *volume_;
}

void PhysicsComponent::update()//Node& world)
{

}

WorldPhysicsComponent::WorldPhysicsComponent(PhysObj* volume) :
   PhysicsComponent(volume)
{ }

// VehiclePhysicsComponent::VehiclePhysicsComponent(PhysObj* volume)
// {
//    this->setVolume(volume);
// }