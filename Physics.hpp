#ifndef _PHYSICS_HPP_
#define _PHYSICS_HPP_

#include <vector>

// gravity const!!!

typedef struct vec2_s vec2_t;
typedef struct vec3_s vec3_t;
typedef struct ray2_s ray2_t;
typedef struct material_s material_t;
typedef struct AABB AABB_t; // wtf whyyyyyyy???

// forward declaration of hitbox types
struct PhysObj;
struct AABB;
struct Circle;
struct Triangle;

struct vec2_s
{
   vec2_s() :
      x(0.0f),
      y(0.0f)
   { }

   vec2_s(float a_x, float a_y) :
      x(a_x),
      y(a_y)
   { }

   float x, y;
};

struct vec3_s
{
   vec3_s() :
      x(0.0f),
      y(0.0f),
      z(0.0f)
   { }

   vec3_s(float a_x, float a_y, float a_z) :
      x(a_x),
      y(a_y),
      z(a_z)
   { }

   float x, y, z;
};

// NOTE: for a projection vector (ray) the magnitude is the penetration depth
struct ray2_s
{
   float mag; // magnitude
   vec2_t dir; // direction; (normalized) unit vector
};

struct material_s
{
   float bounce_coeff;
   float friction_coeff;
};

struct phys_data_s
{
   phys_data_s() :
      vel(),
      mass(0)
   {
   }

   vec2_t vel; // or ray2_t? either way, vel as vector
   // m_position is stored as m_world_xform
   float mass;
};

enum PhysObj_e 
{
   NONE = 0,
   AABB,
   CIRCLE,
   TRI,
   // RECT,
   // LINE
};
const vec2_t X_AXIS = vec2_t(1.0f, 0.0f);
const vec2_t Y_AXIS = vec2_t(0.0f, 1.0f);

// geometric projection functions
void normalize(vec2_t& v);
float dot(const vec2_t& a, const vec2_t& b);
float scalar_proj(const vec2_t& a, const vec2_t& b);
vec2_t vector_proj(const vec2_t& a, const vec2_t& b);
float magnitude(const vec2_t& v);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PhysObj & children
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct PhysObj
{
   PhysObj(PhysObj_e a_type, vec2_t position) :
      type(a_type),
      p(position)
   { }

   virtual AABB_t* asAABB() { return nullptr; }
   virtual Circle* asCircle() { return nullptr; }
   virtual Triangle* asTriangle() { return nullptr; }

   virtual std::vector<float> getVertices() = 0;
   virtual std::vector<unsigned int> getIndices() = 0;

   // returns half length of projection onto given axis
   virtual float projectOntoAxis(vec2_t axis) = 0;

   const PhysObj_e type;
   vec2_t p; // position, same as model matrix xform in SceneNode
};

struct AABB : public PhysObj // axis-aligned bounding box
{
   AABB(vec2_t p, vec2_t a_xw, vec2_t a_yw) :
      PhysObj(PhysObj_e::AABB, p),
      xw(a_xw),
      yw(a_yw)
   { }

   AABB_t* asAABB() { return this; }

   std::vector<float> getVertices() override;
   std::vector<unsigned int> getIndices() override;

   float projectOntoAxis(vec2_t axis) override {
      return scalar_proj(xw, axis) + scalar_proj(yw, axis);
   }

   // x and y halfwidth vectors
   vec2_t xw, yw;
};

struct Circle : public PhysObj
{
   Circle(vec2_t p, float radius) :
      PhysObj(PhysObj_e::CIRCLE, p),
      r(radius)
   { }

   Circle* asCircle() override { return this; }

   std::vector<float> getVertices() override;
   std::vector<unsigned int> getIndices() override;

   float projectOntoAxis(vec2_t axis) override {
      return r;
   }

   float r;
};

struct Triangle : public PhysObj // axis-aligned right triangle
{
   Triangle(vec2_t p, vec2_t a_xw, vec2_t a_yw) :
      PhysObj(PhysObj_e::TRI, p),
      xw(a_xw),
      yw(a_yw)
   { 
      third_axis = vec2_t(xw.x, yw.y);
      normalize(third_axis);
   }

   Triangle* asTriangle() override { return this; }

   std::vector<float> getVertices() override;
   std::vector<unsigned int> getIndices() override;

   float projectOntoAxis(vec2_t axis) override {
      return scalar_proj(xw, axis) + scalar_proj(yw, axis);
   }

   // x and y aligned legs
   vec2_t xw, yw, third_axis;
};

struct Manifold
{
   PhysObj* a;
   PhysObj* b;
   vec2_t proj;
   // float penetration;
   // glm::vec2 normal; 
};


class PhysEngine
{
public:

private:
   bool AABBvsAABB(Manifold* m);
   bool AABBvsCircle(Manifold* m);
   bool CirclevsCircle(Manifold* m);
   bool AABBvsTriangle(Manifold* m);
};

#endif