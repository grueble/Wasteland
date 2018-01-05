#include "Command.hpp"

JumpCommand::execute(Actor& actor)
{
   actor.jump();
}