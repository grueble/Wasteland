#include <cmath> // "math.h"
#include "Physics.hpp"

// for curved edge tiles, detect VR as a initial test to remove certain cases

// for next time, determine importance of sign in proj vec - do i need to check this?

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
// - 2D Collision Functions
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool collide(Manifold& m)
{
   bool res = false;
   switch (m.a->getType())
   {
      case AABB :
      {
         switch (m.b->getType())
         {
            case AABB :
            {
               res = AABBvsAABB(m);
               break;
            }
            case CIRCLE :
            {
               res = AABBvsCircle(m);
               break;
            }
            case TRI :
            {
               res = AABBvsTriangle(m);
               break;
            }
         }
      }
   }

   return res;
}

bool AABBvsAABB(Manifold& m)
{
   AABB_t* a = m.a->asAABB();
   AABB_t* b = m.b->asAABB();

   // test for projection overlap along the x-axis
   float dist_x = m.a_p.x - m.b_p.x;
   float a_proj = scalar_proj(a->xw, X_AXIS); // + scalar_proj(a->yw, X_AXIS);
   float b_proj = scalar_proj(b->xw, X_AXIS); // + scalar_proj(b->yw, X_AXIS);
   float overlap_x = a_proj + b_proj - std::abs(dist_x);
   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = m.a_p.y - m.b_p.y;
      a_proj = /* scalar_proj(a->xw, Y_AXIS) + */ scalar_proj(a->yw, Y_AXIS);
      b_proj = /* scalar_proj(b->xw, Y_AXIS) + */ scalar_proj(b->yw, Y_AXIS);
      float overlap_y = a_proj + b_proj - std::abs(dist_y);
      if (overlap_y > 0)
      {
         // project out of collision along the axis of least overlap
         if (overlap_x < overlap_y) { // x-axis is min
            if (dist_x > 0) { 
               m.proj = vec2_t(overlap_x, 0);
            }
            else {
               m.proj = vec2_t(-overlap_x, 0);
            }
         }
         else { // y-axis is min
            if (dist_y > 0) {
               m.proj = vec2_t(0, overlap_y); 
            }
            else {
               m.proj = vec2_t(0, -overlap_y);
            }
         }

         return true;
      }
   }

   return false;
}

