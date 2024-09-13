#include"pause.hpp"
#include"title.hpp"

Pause::Pause(const char* path, GameLib3D::Framework * const fra):PlayStatus(fra), image(path)
{
    frame_instance->init_gl();
}

Base * Pause::update(Base * const game)
{
    GameLib3D::InputKey input_key = frame_instance->read_once_input();
    this->draw();
    if (input_key == GameLib3D::ESCAPE)
    {
        Title * title = new Title("resources/title.png", frame_instance);
        return title;
    }
    else if (input_key == GameLib3D::SPACE)
    {
        return game;
    }
    else
    {
        return this;
    }
}

void Pause::draw()
{
    frame_instance->draw_image(image, image.width() / 2, image.height() / 2);
}