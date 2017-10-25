#ifndef _SCENE_MANAGER_HPP_
#define _SCENE_MANAGER_HPP_

#include "SceneGraph.hpp"

class SceneManager
{
public:
   SceneManager(Renderer_t& renderer);

   Node& getWorld();

private:
   Node* createPlatformAABB(vec2_t xw, 
                            vec2_t yw, 
                            const char* fpath, 
                            vec3_t position);

   // Node* createPlatformCircle(float r, 
   //                            const char* fpath, 
   //                            vec3_t position);

   Node* createPlatformTri(vec2_t xw, 
                           vec2_t yw, 
                           const char* fpath, 
                           vec3_t position);

   Renderer_t& renderer_;

   Node scene_;
};

#endif