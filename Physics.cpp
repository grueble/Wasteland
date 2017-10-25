#include <cmath> // "math.h"
#include "Physics.hpp"

// for curved edge tiles, detect VR as a initial test to remove certain cases

// for next time, determine importance of sign in proj vec - do i need to check this?

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PhysObj Collision Functions
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool PhysEngine::AABBvsAABB(Manifold* m)
{
   AABB_t* a = m->a->asAABB();
   AABB_t* b = m->b->asAABB();

   // test for projection overlap along the x-axis
   float dist_x = std::abs(a->p.x - b->p.x);
   float a_proj = scalar_proj(a->xw, X_AXIS); // + scalar_proj(a->yw, X_AXIS);
   float b_proj = scalar_proj(b->xw, X_AXIS); // + scalar_proj(b->yw, X_AXIS);
   float overlap_x = a_proj + b_proj - dist_x;
   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = std::abs(a->p.y - b->p.y);
      a_proj = /* scalar_proj(a->xw, Y_AXIS) + */ scalar_proj(a->yw, Y_AXIS);
      b_proj = /* scalar_proj(b->xw, Y_AXIS) + */ scalar_proj(b->yw, Y_AXIS);
      float overlap_y = a_proj + b_proj - dist_y;
      if (overlap_y > 0)
      {
         // project out of collision along the axis of least overlap
         if (overlap_x < overlap_y) { 
            m->proj = vec2_t(overlap_x, 0); // x-axis is min
         } 
         else {
            m->proj = vec2_t(0, overlap_y); // y-axis is min 
         }

         return true;
      }
   }

   return false;
}

bool PhysEngine::AABBvsCircle(Manifold* m)
{
   AABB_t* a = m->a->asAABB();
   Circle* b = m->b->asCircle();

   // test for projection overlap along the x-axis
   float dist_x = a->p.x - b->p.x;
   float a_proj = scalar_proj(a->xw, X_AXIS); // + scalar_proj(a->yw, X_AXIS);
   float overlap_x = a_proj + b->r - std::abs(dist_x);
   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = a->p.y - b->p.y;
      a_proj = /* scalar_proj(a->xw, Y_AXIS) + */ scalar_proj(a->yw, Y_AXIS); 
      float overlap_y = a_proj + b->r - std::abs(dist_y);
      if (overlap_y > 0)
      {
         // determine voronoi region 
         if (std::abs(dist_x) > a->xw.x && std::abs(dist_y) > a->yw.y) // throw out "edge" regions
         {
            vec2_t nearest_vertex;
            if (dist_x > 0 && dist_y > 0) { // top-right
               nearest_vertex = vec2_t(a->p.x + a->xw.x, a->p.y + a->yw.y);
            } 
            else if (dist_x < 0 && dist_y > 0) { // top-left
               nearest_vertex = vec2_t(a->p.x - a->xw.x, a->p.y + a->yw.y);
            } 
            else if (dist_x < 0 && dist_y < 0) { // bottom-left
               nearest_vertex = vec2_t(a->p.x - a->xw.x, a->p.y - a->yw.y);
            } 
            else if (dist_x > 0 && dist_y < 0) { // bottom-right
               nearest_vertex = vec2_t(a->p.x + a->xw.x, a->p.y - a->yw.y);
            }
            // test for projection overlap along the third separating axis
            float dist_third = 
               pow(b->p.x - nearest_vertex.x, 2.0) + pow(b->p.y - nearest_vertex.y, 2.0);
            float overlap_third = (b->r * b->r) - dist_third; // NOTE: avoid sqrt til forced
            if (overlap_third > 0)
            {
               // project out of collision along the axis of least overlap
               vec2_t third_axis;
               overlap_third = b->r - sqrt(dist_third);
               if (overlap_x < overlap_y) {
                  if (overlap_x < overlap_third) {
                     m->proj = vec2_t(overlap_x, 0); // x- axis is min
                  } 
                  else { // third axis is min
                     third_axis =vec2_t(nearest_vertex.x - b->p.x, nearest_vertex.y - b->p.y);
                     normalize(third_axis);
                     m->proj = 
                        vec2_t(third_axis.x * overlap_third, third_axis.y * overlap_third);
                  }
               } 
               else {
                  if (overlap_y < overlap_third) {
                     m->proj = vec2_t(0, overlap_y); // y-axis is min
                  } 
                  else { // third axis is min
                     third_axis = vec2_t(nearest_vertex.x - b->p.x, nearest_vertex.y - b->p.y);
                     normalize(third_axis);
                     m->proj = 
                        vec2_t(third_axis.x * overlap_third, third_axis.y * overlap_third);
                  }
               }

               return true;
            }
         }
      }
   }

   return false;
}

