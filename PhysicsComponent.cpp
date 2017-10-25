#include "PhysicsComponent.hpp"
#include <iostream>

PhysicsComponent::~PhysicsComponent()
{
   delete hitbox_;
}

void PhysicsComponent::update()//Node& world)
{

}

PhysObj* PhysicsComponent::getVolume() const
{
   return hitbox_;
}

void PhysicsComponent::setVolume(PhysObj* hitbox)
{
   hitbox_ = hitbox;
}

WorldPhysicsComponent::WorldPhysicsComponent(PhysObj* hitbox)
{
   this->setVolume(hitbox);
}

// VehiclePhysicsComponent::VehiclePhysicsComponent(PhysObj* hitbox)
// {
//    this->setVolume(hitbox);
// }