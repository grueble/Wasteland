#ifndef _SCENE_MANAGER_HPP_
#define _SCENE_MANAGER_HPP_

#include "GameObjs.hpp"

// arbitrary limit
static const int MAX_COMPONENTS = 100;
static const vec3_t START_POSITION = vec3_t( 2.0f, 34.0f, 0.0f );

class SceneManager
{
public:
   SceneManager(Renderer_t& renderer, SDL_Window* window, SDL_GLContext& gl_context);

   ~SceneManager();

   std::vector<BoundingNode>& getWorld();

   Actor& getPlayer();

   Camera& getCamera();

   void loadScene();

   void handleInput(const SDL_Event& e);

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

   // implement an array based stack, use to add menus as a ui layer

   Actor player_;
   Camera camera_;

   // this ownership functionality should be moved to subsystem manager classes at some point
   // InputComponent *i_components_[MAX_COMPONENTS] = {};
   // PhysicsComponent *p_components_[MAX_COMPONENTS] = {};
   // GraphicsComponents *g_components_[MAX_COMPONENTS] = {};
   std::vector<InputComponent*> i_components_;
   std::vector<PhysicsComponent*> p_components_;
   std::vector<GraphicsComponent*> g_components_;
};

#endif