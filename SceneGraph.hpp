#ifndef _SCENE_GRAPH_HPP_
#define _SCENE_GRAPH_HPP_

#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"

static glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
// glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Node
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Node
{
public:
   Node();

   Node(std::unique_ptr<PhysicsComponent> physics,
        glm::mat4 local_xform);

   Node(std::unique_ptr<GraphicsComponent> graphics,
        glm::mat4 local_xform);

   Node(std::unique_ptr<PhysicsComponent> physics, 
        std::unique_ptr<GraphicsComponent> graphics, 
        glm::mat4 local_xform);

   ~Node();

   // copy constructor and copy-assignment operator
   Node( const Node & ) = delete; // not copy constructable
   Node & operator=( const Node & ) = delete; // not copy-assignable

   // move constructor and move-assignment operator
   Node( Node && other );
   Node & operator=( Node && other );

   void addChild(std::unique_ptr<Node> node);

   void update(Renderer_t& renderer, glm::mat4 view);

protected:
   // Node(); // disable default constructor

   // vec3_t position_; // this is invalidated by the world_xform_, which contains the position

   Node* parent_;

   std::unique_ptr<PhysicsComponent> physics_;
   std::unique_ptr<GraphicsComponent> graphics_;

   // one option would be to store this stuff in the OpenGLMesh?? or maybe the GraphicsComponent
   glm::mat4 local_xform_; // translation * rotation * scale
   glm::mat4 world_xform_; // model matrix for vert shader

   std::vector<std::unique_ptr<Node> > children_;
};

class WorldNode : public Node
{
public:
   WorldNode(std::unique_ptr<PhysicsComponent> physics,
             glm::mat4 local_xform);

   WorldNode(std::unique_ptr<GraphicsComponent> graphics,
             glm::mat4 local_xform);

   WorldNode(std::unique_ptr<PhysicsComponent> physics, 
             std::unique_ptr<GraphicsComponent> graphics, 
             glm::mat4 local_xform);

   void resolveCollision(PhysObj* volume, int x, int y, int velocity);

private:

};

#endif