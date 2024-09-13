#include "Complete.hpp"
#include "title.hpp"

Complete::Complete(const char* image_path, GameLib3D::Framework* const fra): GameStatus(fra), complete_image(image_path)
{
    fra->init_gl();
}

Base* Complete::update(Base* const)
{
    GameLib3D::InputKey input_key = frame_instance->read_once_input();
    this->draw();
    if (input_key == GameLib3D::SPACE)
    {
        return new Title("resources/title.png", frame_instance);
    }
    return this;
}

void Complete::draw()
{
    frame_instance->clear({0, 0, 0});
    frame_instance->clear({0, 0, 0});
    frame_instance->draw_image(complete_image, complete_image.width() / 2, complete_image.height() / 2);
}