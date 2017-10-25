#include "SceneManager.hpp"

#include <iostream>

SceneManager::SceneManager(Renderer_t& renderer) :
   renderer_(renderer),
   scene_(Node(NULL, NULL, (vec3_t){ 0.0f, 0.0f, 0.0f }))
{ }

Node& SceneManager::getWorld()
{
   printf("top of getWorld\n");

   scene_.addChild(createPlatformAABB((vec2_t){ 10.0f, 0.0f }, 
                                      (vec2_t){ 0.0f, 5.0f }, 
                                      "./assets/dune_glitch.png",
                                      (vec3_t){ 10.0f, 5.0f, 0.0f }));

   scene_.addChild(createPlatformTri((vec2_t){ -15.0f, 0.0f }, 
                                     (vec2_t){ 0.0f, -15.0f },
                                     "./assets/aliendesert.jpg",
                                     (vec3_t){ 0.0f, 15.0f, 0.0f }));

   scene_.addChild(createPlatformTri((vec2_t){ 5.0f, 0.0f }, 
                                     (vec2_t){ 0.0f, 5.0f },
                                     "./assets/dunepool.jpg",
                                     (vec3_t){ 0.0f, 10.0f, 0.0f }));

   scene_.addChild(createPlatformAABB((vec2_t){ 2.5f, 0.0f }, 
                                      (vec2_t){  0.0f, 2.5f },
                                      "./assets/brklynware.jpg",
                                      (vec3_t){ -2.5f, 2.5f, 0.0f }));
   
   return scene_;
}

Node* SceneManager::createPlatformAABB(vec2_t xw, vec2_t yw, 
                                       const char* fpath, vec3_t position)
{
   PhysicsComponent* p_comp = 
      new WorldPhysicsComponent(new AABB_t((vec2_t){ position.x, position.y }, xw, yw));

   GraphicsComponent* g_comp = 
      new WorldGraphicsComponent(renderer_.createMesh(fpath, 
                                                      0, 
                                                      p_comp->getVolume()->getVertices(), 
                                                      p_comp->getVolume()->getIndices()));

   return new Node(p_comp, g_comp, position);
}

// Node* SceneManager::createPlatformCircle(float r, const char* fpath, vec3_t position)
// {
//    PhysicsComponent* p_comp = 
//       new WorldPhysicsComponent(new Circle((vec2_t){ position.x, position.y }, r));

//    GraphicsComponent* g_comp = 
//       new WorldGraphicsComponent(renderer_.createMesh(fpath, 
//                                                       0, 
//                                                       p_comp->getVolume()->getVertices(), 
//                                                       p_comp->getVolume()->getIndices()));

//    return new Node(p_comp, g_comp, position);
// }

Node* SceneManager::createPlatformTri(vec2_t xw, vec2_t yw, 
                                      const char* fpath, vec3_t position)
{
   PhysicsComponent* p_comp = 
      new WorldPhysicsComponent(new Triangle((vec2_t){ position.x, position.y }, xw, yw));

   GraphicsComponent* g_comp = 
      new WorldGraphicsComponent(renderer_.createMesh(fpath, 
                                                      0, 
                                                      p_comp->getVolume()->getVertices(), 
                                                      p_comp->getVolume()->getIndices()));

   return new Node(p_comp, g_comp, position);
}