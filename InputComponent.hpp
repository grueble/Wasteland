#ifndef _INPUT_COMPONENT_HPP_
#define _INPUT_COMPONENT_HPP_

#include "Command.hpp"

// forward declarations
class Entity;

static const int MAX_PENDING = 50;

// enum ActionId
// {
//    LEFT_ACCEL_ON,
//    LEFT_ACCEL_OFF,
//    RIGHT_ACCEL_ON,
//    RIGHT_ACCEL_OFF,
//    START_JUMP,
//    END_JUMP,
//    ENABLE_CROUCH,
//    DISABLE_CROUCH,
//    CAMERA_CENTER,
//    CAMERA_FOCUS,
// };

// struct PlayerActionMessage
// {
//    ActionId id;

// };

// struct CameraActionMessage
// {

// };



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

   virtual void update(Actor& obj);

   virtual void pushCommand(const SDL_Event& e) = 0;

protected:
   Command *pending_[MAX_PENDING] = {};
   int num_pending_ = 0;

// private:
//    static const int MAX_PENDING = 16;
};

class PlayerInputComponent : public InputComponent
{
public:
   PlayerInputComponent();

   ~PlayerInputComponent();

   // void update(Actor& obj) override;

   void pushCommand(const SDL_Event& e) override;

private:
   // NOTE: will want to count the # of each type of event per frame, 
   //       use to limit how many of the same event are processed (just return nullptr)
   virtual Command* handleMouseButtonDown(const SDL_Event& e);
   // virtual Command* handleMouseButtonUp(const SDL_Event& e);
   virtual Command* handleKeyDown(const SDL_Event& e);
   virtual Command* handleKeyUp(const SDL_Event& e);

   // commands available to this actor
   Command *jump_ = nullptr;
   Command *left_accel_ = nullptr;
   Command *right_accel_ = nullptr;
   Command *brake_ = nullptr;
   Command *enable_crouch_ = nullptr; 
   Command *disable_crouch_ = nullptr;
};

// class CameraInputComponent : public InputComponent
// {
// public:
//    CameraInputComponent();

//    ~CameraInputComponent();

//    // void update(Actor& obj) override;

//    void pushCommand(const SDL_Event& e) override;
// };

#endif
