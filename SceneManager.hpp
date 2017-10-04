#ifndef _SCENE_MANAGER_HPP_
#define _SCENE_MANAGER_HPP_

#include "SceneGraph.hpp"
// #include <glm/gtx/transform.hpp>

// ***NOTE: this should eventually be refactored into a SceneManager
// create scene class to store everything - vbo (coords, colors, +), mvps, etc.
// create scenes in wasteland.cpp
// loadScene and drawScene routines, executed on scenes
// scenes loaded by name... this feels bad, i want more polymorphism

// point3_t eye_position = { 4.0f, 3.0f, 3.0f };
// point3_t look_at_point = { 0.0f, 0.0f, 0.0f };
// vector3_t up_dir = { 0.0f, 1.0f, 0.0f };

SceneNode* loadScene();
ActorNode* loadPlayer();

// class Scene
// {
// public:
//    Scene();

//    ~Scene();

//    void loadScene();

//    void drawScene();

// protected:
//    std::vector<PhysObj*> m_scene_root; // primitive scene graph

//    glm::mat4 m_projection;
//    glm::mat4 m_view;
// };

#endif