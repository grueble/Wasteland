#include <thread>

#include "SceneManager.hpp"
#include "InputComponent.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"

#include <iostream>

SceneManager::SceneManager(Renderer_t& renderer, SDL_Window* window, SDL_GLContext& gl_context) :
   renderer_(renderer),
   window_(window),
   gl_context_(gl_context),
   camera_((vec3_t){ 2.0f, 34.0f, 0.0f })
{ 
   player_ = Entity(
      (vec3_t){ 2.0f, 34.0f, 0.0f },
      std::make_unique<PlayerInputComponent>(),
      std::make_unique<ActorPhysicsComponent>(
         new AABB_t((vec2_t){ 0.5f, 0.0f }, (vec2_t){ 0.0f, 0.5f })),
      std::make_unique<GraphicsComponent>(
         renderer_, 
         0, /*shader_index*/
         renderer_.loadTexture("./assets/player.png"), // 0
         renderer_.getVerticesAABB(0.5f, 0.5f), 
         renderer_.getIndicesAABB()));
}

std::vector<BoundingNode>& SceneManager::getWorld()
{
   return scene_;
}

// change this to Actor&
Entity& SceneManager::getPlayer()
{
   return player_;
}

void SceneManager::loadScene()
{
   // NOTE: OpenGL prefers main thread, cannot call OpenGL functions from two threads at once
   // std::thread t(SceneManager::loadDemoObjects, this);
   // both attachMesh (called during GraphicsComponent initialization and render use glGraphics_ 

   loadDemoTextures();
   loadDemoObjects();

   // if (t.joinable())
   // {
   //    t.join();
   // }
}

