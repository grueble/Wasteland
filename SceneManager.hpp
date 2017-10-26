#ifndef _SCENE_MANAGER_HPP_
#define _SCENE_MANAGER_HPP_

#include "SceneGraph.hpp"
#include <vector>

class SceneManager
{
public:
   SceneManager(Renderer_t& renderer);

   Node& getWorld();

private:
   std::unique_ptr<Node> createPlatformAABB(vec2_t xw, 
                                            vec2_t yw, 
                                            const char* fpath, 
                                            vec3_t position);

   std::unique_ptr<Node> createPlatformCircle(float r, 
                                              const char* fpath, 
                                              vec3_t position);

   std::unique_ptr<Node> createPlatformTri(vec2_t xw, 
                                           vec2_t yw, 
                                           const char* fpath, 
                                           vec3_t position);

   std::vector<float> getVertices(PhysObj_e type, PhysObj& volume);

   std::vector<unsigned int> getIndices(PhysObj_e type);

   Renderer_t& renderer_;

   Node scene_;
};

#endif