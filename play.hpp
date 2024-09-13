#ifndef PLAY_H_
#define PLAY_H_
#include"playstatus.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "gamecamera.hpp"
#include "asset.hpp"

class Play: public PlayStatus
{
    public:
    bool is_crash(const BasicAsset * asset1, const BasicAsset * asset2);
    std::vector<ParamAsset> bullets;
    std::vector<ParamAsset> boxes;
    Shader shader;
    Shader simpleDepthShader;
    ObjAsset tank;
    ParamAsset simu_box;
    FollowCamera follow_tank_camera;
    GLuint depthMapFBO;
    GLuint depthCubeMap;
    float cool_down;
    glm::vec3 lightPos;
    Play(GameLib3D::Framework * fra_instance);
    virtual ~Play();
    void play_init();
    bool in_box(BasicAsset*);
    virtual void draw();
    virtual Base* update(Base* another_status);
};
#endif