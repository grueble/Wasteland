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
class Node // NEED copy & move businesss
{
public:
   Node(PhysicsComponent* physics, 
        GraphicsComponent* graphics, 
        vec3_t position);

   ~Node();

   Node( const Node & ) = delete; // not copy-constructable
   Node & operator=( const Node & ) = delete; // not copyable

   Node( Node && ); // move constructor
   Node & operator=( Node && ); // move assignment operator

   void addChild(Node* node);

   void update(Renderer_t& renderer, glm::mat4 view);

protected:
   Node* parent_;
   PhysicsComponent* physics_;
   GraphicsComponent* graphics_;

   vec3_t position_;

   glm::mat4 local_xform_; // translation * rotation * scale
   glm::mat4 world_xform_; // model matrix for vert shader

   std::vector<Node*> children_;
};

#endif