#include "GameLib3D.hpp"
#include "game.hpp"

void GameLib3D::Framework::update()
{
    Game* games = Game::instance(this);
    games->update();
}
