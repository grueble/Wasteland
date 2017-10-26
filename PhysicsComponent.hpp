#ifndef _PHYSICS_COMPONENT_HPP_
#define _PHYSICS_COMPONENT_HPP_

#include <memory>

#include "Physics.hpp"

class PhysicsComponent
{
public:
   PhysicsComponent(PhysObj* volume);

   ~PhysicsComponent();

   // copy constructor and copy-assignment operator
   WorldPhysicsComponent( const PhysicsComponent & other ) = delete; // not copy constructable
   PhysicsComponent & operator=( const PhysicsComponent & other ) = delete; // not copy-assignable

   // move constructor and move-assignment operator
   PhysicsComponent( PhysicsComponent && other );
   PhysicsComponent & operator=( PhysicsComponent && other );

   PhysObj& getVolume();

   void update();//Node& world);

private:
   PhysicsComponent(); // disable default constructor

   PhysObj* volume_;
};

class WorldPhysicsComponent : public PhysicsComponent
{
public:
   WorldPhysicsComponent(PhysObj* volume);
};

// class VehiclePhysicsComponent : public PhysicsComponent
// {
// public:
//    VehiclePhysicsComponent(PhysObj* volume);
// };

#endif