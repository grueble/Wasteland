#ifndef _PHYSICS_COMPONENT_HPP_
#define _PHYSICS_COMPONENT_HPP_

#include "Physics.hpp"

// forward declarations
class Entity;
class BoundingNode;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PhysicsComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class PhysicsComponent
{
public:
   PhysicsComponent();

   PhysicsComponent(PhysObj* volume);

   ~PhysicsComponent();

   // move constructor and move-assignment operator
   PhysicsComponent( PhysicsComponent && other );
   PhysicsComponent & operator=( PhysicsComponent && other );

   PhysObj& getVolume();

   virtual void update(Entity& obj, std::vector<BoundingNode>& world);

private:
   PhysObj* volume_;

   // copy constructor and copy-assignment operator
   PhysicsComponent( const PhysicsComponent & other ) = delete; // not copy constructable
   PhysicsComponent & operator=( const PhysicsComponent & other ) = delete; // not copy-assignable
};

class ActorPhysicsComponent : public PhysicsComponent
{
public:
   ActorPhysicsComponent();

   ActorPhysicsComponent(PhysObj* volume);

   // move constructor and move-assignment operator
   ActorPhysicsComponent( ActorPhysicsComponent && other );
   ActorPhysicsComponent & operator=( ActorPhysicsComponent && other );

   void update(Entity& obj, std::vector<BoundingNode>& world) override;

private:
   // copy constructor and copy-assignment operator
   ActorPhysicsComponent( const ActorPhysicsComponent & other ) = delete; // not copy constructable
   ActorPhysicsComponent & operator=( const ActorPhysicsComponent & other ) = delete; // not copy-assignable
};

#endif

/* relevant:
 * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
 */