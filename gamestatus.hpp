#ifndef GAMESTATUS_H_
#define GAMESTATUS_H_
#include "GameLib3D.hpp"
#include "base.hpp"
class GameStatus: public Base
{
    public:
    GameStatus(GameLib3D::Framework * const);
    virtual ~GameStatus();
    virtual Base * update(Base * const) override = 0;
    virtual void draw() = 0;
};
#endif