#include "SceneGraph.hpp"
#include <iostream>

SceneNode::SceneNode(RenderObj* mesh,
                     PhysObj* hitbox) : // , 
                     // glm::mat4 local_xform,
                     // glm::mat4 world_xform) :
   m_mesh(mesh),
   m_hitbox(hitbox) //,
   // m_local_xform(local_xform),
   // m_world_xform(world_xform)
{
   m_parent = NULL;
}

SceneNode::~SceneNode()
{
   if (m_mesh) { delete m_mesh; }
   if (m_hitbox) { delete m_hitbox; }

   std::vector<SceneNode*>::iterator it;
   for (it = m_children.begin(); it != m_children.end(); ++it)
   {
      delete *it;
   }
}

void SceneNode::addChild(SceneNode* node)
{
   m_children.push_back(node);
   node->m_parent = this;
}

void SceneNode::update()
{
   if (m_parent) {
      m_world_xform = m_parent->m_world_xform * m_local_xform;
   }
   else { // root node
      m_world_xform = m_local_xform;
   }

   if (m_hitbox) {
      // generate / update hitbox position
   }

   std::vector<SceneNode*>::iterator it;
   for (it = m_children.begin(); it != m_children.end(); ++it)
   {
      (*it)->update();
   }
}

void SceneNode::render(glm::mat4 view) // basic rendering on GL tris / elements
{
   // do collide with world projection / screen projection here. VIEW CULLING V_V

   if (m_mesh) 
   {
      m_mesh->render(m_projection * view * m_world_xform);
   }

   if (m_children.size() > 0)
   {
      std::vector<SceneNode*>::iterator it;
      for (it = m_children.begin(); it != m_children.end(); ++it)
      {
         (*it)->render(view);
      }
   }
}

void SceneNode::collide(SceneNode* node)
{
   if (node->m_hitbox)
   {

   }
   else
   {
      std::vector<SceneNode*>::iterator it;
      for (it = m_children.begin(); it != m_children.end(); ++it)
      {
         // (*it)->collide
      }
   }
}

GeomNode::GeomNode(RenderObj* mesh,
                   PhysObj* hitbox) : // , 
                   // glm::mat4 local_xform,
                   // glm::mat4 world_xform) : // ,
                   // material_t material) :
   SceneNode(mesh, hitbox) //, local_xform, world_xform) // ,
   // m_material(material)
{

}

GeomNode::~GeomNode()
{

}

void GeomNode::update()
{
   SceneNode::update();
}

void GeomNode::render(glm::mat4 view)
{
   SceneNode::render(view);
}

void GeomNode::collide(SceneNode* node)
{

}

ActorNode::ActorNode(RenderObj* mesh,
                     PhysObj* hitbox) : // , 
                     // glm::mat4 local_xform,
                     // glm::mat4 world_xform) : // ,
                     // phys_data_s data) :
   SceneNode(mesh, hitbox), //, local_xform, world_xform), // ,
   m_phys_data()
{

}

ActorNode::~ActorNode()
{

}

void ActorNode::update()
{
   SceneNode::update();
}

void ActorNode::render(glm::mat4 view)
{
   SceneNode::render(view);
}

void ActorNode::collide(SceneNode* node)
{

}

void ActorNode::integrate(dsec a_t, dsec a_dt)
{

}