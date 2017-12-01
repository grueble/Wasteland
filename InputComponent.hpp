#ifndef _INPUT_COMPONENT_HPP_
#define _INPUT_COMPONENT_HPP_

// forward declarations
class Entity;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - InputComponent
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

#endif
