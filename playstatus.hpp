#ifndef PLAYSTATUS_H_
#define PLAYSTATUS_H_
#include "GameLib3D.hpp"
#include "base.hpp"

/// @brief all play status classes would inherit PlayStatus
class PlayStatus: public Base
{
    public:
    PlayStatus(GameLib3D::Framework * frame);
    virtual Base * update(Base * const) override = 0;
};
#endif