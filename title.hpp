#ifndef TITLE_H_
#define TITLE_H_

#include"gamestatus.hpp"
#include"GameLib3D.hpp"

class Title:public GameStatus
{
    private:
    GameLib3D::ImageMat title_image;
    public:
    Title(const char * title_pic, GameLib3D::Framework * const fra_data);
    virtual Base* update(Base * const) override;
    virtual void draw() override;
};
#endif