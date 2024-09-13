#ifndef BASE_H_
#define BASE_H_
#include "GameLib3D.hpp"

/// @brief Base class, all status classes would inherit Base
class Base
{
protected:
    GameLib3D::Framework* frame_instance;
public:
    Base(GameLib3D::Framework* const);
    virtual Base * update(Base * const) = 0;
    
    virtual void draw() = 0;
};
#endif