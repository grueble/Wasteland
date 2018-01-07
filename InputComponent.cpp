#include "InputComponent.hpp"
#include "GameObjs.hpp"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - InputComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputComponent::update(Actor& obj)
{
   std::cout << "input: update " << num_pending_ << "w/ " << &obj << std::endl;

   for (int i = 0; i < num_pending_; ++i)
   {
      pending_[i]->execute(obj);
   }

   num_pending_ = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - PlayerInputComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PlayerInputComponent::PlayerInputComponent()
{
   jump_ = new JumpCommand(0.25f);
   left_accel_ = new AccelCommand(-0.05f);
   right_accel_ = new AccelCommand(0.05f);
}

PlayerInputComponent::~PlayerInputComponent()
{
   if (jump_) { delete jump_; }
   if (left_accel_) { delete left_accel_; }
   if (right_accel_) { delete right_accel_; }
   if (brake_) { delete brake_; }
   if (enable_crouch_) { delete enable_crouch_; }
   if (disable_crouch_) { delete disable_crouch_; }
}

void PlayerInputComponent::pushCommand(const SDL_Event& e)
{
   // std::cout << "pushCommand" << num_pending_ << std::endl;

   Command *cmd = nullptr;

   switch (e.type)
   {
      // case SDL_MOUSEBUTTONDOWN :
      // {
      //    cmd = handleMouseButtonDown(e);
      //    break;
      // }
      // case SDL_MOUSEBUTTONUP :
      // {
      //    cmd = handleMouseButtonUp(e);
      //    break;
      // }
      case SDL_KEYDOWN :
      {
         cmd = handleKeyDown(e);
         break;
      }
      case SDL_KEYUP :
      {
         cmd = handleKeyUp(e);
         break;
      }
      default : { /*no action*/ break; }
   }

   if (cmd)
   {
      assert(num_pending_ < MAX_PENDING);

      pending_[num_pending_] = cmd;
      num_pending_++;
   }
}

Command* PlayerInputComponent::handleMouseButtonDown(const SDL_Event& e) { }

Command* PlayerInputComponent::handleKeyDown(const SDL_Event& e)
{
   Command *cmd = nullptr;

   switch (e.key.keysym.sym)
   {
      case 'w' :
      {
         cmd = jump_;
         break;
      }
      case 'a' :
      {
         cmd = left_accel_;
         break;
      }
      case 's' :
      {
         cmd = enable_crouch_;
         break;
      }
      case 'd' :
      {
         cmd = right_accel_;
         break;
      }
      default : { /*no action*/ break; }
   }

   return cmd;
}

Command* PlayerInputComponent::handleKeyUp(const SDL_Event& e)
{
   Command *cmd = nullptr;

   switch (e.key.keysym.sym)
   {
      case 'a' :
      case 'd' :
      {
         cmd = brake_;
         break;
      }
      case 's' :
      {
         cmd = disable_crouch_;
         break;
      }
      default : { /*no action*/ break; }
   }

   return cmd;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - CameraInputComponent
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CameraInputComponent::CameraInputComponent()
// {
   
// }

// CameraInputComponent::~CameraInputComponent()
// {

// }

// // pass player into this?
// void CameraInputComponent::update(Actor& obj)
// {

// }

// void CameraInputComponent::pushCommand(const SDL_Event& e)
// {

// }