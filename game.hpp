#ifndef GAME_H_
#define GAME_H_
#include"GameLib3D.hpp"

class GameStatus;

class Game
{
private:
    GameStatus * game_status;
    Game(GameLib3D::Framework * const framework);
    static Game * game;
    GameLib3D::Framework * frame_instance;
public:
    /// @brief this function would create a instance of Game if instances do not exist. If 
    /// a instance exists, this function would return this instance
    /// @param framework pointer of the GameLib3D::Framework
    /// @return the pointer of the instance of Game
    static Game * instance(GameLib3D::Framework * const framework);
    void update();
    virtual ~Game();
};

#endif