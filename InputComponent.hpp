#ifndef _INPUT_COMPONENT_HPP_
#define _INPUT_COMPONENT_HPP_

#include "Command.hpp"

// forward declarations
class Entity;

enum ActionId
{
   LEFT_ACCEL_ON,
   LEFT_ACCEL_OFF,
   RIGHT_ACCEL_ON,
   RIGHT_ACCEL_OFF,
   START_JUMP,
   END_JUMP,
   ENABLE_CROUCH,
   DISABLE_CROUCH,
   CAMERA_CENTER,
   CAMERA_FOCUS,
};

struct PlayerActionMessage
{
   ActionId id;

};

struct CameraActionMessage
{

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - InputComponent
//
// --> so this needs an event queue, which will allow me to build AI
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class InputComponent 
{
public:
   virtual ~InputComponent() {}

   void update(Entity& obj);
};

class PlayerInputComponent : public InputComponent
{
public:
   PlayerInputComponent();
};

class CameraInputComponent : public InputComponent
{
public:
   CameraInputComponent();

   void update(Entity& obj) override;
};

#endif
