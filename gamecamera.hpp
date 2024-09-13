#ifndef GAME_CAMERA_H_
#define GAME_CAMERA_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BasicAsset;

class FollowCamera
{
    public:
    BasicAsset * const follow_object;
    FollowCamera(BasicAsset * const origin_object);

    glm::mat4 get_view();
};
#endif