#ifndef ASSET_H_
#define ASSET_H_
#include"playstatus.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "GameLib3D.hpp"
#include <stb_image.h>

class BasicAsset
{
    public:
    float speed;
    float rotate_speed;
    float radians_angle();
    float cos_angle();
    float sin_angle();
    float x_max, x_min, y_max, y_min, z_max, z_min;
    std::vector<float> edge_vector() const;
    glm::vec3 ori_front;
    glm::vec3 pos_vec;
    float rotated_angle;
    glm::vec3 scale_vec;
    int reverse_normals;
    glm::mat4 model_mat();
    BasicAsset(glm::vec3 pos, glm::vec3 scale, float rotated_angle, glm::vec3 ori_front, int reverse_normal, float speed, float rotate_speed);
    virtual ~BasicAsset();
    virtual void draw(Shader& shader) = 0;
    glm::vec3 front_vec();
};

class ObjAsset: public BasicAsset
{
    public:
    Model model;
    ObjAsset(const char * object_path, int reverse_normal, glm::vec3 pos_vec, 
    glm::vec3 scale_vec, float speed, float rotate_speed=0.0f, float rotated_angle=0.f, 
    glm::vec3 ori_front={0.0f, 0.0f, 1.0f});

    virtual void draw(Shader& shader);
};

class ParamAsset: public BasicAsset
{
    public:
    GLuint VAO, VBO;
    GLuint texture;
    float rotate_speed;
    std::string texture_path;
    ParamAsset(const ParamAsset&);
    ParamAsset(const char* texture_path, int reverse_normal, float rotate_speed=0.0f, 
    float speed=0.0f, glm::vec3 pos_vec={0.0f, 0.0f, 0.0f}, glm::vec3 scale=glm::vec3(1.0f),
    float rotated_angle=0.0f, glm::vec3 ori_front={0.0f, 0.0f, -1.0f});
    ParamAsset& operator=(const ParamAsset& another);
    virtual void draw(Shader& shader);
    virtual ~ParamAsset();
};

#endif