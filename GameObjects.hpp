#ifndef _GAME_OBJECTS_HPP_
#define _GAME_OBJECTS_HPP_

#include <vector>
#include <utility>

#include <GL/gl.h>

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

// nah don't do this
enum Actors_e
{
   PLAYER = 0,
   NON_PLAYER = 1,
   PROJECTILE = 2
};

typedef Actors_e actor_t;

static struct
{
   GLuint vertex_buffer, element_buffer;
   GLuint textures[2];

   // fields for shader objects
} g_resources;

// typedef std::deque<PhysObj> objlist_t;

// typedef std::vector<Actor> actorlist_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - PhysObj & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class PhysObj
{
public:
   PhysObj();

   ~PhysObj();

   virtual void draw() = 0;
private:

};

class LineSegment : public PhysObj
{
public:
   LineSegment();

   ~LineSegment();

   void draw();

private:
   point2_t a;
   
   point2_t b;
};

class Rectangle : public PhysObj
{
public:
   Rectangle();

   ~Rectangle();

   void draw();

private:
   int w;

   int h; 

   point2_t top_left;
};

class Circle : public PhysObj
{
public:
   Circle();

   ~Circle();

   void draw();

private:
   int r;

   point2_t center;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Actor & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Actor
{
public:
   Actor();

   ~Actor();   

   virtual void update();

private:
   // base is the actor's focus point, dir is the horiz & vert velocities
   ray2_t mov_ray;

   // used in the broad phase of collision det
   // Rectangle bound_box;

   // push PhysObjs on, relative to focus point
   std::vector<PhysObj> hitboxes;

   // "textureData" : texture (pointer?), size, render fn()
};

// class Player : public Actor
// {
// public:
//    Player();

//    ~Player();

// private:

// };

#endif