#include"title.hpp"
#include"GameLib3D.hpp"
#include"gameplay.hpp"
#include "Complete.hpp"

Title::Title(const char * title_pic, GameLib3D::Framework * const fra_data):GameStatus(fra_data), title_image(title_pic)
{
    frame_instance->init_gl();
}
Base * Title::update(Base * const)
{
    GameLib3D::InputKey input_key = frame_instance->read_once_input();
    this->draw();
    if (input_key == GameLib3D::SPACE)
    {
        GamePlay * game_play = new GamePlay(frame_instance);
        return game_play;
        // return new Complete("resources/congratulations.png", frame_instance);
    }
    if (input_key == GameLib3D::ESCAPE)
    {
        frame_instance->destory();
    }
    return this;
}

void Title::draw()
{
    frame_instance->draw_image(title_image, title_image.width() / 2, title_image.height() / 2);
}