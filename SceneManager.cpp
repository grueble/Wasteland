#include "SceneManager.hpp"
#include "InputComponent.hpp"
#include "PhysicsComponent.hpp"
#include "GraphicsComponent.hpp"

#include <iostream>

SceneManager::SceneManager(Renderer_t& renderer) :
   renderer_(renderer),
   scene_()
{ }

Node& SceneManager::getWorld()
{
   scene_.addChild(
      std::move(createPlatformAABB((vec2_t){ 10.0f, 0.0f }, 
                                   (vec2_t){ 0.0f, 5.0f }, 
                                   "./assets/dune_glitch.png",
                                   (vec3_t){ 10.0f, 5.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformTri((vec2_t){ -7.5f, 0.0f }, 
                                  (vec2_t){ 0.0f, -7.5f },
                                  "./assets/aliendesert.png",
                                  (vec3_t){ 0.0f, 15.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformTri((vec2_t){ 2.5f, 0.0f }, 
                                  (vec2_t){ 0.0f, 2.5f },
                                  "./assets/dunepool.png",
                                  (vec3_t){ 0.0f, 10.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformAABB((vec2_t){ 2.5f, 0.0f }, 
                                   (vec2_t){ 0.0f, 2.5f },
                                   "./assets/brklynware.png",
                                   (vec3_t){ -2.5f, 2.5f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformAABB((vec2_t){ 3.0f, 0.0f },
                                   (vec2_t){ 0.0f, 2.0f },
                                   "./assets/basalt.png",
                                   (vec3_t){ 18.0f, 15.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformCircle(4.0f,
                                     "./assets/thepatricianedit.png",
                                     (vec3_t){ 9.0f, 20.0f, 0.0f })));

   return scene_;
}

std::unique_ptr<Entity> SceneManager::getPlayer()
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<ActorPhysicsComponent>(new AABB_t((vec2_t){ 0.5f, 0.0f }, (vec2_t){ 0.0f, 0.5f }));

   std::unique_ptr<Entity> player = 
      std::make_unique<Entity>(
         (vec3_t){ 9.0f, 15.75f, 0.0f },
         std::make_unique<PlayerInputComponent>(),
         std::move(p_comp),
         std::make_unique<WorldGraphicsComponent>(renderer_.createMesh("./assets/player.png", 
                                                  0, /*shader_index*/
                                                  this->getVertices(AABB, p_comp->getVolume()), 
                                                  this->getIndices(AABB))));

   return std::move(player);
}

std::unique_ptr<Node> SceneManager::createPlatformAABB(vec2_t xw, vec2_t yw, 
                                                       const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<PhysicsComponent>(new AABB_t(xw, yw));

   return std::make_unique<Node>(position, std::move(p_comp),
                                 std::make_unique<WorldGraphicsComponent>(
                                    renderer_.createMesh(fpath, 
                                                         0, /*shader_index*/
                                                         this->getVertices(AABB, p_comp->getVolume()), 
                                                         getIndices(AABB))), 
                                 glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

std::unique_ptr<Node> SceneManager::createPlatformCircle(float r, const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<PhysicsComponent>(new Circle(r));

   return std::make_unique<Node>(position, std::move(p_comp), 
                                 std::make_unique<WorldGraphicsComponent>(
                                    renderer_.createMesh(fpath, 
                                                         0, /*shader_index*/
                                                         getVertices(CIRCLE, p_comp->getVolume()), 
                                                         getIndices(CIRCLE))), 
                                 glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

std::unique_ptr<Node> SceneManager::createPlatformTri(vec2_t xw, vec2_t yw,
                                                      const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<PhysicsComponent>(new Triangle(xw, yw));

   return std::make_unique<Node>(position, std::move(p_comp),
                                 std::make_unique<WorldGraphicsComponent>(
                                    renderer_.createMesh(fpath, 
                                                         0, /*shader_index*/
                                                         getVertices(TRI, p_comp->getVolume()), 
                                                         getIndices(TRI))), 
                                 glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

std::vector<float> SceneManager::getVertices(PhysObj_e type, PhysObj& volume)
{
   std::vector<float> vertices;

   switch (type)
   {
      case AABB :
      {
         AABB_t* v = volume.asAABB();
         if (v)
         {
            vertices = {
               // position                    // color           // uv coords
               -(v->xw.x),   v->yw.y,  0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
                 v->xw.x,    v->yw.y,  0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
                 v->xw.x,  -(v->yw.y), 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
               -(v->xw.x), -(v->yw.y), 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
            };
         }
         break;
      }
      case CIRCLE :
      {
         Circle* v = volume.asCircle();
         if (v)
         {
            vertices = {
               // position              // color           // uv coords
               -(v->r),   v->r,  0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
                 v->r,    v->r,  0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
                 v->r,  -(v->r), 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
               -(v->r), -(v->r), 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
            };
         }
         break;
      }
      case TRI :
      {
         Triangle* v = volume.asTriangle();
         if (v)
         {
            vertices = {
               // position                  // color           // uv coords
               0.0f,    2 * v->yw.y, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
               0.0f,    0.0f,        0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
               2 * v->xw.x, 0.0f,    0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
            };
         }
         break;
      }
      default : // case NONE :
      {
         // error msg
         break;
      }
   }

   return vertices;
} 

std::vector<unsigned int> SceneManager::getIndices(PhysObj_e type)
{
   std::vector<unsigned int> indices;

   switch (type)
   {
      case AABB :
      case CIRCLE :
      {
         indices = {
            0, 1, 2, // first triangle
            2, 0, 3  // second triangle
         };
         break;
      }
      case TRI :
      {
         indices = {
            0, 1, 2
         };
         break;
      }
      default : // case NONE :
      {
         // error msg
         break;
      }
   }

   return indices;
}
