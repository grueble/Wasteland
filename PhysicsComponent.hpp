#ifndef _PHYSICS_COMPONENT_HPP_
#define _PHYSICS_COMPONENT_HPP_

#include "Physics.hpp"

class PhysicsComponent
{
public:
   virtual ~PhysicsComponent();

   void update();//Node& world);

   PhysObj* getVolume() const;

   void setVolume(PhysObj* hitbox);

private:
   // virtual void

   PhysObj* hitbox_;
};

class WorldPhysicsComponent : public PhysicsComponent
{
public:
   WorldPhysicsComponent(PhysObj* hitbox);
};

// class VehiclePhysicsComponent : public PhysicsComponent
// {
// public:
//    VehiclePhysicsComponent(PhysObj* hitbox);
// };

#endif