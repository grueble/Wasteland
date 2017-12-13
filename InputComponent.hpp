#ifndef _INPUT_COMPONENT_HPP_
#define _INPUT_COMPONENT_HPP_

// forward declarations
class Entity;

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

#endif