bool AABBvsCircle(Manifold& m)
{
   AABB_t* a = m.a->asAABB();
   Circle* b = m.b->asCircle();

   // test for projection overlap along the x-axis
   float dist_x = m.a_p.x - m.b_p.x;
   float a_proj = scalar_proj(a->xw, X_AXIS); // + scalar_proj(a->yw, X_AXIS);
   float overlap_x = a_proj + b->r - std::abs(dist_x);

   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = m.a_p.y - m.b_p.y;
      a_proj = /* scalar_proj(a->xw, Y_AXIS) + */ scalar_proj(a->yw, Y_AXIS); 
      float overlap_y = a_proj + b->r - std::abs(dist_y);

      if (overlap_y > 0)
      {
         // determine voronoi region 
         if (std::abs(dist_x) > a->xw.x && std::abs(dist_y) > a->yw.y) // throw out "edge" regions
         {
            vec2_t nearest_vertex;
            if (dist_x > 0 && dist_y > 0) { // top-right
               nearest_vertex = vec2_t(m.a_p.x + a->xw.x, m.a_p.y + a->yw.y);
            } 
            else if (dist_x < 0 && dist_y > 0) { // top-left
               nearest_vertex = vec2_t(m.a_p.x - a->xw.x, m.a_p.y + a->yw.y);
            } 
            else if (dist_x < 0 && dist_y < 0) { // bottom-left
               nearest_vertex = vec2_t(m.a_p.x - a->xw.x, m.a_p.y - a->yw.y);
            } 
            else if (dist_x > 0 && dist_y < 0) { // bottom-right
               nearest_vertex = vec2_t(m.a_p.x + a->xw.x, m.a_p.y - a->yw.y);
            }

            // test for projection overlap along the third separating axis
            vec2_t third_axis = vec2_t(nearest_vertex.x - m.b_p.x, nearest_vertex.y - m.b_p.y);
            normalize(third_axis);
            float dist_third = // dot(3rd, b) - dot(3rd, a)
               (third_axis.x * (m.b_p.x - m.a_p.x)) + (third_axis.y * (m.b_p.y - m.a_p.y));

            // 3rd axis may contain negatives, so we need to use magnitude for the AABB
            a_proj = std::abs(scalar_proj(a->xw, third_axis)) + 
                     std::abs(scalar_proj(a->yw, third_axis));

            float overlap_third = a_proj + b->r - std::abs(dist_third);

            if (overlap_third > 0)
            {
               // project out of collision along the axis of least overlap
               if (overlap_x < overlap_y) {
                  if (overlap_x < overlap_third) { // x-axis is min
                     if (dist_x > 0) { 
                        m.proj = vec2_t(overlap_x, 0);
                     }
                     else {
                        m.proj = vec2_t(-overlap_x, 0);
                     }
                  } 
                  else { // third axis is min
                     m.proj = 
                        vec2_t(third_axis.x * overlap_third, third_axis.y * overlap_third);
                  }
               } 
               else {
                  if (overlap_y < overlap_third) { // y-axis is min
                     if (dist_y > 0) {
                        m.proj = vec2_t(0, overlap_y);
                     }
                     else {
                        m.proj = vec2_t(0, -overlap_y);
                     }
                  } 
                  else { // third axis is min
                     m.proj = 
                        vec2_t(third_axis.x * overlap_third, third_axis.y * overlap_third);
                  }
               }

               return true;
            }
         }
         else
         {
            if (overlap_x < overlap_y) { // x-axis is min
               if (dist_x > 0) { 
                  m.proj = vec2_t(overlap_x, 0);
               }
               else {
                  m.proj = vec2_t(-overlap_x, 0);
               }
            }
            else { // y-axis is min 
               if (dist_y > 0) {
                  m.proj = vec2_t(0, overlap_y);
               }
               else {
                  m.proj = vec2_t(0, -overlap_y);
               }
            }

            return true;
         }
      }
   }

   return false;
}

