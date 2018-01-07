#include "Command.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - JumpCommand
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
JumpCommand::JumpCommand(float jump_velocity) :
   jump_velocity_(jump_velocity)
{ }

void JumpCommand::execute(Actor& actor)
{
   actor.jump(jump_velocity_);
   // OR, just do the shit here, we don't need to see anything private, so modifying p / v is cool (for now)
   // i.e. v.y = jumpForce;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - AccelCommand
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AccelCommand::AccelCommand(float acceleration) :
   acceleration_(acceleration)
{ }

void AccelCommand::execute(Actor& actor)
{
   actor.accelerate(acceleration_);
}