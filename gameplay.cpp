#include"gameplay.hpp"
#include"play.hpp"
#include"pause.hpp"

GamePlay::GamePlay(GameLib3D::Framework * const fra):
GameStatus(fra), play(new Play(frame_instance)), pause(new Pause("resources/pause.png", fra))
{
    play_status = play;
}

Base * GamePlay::update(Base * const base)
{
    PlayStatus * now = play_status;
    PlayStatus * another = pause;
    if (play_status == pause)
    {
        another = play;
    }
    Base * next;
    next = play_status->update(another);
    PlayStatus * next_status = dynamic_cast<PlayStatus*>(next);
    if (next_status)
    {
        if (next_status != now)
        {
            play_status = next_status;
        }
        return this;
    }
    else
    {
        /// if next status is not gameplay, return the next status
        return next;
    }
}

GamePlay::~GamePlay()
{
    delete play;
    delete pause;
}

void GamePlay::draw()
{
    ;
}