bool AABBvsTriangle(Manifold& m)
{
   AABB_t* a = m.a->asAABB();
   Triangle* b = m.b->asTriangle();

   bool hypotenuse_case = collide_hyp(*b, m.b_p, m.a_p);

   // test for projection overlap along the x-axis
   float dist_x = m.a_p.x - m.b_p.x;
   float a_proj = scalar_proj(a->xw, X_AXIS); // + scalar_proj(a->yw, X_AXIS);
   float b_proj, overlap_x;
   // determine the relative position of AABB to triangle on the x-axis
   if ((b->xw.x > 0 && dist_x < 0) || // b faces (+) and a is (-) of b
       (b->xw.x < 0 && dist_x > 0))   // b faces (-) and a is (+) of b
   {
      // hypotenuse opposite from AABB along x-axis
      overlap_x = a_proj - std::abs(dist_x);
   }
   else { // hypotenuse adjacent to AABB along y-axis
      // float b_halfwidth_x = b->xw.x / 2; // NOTE: changed b->xw.x to halfwidth 
      dist_x = m.a_p.x - (m.b_p.x + b->xw.x);
      overlap_x = a_proj + std::abs(b->xw.x) - std::abs(dist_x);
   }
   if (overlap_x > 0)
   {
      // test for projection overlap along the y-axis
      float dist_y = m.a_p.y - m.b_p.y;
      a_proj = /* scalar_proj(a->xw, Y_AXIS) + */ scalar_proj(a->yw, Y_AXIS);
      float overlap_y;
      // // determine the relative position of AABB to triangle on the y-axis
      if ((b->yw.y > 0 && dist_y < 0) || // b faces (+) and a is (-) of b
          (b->yw.y < 0 && dist_y > 0))   // b faces (-) and a is (+) of b
      {
         // hypotenuse opposite from AABB along x-axis
         overlap_y = a_proj - std::abs(dist_y);
      }
      else { // hypotenuse adjacent to AABB along y-axis
         // float b_halfwidth_y = b->yw.y / 2; // NOTE: changed b->yw.y to halfwidth 
         dist_y = m.a_p.y - (m.b_p.y + b->yw.y);
         overlap_y = a_proj + std::abs(b->yw.y) - std::abs(dist_y);
      }
      if (overlap_y > 0)
      {
         if (!hypotenuse_case)
         {
            // project out of collision along the axis of least overlap
            if (overlap_x < overlap_y) {
               if (dist_x > 0) { 
                  m.proj = vec2_t(overlap_x, 0); // x-axis is min
               }
               else {
                  m.proj = vec2_t(-overlap_x, 0);
               }
            }
            else {
               if (dist_y > 0) {
                  m.proj = vec2_t(0, overlap_y); // y-axis is min 
               }
               else {
                  m.proj = vec2_t(0, -overlap_y);
               }
            }

            return true;
         }
         else // special case for when hypotenuse lies between the two centers
         {
            // test for projection overlap along the 3rd separating axis
            float dist_third = // dot(3rd, b) - dot(3rd, a)
               (b->third_axis.x * (m.b_p.x - m.a_p.x)) + (b->third_axis.y * (m.b_p.y - m.a_p.y));

            // 3rd axis may contain negatives, so we need to use magnitude for the AABB
            a_proj = std::abs(scalar_proj(a->xw, b->third_axis)) + 
                     std::abs(scalar_proj(a->yw, b->third_axis));

            // both legs are used bc they are halfwidths
            b_proj = scalar_proj(b->xw, b->third_axis) + scalar_proj(b->yw, b->third_axis);

            float overlap_third = a_proj + b_proj - std::abs(dist_third);

            if (overlap_third > 0)
            {
               // project out of collision along the axis of least overlap
               if (overlap_x < overlap_y) {
                  if (overlap_x < overlap_third) {
                     if (dist_x > 0) {
                        m.proj = vec2_t(overlap_x, 0); // x- axis is min
                     }
                     else {
                        m.proj = vec2_t(-overlap_x, 0);
                     }
                  } 
                  else { // third axis is min
                     m.proj = 
                        vec2_t(b->third_axis.x * overlap_third, b->third_axis.y * overlap_third);
                  }
               }
               else {
                  if (overlap_y < overlap_third) {
                     if (dist_y > 0) {
                        m.proj = vec2_t(0, overlap_y); // y-axis is min   
                     }
                     else {
                        m.proj = vec2_t(0, -overlap_y);
                     }
                  } 
                  else { // third axis is min
                     m.proj = 
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

// bool CirclevsCircle(Manifold* m)
// {
//    Circle* a = m->a->asCircle();
//    Circle* b = m->b->asCircle();

//    // test for projection overlap along the separating axis
//    float dist_sq = pow(b->p.x + a->p.x, 2.0) + pow(b->p.y + a->p.y, 2.0);
//    float overlap = (a->r + b->r) * (a->r + b->r) - dist_sq;
//    if (overlap > 0)
//    {
//       vec2_t axis = vec2_t(a->p.x - b->p.x, a->p.y - b->p.y);
//       normalize(axis);
//       overlap = a->r + b->r - sqrt(dist_sq);
//       m->proj = vec2_t(axis.x * overlap, axis.y * overlap);

//       return true;
//    }

//    return false;
// }

bool collide_hyp(Triangle& tri, vec3_t& tri_p, vec3_t& other_p)
{
   vec2_t dist = (vec2_t){ tri_p.x - other_p.x, tri_p.y - other_p.y };

   // watch for 0 problems here
   if (std::signbit(dist.x) != std::signbit(tri.xw.x)) {
      if (std::signbit(dist.y) != std::signbit(tri.yw.y)) {
         return true;
      }
   }

   return false;
}