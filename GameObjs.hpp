#ifndef _GAME_OBJS_HPP_
#define _GAME_OBJS_HPP_

#include <memory>
#include <vector>
#include <iostream>

#include "Physics.hpp"
#include "OpenGLGraphics.hpp"
// #include "InputComponent.hpp"
// #include "PhysicsComponent.hpp"
// #include "GraphicsComponent.hpp"

// forward declarations
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;
class Node;

static glm::mat4 _projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
// glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

// typedef class Node World_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Entity
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Entity
{
public:
   Entity();

   Entity(vec3_t position,
          std::unique_ptr<InputComponent> input,
          std::unique_ptr<PhysicsComponent> physics, 
          std::unique_ptr<GraphicsComponent> graphics);

   /*virtual*/ ~Entity();

   // move constructor and move-assignment operator
   Entity( Entity && other );
   Entity & operator=( Entity && other );

   virtual void update(Node& world, Renderer_t& renderer, glm::mat4 view);

   virtual void resolveCollision(Manifold& m);//PhysObj& other_volume, vec3_t& other_pos);

   vec3_t p; // position
   vec2_t v; // velocity
   glm::mat4 world_xform_; // model matrix for vert shader

protected:
   std::unique_ptr<InputComponent> input_;
   std::unique_ptr<PhysicsComponent> physics_;
   std::unique_ptr<GraphicsComponent> graphics_;

private:
   // std::unique_ptr<InputComponent> input_;
   // std::unique_ptr<PhysicsComponent> physics_;
   // std::unique_ptr<GraphicsComponent> graphics_;

   // copy constructor and copy-assignment operator
   Entity( const Entity & ) = delete; // not copy constructable
   Entity & operator=( const Entity & ) = delete; // not copy-assignable
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node 
// --> possible to extend this class with built in space partitioning
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Node : public Entity
{
public:
   // root node constructor
   Node();

   // bounding node constructor
   Node(vec3_t position,
        std::unique_ptr<PhysicsComponent> physics,
        glm::mat4 local_xform);

   // scene node constructor
   Node(vec3_t position,
        std::unique_ptr<GraphicsComponent> graphics,
        glm::mat4 local_xform);

   // static world node constructor
   Node(vec3_t position,
        std::unique_ptr<PhysicsComponent> physics, 
        std::unique_ptr<GraphicsComponent> graphics, 
        glm::mat4 local_xform);

   // dynamic world node constructor
   Node(vec3_t position,
        std::unique_ptr<InputComponent> input,
        std::unique_ptr<PhysicsComponent> physics, 
        std::unique_ptr<GraphicsComponent> graphics, 
        glm::mat4 local_xform);

   // move constructor and move-assignment operator
   Node( Node && other );
   Node & operator=( Node && other );

   void addChild(std::unique_ptr<Node> node);

   virtual void update(Node& world, Renderer_t& renderer, glm::mat4 view);

   virtual void resolveCollision(Manifold& m);//PhysObj& other_volume, vec3_t& other_pos);

private:
   Node* parent_;

   // A trick for Nodes...
   // -> don't set the world_xform_ in the constructor;
   //    instead, used the local_xform parameter and set the  
   //    world_xform_ in the update method
   glm::mat4 local_xform_; // translation * rotation * scale
   // glm::mat4 world_xform_; // model matrix for vert shader

   std::vector<std::unique_ptr<Node> > children_;

   // copy constructor and copy-assignment operator
   Node( const Node & ) = delete; // not copy constructable
   Node & operator=( const Node & ) = delete; // not copy-assignable
};

#endif