void SceneManager::createPlatformAABB(vec3_t position, 
                                      vec2_t xw, vec2_t yw, 
                                      int texture_index,
                                      BoundingNode* parent)
{
   parent->addChild(
      std::make_unique<Entity>(
         position,
         nullptr, // no InputComponent
         std::make_unique<PhysicsComponent>(new AABB_t(xw, yw)),
         std::make_unique<GraphicsComponent>(
            renderer_, 
            0, /*shader_index*/
            texture_index, 
            renderer_.getVerticesAABB(xw.x, yw.y), 
            renderer_.getIndicesAABB())));//,
         // glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

void SceneManager::createPlatformCircle(vec3_t position, float r,  
                                        int texture_index,
                                        BoundingNode* parent)
{
    parent->addChild(
      std::make_unique<Entity>(
         position,
         nullptr, // no InputComponent
         std::make_unique<PhysicsComponent>(new Circle(r)),
         std::make_unique<GraphicsComponent>(
            renderer_, 
            0, /*shader_index*/
            texture_index, 
            renderer_.getVerticesCircle(r), 
            renderer_.getIndicesAABB())));//,
         // glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

void SceneManager::createPlatformTri(vec3_t position,
                                     vec2_t xw, vec2_t yw, 
                                     int texture_index,
                                     BoundingNode* parent)
{
   parent->addChild(
      std::make_unique<Entity>(
         position,
         nullptr, // no InputComponent
         std::make_unique<PhysicsComponent>(new Triangle(xw, yw)),
         std::make_unique<GraphicsComponent>(
            renderer_, 
            0, /*shader_index*/
            texture_index, 
            renderer_.getVerticesTri(xw.x, yw.y), 
            renderer_.getIndicesTri())));//,
         // glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

// loadStartingTextures and backgroundTextureLoad
// ---> for background loading, create an insertion queue that adds any queued events to 
void SceneManager::loadDemoTextures()
{
   renderer_.loadTexture("./assets/dune_glitch.png");       // 1
   renderer_.loadTexture("./assets/aliendesert.png");       // 2
   renderer_.loadTexture("./assets/dunepool.png");          // 3
   renderer_.loadTexture("./assets/brklynware.png");        // 4
   renderer_.loadTexture("./assets/basalt.png");            // 5
   renderer_.loadTexture("./assets/thepatricianedit.png");  // 6
} 

void SceneManager::loadDemoObjects()
{
   // scene_.push_back(Entity());

   // Entity& root = scene_.at(0);

   // Panel 1
   BoundingNode panel_1 = 
      BoundingNode((vec3_t){ 10.0f, 20.0f, 0.0f }, 10.0f, 10.0f);

   createPlatformAABB((vec3_t){ 15.0f, 17.5f, 0.0f },
                      (vec2_t){ 5.0f, 0.0f },
                      (vec2_t){ 0.0f, 2.5f },
                      1, &(panel_1));

   createPlatformTri((vec3_t){ 10.f, 20.f, 0.0f },
                     (vec2_t){ 2.5f, 0.0f },
                     (vec2_t){ 0.0f, 2.5f },
                     2, &(panel_1));

   createPlatformTri((vec3_t){ 10.f, 25.f, 0.0f },
                     (vec2_t){ -5.0f, 0.0f },
                     (vec2_t){ 0.0f, -5.0f },
                     3, &(panel_1));

   // Panel 2
   BoundingNode panel_2 = 
      BoundingNode((vec3_t){ 27.5f, 15.0f, 0.0f }, 7.5f, 15.0f);

   createPlatformAABB((vec3_t){ 27.5f, 7.5f, 0.0f },
                      (vec2_t){ 2.5f, 0.0f },
                      (vec2_t){ 0.0f, 2.5f },
                      1, &(panel_2));

   createPlatformTri((vec3_t){ 20.0f, 15.0f, 0.0f },
                     (vec2_t){ 1.25f, 0.0f },
                     (vec2_t){ 0.0f, 1.25f },
                     2, &(panel_2));

   createPlatformCircle((vec3_t){ 30.f, 20.f, 0.0f },
                        5.0f, 0, &(panel_2));

   createPlatformTri((vec3_t){ 30.0f, 5.0f, 0.0f },
                     (vec2_t){ 2.5f, 0.0f },
                     (vec2_t){ 0.0f, 2.5f },
                     3, &(panel_2));

   // Panel 3
   BoundingNode panel_3 =
      BoundingNode((vec3_t){ 42.5f, 10.0f, 0.0f }, 7.5f, 10.0f);

   createPlatformAABB((vec3_t){ 42.5f, 4.0f, 0.0f },
                      (vec2_t){ 2.5f, 0.0f },
                      (vec2_t){ 0.0f, 1.0f },
                      1, &(panel_3));

   createPlatformTri((vec3_t){ 45.0f, 5.0f, 0.0f },
                     (vec2_t){ -1.25f, 0.0f },
                     (vec2_t){ 0.0f, 2.0f },
                     2, &(panel_3));

   createPlatformTri((vec3_t){ 48.5f, 12.0f, 0.0f },
                     (vec2_t){ -1.25f, 0.0f },
                     (vec2_t){ 0.0f, 2.0f },
                     3, &(panel_3));

   // Panel 4
   BoundingNode panel_4 =
      BoundingNode((vec3_t){ 60.0f, 15.0f, 0.0f }, 10.0f, 15.0f);

   createPlatformCircle((vec3_t){ 60.0f, 10.0, 0.0f },
                        10.0f, 0, &(panel_4));

   createPlatformAABB((vec3_t){ 42.5f, 4.0f, 0.0f },
                      (vec2_t){ 2.5f, 0.0f },
                      (vec2_t){ 0.0f, 1.0f },
                      1, &(panel_4));

   createPlatformAABB((vec3_t){ 42.5f, 4.0f, 0.0f },
                      (vec2_t){ 2.5f, 0.0f },
                      (vec2_t){ 0.0f, 1.0f },
                      1, &(panel_4));

   // Panel 5
   BoundingNode panel_5 =
      BoundingNode((vec3_t){ 85.0f, 15.0f, 0.0f }, 15.0f, 15.0f);

   createPlatformAABB((vec3_t){ 75.0f, 1.25f, 0.0f },
                      (vec2_t){ 5.0f, 0.0f },
                      (vec2_t){ 0.0f, 1.25f },
                      1, &(panel_5));

   createPlatformAABB((vec3_t){ 85.0f, 5.0f, 0.0f },
                      (vec2_t){ 1.5f,   0  },
                      (vec2_t){ 0.0f, 1.0f },
                      1, &(panel_5));

   createPlatformCircle((vec3_t){ 90.0f, 5.0f, 0.0f },
                        5.0f, 0, &(panel_5));

   createPlatformTri((vec3_t){ 90.0f, 20.0f, 0.0f },
                     (vec2_t){ -5.0f, 0.0f },
                     (vec2_t){ 0.0f, -5.0f },
                     3, &(panel_5));

   createPlatformAABB((vec3_t){ 97.5f, 25.0f, 0.0f },
                      (vec2_t){ 2.5, 0.0f },
                      (vec2_t){ 0.0f, 5.0f },
                      1, &(panel_5));

   // Panel 6
   BoundingNode panel_6 =
      BoundingNode((vec3_t){ 100.0f, 40.0f, 0.0f }, 10.0f, 10.0f);

   createPlatformTri((vec3_t){ 100.0f, 30.0f, 0.0f },
                     (vec2_t){ -2.5f, 0.0f },
                     (vec2_t){ 0.0f, 5.0f },
                     2, &(panel_6));

   createPlatformTri((vec3_t){ 100.0f, 35.0f, 0.0f },
                     (vec2_t){ 2.5f, 0.0f },
                     (vec2_t){ 0.0f, 2.5f },
                     3, &(panel_6));

   createPlatformAABB((vec3_t){ 105.0f, 32.5f, 0.0f },
                      (vec2_t){ 5.0f, 0.0f },
                      (vec2_t){ 0.0f, 2.5f },
                      1, &(panel_6));

   createPlatformCircle((vec3_t){ 105.0f, 45.0f, 0.0f },
                        2.5f, 6, &(panel_6));

   scene_.push_back(std::move(panel_1));
   scene_.push_back(std::move(panel_2));
   scene_.push_back(std::move(panel_3));
   scene_.push_back(std::move(panel_4));
   scene_.push_back(std::move(panel_5));
   scene_.push_back(std::move(panel_6));
}