#ifndef _GAME_OBJECTS_HPP_
#define _GAME_OBJECTS_HPP_

#include <vector>
#include <utility>

struct coord2d_s
{
   float x;

   float y;
};

typedef coord2d_s point2_t;

typedef coord2d_s vector2_t;

struct ray2_s
{
   point2_t base;

   vector2_t dir;
};

typedef ray2_s ray2_t;

enum Actors_e
{
   PLAYER = 0,
   NON_PLAYER = 1,
   PROJECTILE = 2
};

typedef Actors_e actor_t;

// typedef std::deque<PhysObj> objlist_t;

// typedef std::vector<Actor> actorlist_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - PhysObj & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class PhysObj
{
public:
   PhysObj();

private:

};

class LineSegment : public PhysObj
{
public:
   LineSegment();

private:
   point2_t a;
   
   point2_t b;
};

class Rectangle : public PhysObj
{
public:
   Rectangle();

private:
   int w;

   int h; 

   point2_t top_left;
};

class Circle : public PhysObj
{
public:
   Circle();

private:
   int r;

   point2_t center;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Actor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Actor
{
public:
   Actor();

   virtual void update();

private:
   actor_t actor_type;

   // base is the actor's focus point, dir is the horiz & vert velocities
   ray2_t mov_ray;

   // used in the broad phase of collision det
   Rectangle bound_box;

   // push PhysObjs on, relative to focus point
   std::vector<PhysObj> hitboxes;
};

#endif