bool PhysEngine::CirclevsCircle(Manifold* m)
{
   Circle* a = m->a->asCircle();
   Circle* b = m->b->asCircle();

   // test for projection overlap along the separating axis
   float dist_sq = pow(b->p.x + a->p.x, 2.0) + pow(b->p.y + a->p.y, 2.0);
   float overlap = (a->r + b->r) * (a->r + b->r) - dist_sq;
   if (overlap > 0)
   {
      vec2_t axis = vec2_t(a->p.x - b->p.x, a->p.y - b->p.y);
      normalize(axis);
      overlap = a->r + b->r - sqrt(dist_sq);
      m->proj = vec2_t(axis.x * overlap, axis.y * overlap);

      return true;
   }

   return false;
}

bool PhysEngine::AABBvsTriangle(Manifold* m)
{
   AABB_t* a = m->a->asAABB();
   Triangle* b = m->b->asTriangle();

   bool hypotenuse_case = false;

   // test for projection overlap along the x-axis
   float dist_x = a->p.x - b->p.x;
   float a_proj = scalar_proj(a->xw, X_AXIS) + scalar_proj(a->yw, X_AXIS);
   float b_proj, overlap_x;
   // determine the relative position of AABB to triangle on the x-axis
   if ((b->xw.x > 0 && dist_x < 0) || (b->xw.x < 0 && dist_x > 0))
   {
      // hypotenuse opposite from AABB along x-axis
      overlap_x = a_proj - std::abs(dist_x);
   }
   else { // hypotenuse adjacent to AABB along y-axis
      b_proj = scalar_proj(b->xw, X_AXIS) + scalar_proj(b->yw, X_AXIS);
      overlap_x = a_proj + b_proj - std::abs(dist_x);
      hypotenuse_case = true;
   }
   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = std::abs(a->p.y - b->p.y);
      a_proj = scalar_proj(a->xw, Y_AXIS) + scalar_proj(a->yw, Y_AXIS);
      float overlap_y;
      // determine the relative position of AABB to triangle on the y-axis
      if ((b->xw.x > 0 && dist_x < 0) || (b->xw.x < 0 && dist_x > 0)) 
      {
         // hypotenuse opposite from AABB along x-axis
         overlap_y = a_proj - std::abs(dist_y);
      }
      else { // hypotenuse adjacent to AABB along y-axis
         b_proj = scalar_proj(b->xw, Y_AXIS) + scalar_proj(b->yw, Y_AXIS);
         overlap_y = a_proj + b_proj - std::abs(dist_y);
         hypotenuse_case = true;
      }
      if (overlap_y > 0)
      {
         if (!hypotenuse_case)
         {
            // project out of collision along the axis of least overlap
            if (overlap_x < overlap_y) { 
               m->proj = vec2_t(overlap_x, 0); // x-axis is min
            } 
            else {
               m->proj = vec2_t(0, overlap_y); // y-axis is min 
            }
         }
         else // special case for when hypotenuse lies between the two centers
         {
            // test for projection overlap along the 3rd separating axis
            float dist_third = pow(b->p.x - a->p.x, 2.0) + pow(b->p.y - a->p.y, 2.0); 
            a_proj = 
               scalar_proj(a->xw, b->third_axis) + scalar_proj(a->yw, b->third_axis);
            b_proj = scalar_proj(b->xw, b->third_axis); // use either b->xw or b->yw, NOT BOTH
            float overlap_third = (a_proj + b_proj) * (a_proj + b_proj) - dist_third; // delay sqrt
            if (overlap_third > 0)
            {
               // project out of collision along the axis of least overlap
               overlap_third = a_proj + b_proj - sqrt(dist_third);
               if (overlap_x < overlap_y) {
                  if (overlap_x < overlap_third) {
                     m->proj = vec2_t(overlap_x, 0); // x- axis is min
                  } 
                  else { // third axis is min
                     m->proj = 
                        vec2_t(b->third_axis.x * overlap_third, b->third_axis.y * overlap_third);
                  }
               }
               else {
                  if (overlap_y < overlap_third) {
                     m->proj = vec2_t(0, overlap_y); // y-axis is min
                  } 
                  else { // third axis is min
                     m->proj = 
                        vec2_t(b->third_axis.x * overlap_third, b->third_axis.y * overlap_third);
                  }
               }

               return true;
            }
         }
      }
   }

   return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - Geometric Projection Functions
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void normalize(vec2_t& v)
{
   // calculate magnitude
   float mag = magnitude(v);

   // normalize passed vector
   v.x /= mag;
   v.y /= mag; // or should I return a new vector?
}

