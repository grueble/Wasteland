#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

class Command 
{
public:
   virtual ~Command() {}
   virtual void execute(Actor& actor) = 0;
};

class JumpCommand : public Command 
{
public:
   void execute(Actor& actor) override;
};

#endif