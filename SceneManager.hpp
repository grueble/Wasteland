#ifndef _SCENE_MANAGER_HPP_
#define _SCENE_MANAGER_HPP_

#include "GameObjs.hpp"

// might need a Scene here... so that i can thread

class SceneManager
{
public:
   SceneManager(Renderer_t& renderer, SDL_Window* window, SDL_GLContext& gl_context);

   std::vector<BoundingNode>& getWorld();

   Entity& getPlayer();

   void loadScene();

private:
   void createPlatformAABB(vec3_t position,
                           vec2_t xw, 
                           vec2_t yw, 
                           int texture_index, 
                           BoundingNode* parent = nullptr);

   void createPlatformCircle(vec3_t position,
                             float r, 
                             int texture_index,
                             BoundingNode* parent = nullptr);

   void createPlatformTri(vec3_t position,
                          vec2_t xw, 
                          vec2_t yw, 
                          int texture_index,
                          BoundingNode* parent = nullptr);

   void loadDemoTextures();
   void loadDemoObjects();

   Renderer_t& renderer_;
   SDL_Window* window_;
   SDL_GLContext& gl_context_;

   // may just want a root node here, and a vector of projectiles, NPCs, etc.
   // check out "swap and pop" for the unloads
   std::vector<BoundingNode> scene_;

   Entity player_;
};

#endif