#include "gamecamera.hpp"
#include "asset.hpp"

FollowCamera::FollowCamera(BasicAsset * const origin_object):
follow_object(origin_object)
{
    ;
}

glm::mat4 FollowCamera::get_view()
{
    glm::vec3 pos_camera = follow_object->pos_vec - follow_object->front_vec() + glm::vec3({0.0f, 0.5f, 0.0f});
    return glm::lookAt(pos_camera, follow_object->pos_vec, {0, 1, 0});
}