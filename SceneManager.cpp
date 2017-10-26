#include "SceneManager.hpp"

#include <iostream>

SceneManager::SceneManager(Renderer_t& renderer) :
   renderer_(renderer),
   scene_()
{ }

Node& SceneManager::getWorld()
{
   printf("top of getWorld\n");

   scene_.addChild(
      std::move(createPlatformAABB((vec2_t){ 10.0f, 0.0f }, 
                                   (vec2_t){ 0.0f, 5.0f }, 
                                   "./assets/dune_glitch.png",
                                   (vec3_t){ 10.0f, 5.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformTri((vec2_t){ -15.0f, 0.0f }, 
                                  (vec2_t){ 0.0f, -15.0f },
                                  "./assets/aliendesert.jpg",
                                  (vec3_t){ 0.0f, 15.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformTri((vec2_t){ 5.0f, 0.0f }, 
                                  (vec2_t){ 0.0f, 5.0f },
                                  "./assets/dunepool.jpg",
                                  (vec3_t){ 0.0f, 10.0f, 0.0f })));

   scene_.addChild(
      std::move(createPlatformAABB((vec2_t){ 2.5f, 0.0f }, 
                                   (vec2_t){  0.0f, 2.5f },
                                   "./assets/brklynware.jpg",
                                   (vec3_t){ -2.5f, 2.5f, 0.0f })));
   
   return scene_;
}

std::unique_ptr<Node> SceneManager::createPlatformAABB(vec2_t xw, vec2_t yw, 
                                                       const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<WorldPhysicsComponent>(
         new AABB_t((vec2_t){ position.x, position.y }, xw, yw));

   std::unique_ptr<GraphicsComponent> g_comp = 
      std::make_unique<WorldGraphicsComponent>(renderer_.createMesh(fpath, 
                                               0, 
                                               this->getVertices(AABB, p_comp->getVolume()), 
                                               getIndices(AABB)));

   return std::make_unique<WorldNode>(std::move(p_comp), 
                                      std::move(g_comp), 
                                      glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

std::unique_ptr<Node> SceneManager::createPlatformCircle(float r, const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<WorldPhysicsComponent>(
         new Circle((vec2_t){ position.x, position.y }, r));

   std::unique_ptr<GraphicsComponent> g_comp = 
      std::make_unique<WorldGraphicsComponent>(renderer_.createMesh(fpath, 
                                               0, 
                                               getVertices(CIRCLE, p_comp->getVolume()), 
                                               getIndices(CIRCLE)));

   return std::make_unique<WorldNode>(std::move(p_comp), 
                                      std::move(g_comp), 
                                      glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
}

std::unique_ptr<Node> SceneManager::createPlatformTri(vec2_t xw, vec2_t yw, 
                                                      const char* fpath, vec3_t position)
{
   std::unique_ptr<PhysicsComponent> p_comp = 
      std::make_unique<WorldPhysicsComponent>(
         new Triangle((vec2_t){ position.x, position.y }, xw, yw));

   std::unique_ptr<GraphicsComponent> g_comp = 
      std::make_unique<WorldGraphicsComponent>(renderer_.createMesh(fpath, 
                                               0, 
                                               getVertices(TRI, p_comp->getVolume()), 
                                               getIndices(TRI)));

   return std::make_unique<WorldNode>(std::move(p_comp), 
                                      std::move(g_comp), 
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
               // position                          // color           // uv coords
               v->p.x - v->xw.x, v->p.y + v->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
               v->p.x + v->xw.x, v->p.y + v->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
               v->p.x + v->xw.x, v->p.y - v->yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
               v->p.x - v->xw.x, v->p.y - v->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
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
               // position                    // color           // uv coords
               v->p.x - v->r, v->p.y + v->r,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
               v->p.x + v->r, v->p.y + v->r,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
               v->p.x + v->r, v->p.y - v->r,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
               v->p.x - v->r, v->p.y - v->r,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
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
               // position                // color           // uv coords
               v->p.x, v->p.y + v->yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
               v->p.x, v->p.y,            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
               v->p.x + v->xw.x, v->p.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
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