float dot(const vec2_t& a, const vec2_t& b)
{
   return ( ( a.x * b.x ) + ( a.y * b.y ) );
}

// if magnitude == 1...
float scalar_proj(const vec2_t& a, const vec2_t& b)
{
   /* could add a parameter bool b_is_normal,
    * use to avoid expensive magnitude calculation... */

   return dot(a, b) / magnitude(b);
}

vec2_t vector_proj(const vec2_t& a, const vec2_t& b) // project a onto b
{
   float dp = dot(a, b);

   // if b is normal, len_b_sq == 1
   float len_b_sq = ( b.x * b.x ) + ( b.y *b.y );

   vec2_t projection = vec2_t( (dp / len_b_sq) * b.x, (dp / len_b_sq) * b.y);

   return projection;
}

float magnitude(const vec2_t& v)
{
   return sqrt( ( v.x * v.x ) + ( v.y * v.y ) );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PhysObj Utility Functions
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::vector<float> AABB::getVertices()
{
   std::vector<float> vertices = {
      // position              // color           // uv coords
      p.x - xw.x, p.y + yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      p.x + xw.x, p.y + yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
      p.x + xw.x, p.y - yw.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      p.x - xw.x, p.y - yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   return vertices;
}

std::vector<unsigned int> AABB::getIndices()
{
   std::vector<unsigned int> indices = {
      0, 1, 2, // first triangle
      2, 0, 3  // second triangle
   };

   return indices;
}

std::vector<float> Circle::getVertices()
{
   std::vector<float> vertices = {
      // position        // color           // uv coords
      p.x - r, p.y + r,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      p.x + r, p.y + r,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
      p.x + r, p.y - r,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      p.x - r, p.y - r,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   return vertices;
}

std::vector<unsigned int> Circle::getIndices()
{
   std::vector<unsigned int> indices = {
      0, 1, 2, // first triangle
      2, 0, 3  // second triangle
   };

   return indices;
}

std::vector<float> Triangle::getVertices()
{
   std::vector<float> vertices = {
      // position       // color           // uv coords
      p.x, p.y + yw.y,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      p.x, p.y,         0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
      p.x + xw.x, p.y,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
   };

   return vertices;
}

std::vector<unsigned int> Triangle::getIndices()
{
   std::vector<unsigned int> indices = {
      0, 1, 2
   };

   return indices;
}