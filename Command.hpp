#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "GameObjs.hpp"

class Command 
{
public:
   virtual ~Command() {}
   virtual void execute(Actor& actor) = 0;
};

class JumpCommand : public Command 
{
public:
   JumpCommand(float jump_velocity);

   void execute(Actor& actor) override;

private:
   float jump_velocity_;
};

class AccelCommand : public Command
{
public:
   AccelCommand(float acceleration);

   void execute(Actor& actor) override;

private:
   float acceleration_;
};

class BrakeCommand : public Command
{
public:

private:
   
};

#endif