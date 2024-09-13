#ifndef COMPLETE_H_
#define COMPLETE_H_
#include "gamestatus.hpp"

class Complete: public GameStatus
{
    private:
    GameLib3D::ImageMat complete_image;
    public:
    Complete(const char * image_path, GameLib3D::Framework* const fra);
    virtual Base* update(Base* const) override;
    virtual void draw() override;
};
#endif