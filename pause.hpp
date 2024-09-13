#ifndef PAUSE_H_
#define PAUSE_H_
#include"playstatus.hpp"

class Play;
class Pause: public PlayStatus
{
    private:
    GameLib3D::ImageMat image;
    public:
    virtual Base * update(Base * const) override;
    Pause(const char* pause_path, GameLib3D::Framework * fra);
    virtual void draw() override;
};

#endif