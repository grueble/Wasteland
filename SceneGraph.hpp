#ifndef _SCENE_GRAPH_HPP_
#define _SCENE_GRAPH_HPP_

#include <chrono>

#include "GamePhysics.hpp"
#include "RenderObjects.hpp" // <vector>

typedef std::chrono::duration<double> dsec;

static glm::mat4 m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
// glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - SceneNode & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SceneNode // adaptive binary search tree !!!
{
public:
   SceneNode(RenderObj* mesh, PhysObj* hitbox, glm::mat4 local_xform, glm::mat4 world_xform);

   ~SceneNode();

   virtual void addChild(SceneNode* node);

   virtual void update();

   virtual void render(glm::mat4 view); // only render if m_render_obj != NULL

protected:
   SceneNode* m_parent;
   RenderObj* m_mesh; // all render data & functions
   PhysObj* m_hitbox;

   glm::mat4 m_local_xform; // translation * rotation * scale
   glm::mat4 m_world_xform; // model matrix for vert shader

   std::vector<SceneNode*> m_children;
};

// class GeomNode : public SceneNode // static physical geometry of the scene
// {
// public:
//    GeomNode(RenderObj* mesh, PhysObj* hitbox, glm::mat4 local_xform, glm::mat4 world_xform);

//    ~GeomNode();

//    // void update();

//    // void render(glm::mat4 view);

// protected:
//    material_t m_material;
// };

// class ActorNode : public SceneNode // dynamic physical geometry of the scene
// {
// public:
//    // one option: only one of PC/NPC's nodes is dynamic (top-level)
//    ActorNode(RenderObj* mesh, PhysObj* hitbox, glm::mat4 local_xform, glm::mat4 world_xform, phys_data_s data);

//    ~ActorNode();

//    // void update();

//    // void render(glm::mat4 view);

//    void integrate(dsec a_t, dsec a_dt); // called within update??

// protected:
//    phys_data_s m_phys_data;
// };

#endif