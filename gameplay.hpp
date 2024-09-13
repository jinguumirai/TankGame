#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_
#include"GameLib3D.hpp"
#include"gamestatus.hpp"

class PlayStatus;
class Play;
class Pause;
class GamePlay: public GameStatus
{
    private:
    Play * const play;
    Pause * const pause;
    PlayStatus * play_status;

    public:
    GamePlay(GameLib3D::Framework * const fra);
    virtual Base * update(Base* const) override;
    virtual ~GamePlay();
    virtual void draw() override;
};
#endif