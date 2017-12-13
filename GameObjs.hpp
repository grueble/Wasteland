#ifndef _GAME_OBJS_HPP_
#define _GAME_OBJS_HPP_

#include <memory>
#include <vector>
#include <iostream>

#include "Physics.hpp"
#include "OpenGLGraphics.hpp"

// forward declarations
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;
class BoundingNode;

typedef BoundingNode World_t;

static glm::mat4 _projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
// glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

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

   ~Entity();

   // move constructor and move-assignment operator
   Entity( Entity && other );
   Entity & operator=( Entity && other );

   virtual void update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4 view);

   // virtual void resolveCollision(Manifold& m);//PhysObj& other_volume, vec3_t& other_pos);
   virtual std::vector<Manifold> resolveCollision(PhysObj& other_volume, vec3_t& other_pos);

   vec3_t p; // position
   vec2_t v; // velocity
   glm::mat4 world_xform_; // model matrix for vert shader

protected:
   std::unique_ptr<InputComponent> input_;
   std::unique_ptr<PhysicsComponent> physics_;
   std::unique_ptr<GraphicsComponent> graphics_;

private:
   // copy constructor and copy-assignment operator
   Entity( const Entity & ) = delete; // not copy constructable
   Entity & operator=( const Entity & ) = delete; // not copy-assignable
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Camera 
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Camera : public Entity
{
   Camera();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node 
// --> possible to extend this class with built in space partitioning
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Node
{
public:
   virtual ~Node() { }
   virtual void addChild(std::unique_ptr<Entity> child) = 0;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - BoundingNode 
// --> used for: Space Partitioning
// --> refactors: QuadTree
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class BoundingNode : public Entity, Node
{
public:
   BoundingNode(vec3_t position, 
                float width, float height);
                // std::unique_ptr<PhysicsComponent> physics);

   // move constructor and move-assignment operator
   BoundingNode( BoundingNode && other );
   BoundingNode & operator=( BoundingNode && other );

   void update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4 view) override;

   std::vector<Manifold> resolveCollision(PhysObj& other_volume, vec3_t& other_pos) override;

   void addChild(std::unique_ptr<Entity> child) override;

private:
   // children need to be Entities, then we can get bound nodes (need separate resolveCollision algs)
   std::vector<std::unique_ptr<Entity> > children_;

   // copy constructor and copy-assignment operator
   BoundingNode( const BoundingNode & ) = delete; // not copy constructable
   BoundingNode & operator=( const BoundingNode & ) = delete; // not copy-assignable
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - SceneNode
// --> used for: Backgrounds, Animation
// --> extensions: colliding SceneNodes
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// class SceneNode : public Entity, Node
// {
// public:
//    SceneNode(vec3_t position,
//              std::unique_ptr<InputComponent> input, 
//              std::unique_ptr<GraphicsComponent> graphics, 
//              glm::mat4 local_xform);

//    // move constructor and move-assignment operator
//    SceneNode( SceneNode && other );
//    SceneNode & operator=( SceneNode && other );

//    void update(std::vector<BoundingNode>& world, Renderer_t& renderer, glm::mat4 view) override;

//    void addChild(std::unique_ptr<Entity> child) override;

//    // NOTE: updating the top-level world_xform_ updates all child local_xform_'s

// private:
//    SceneNode* parent_ = NULL;

//    // A trick for Nodes...
//    // -> don't set the world_xform_ in the constructor;
//    //    instead, used the local_xform parameter and set the  
//    //    world_xform_ in the update method
//    glm::mat4 local_xform_; // translation * rotation * scale

//    std::vector<std::unique_ptr<SceneNode> > children_;

//    // copy constructor and copy-assignment operator
//    SceneNode( const SceneNode & ) = delete; // not copy constructable
//    SceneNode & operator=( const SceneNode & ) = delete; // not copy-assignable
// };

#endif