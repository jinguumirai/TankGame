#include"play.hpp"
#include <stb_image.h>
#include "Complete.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Play::Play(GameLib3D::Framework* fra_instance):
PlayStatus(fra_instance), 
tank("resources/model/tank/export3dcoat.obj", 0, {0, -5, -2}, {0.01, 0.01, 0.01}, 1.0, 30.0f),
simu_box("resources/wood.png", 1, 0.0f, 0.0f, glm::vec3(0.0f), 
glm::vec3(5.0f)), follow_tank_camera(&tank),
shader("point_shadows.vs", "point_shadows.fs"), lightPos(0.0f, 0.0f, 0.0f), cool_down(0.0f),
simpleDepthShader("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs")
{
    boxes.push_back(ParamAsset("resources/wood.png", 0, 0.0f, 0.0f, 
    {-3.0f, -4.0f, -3.0f}, glm::vec3(1.0f)));
    boxes.push_back(ParamAsset("resources/wood.png", 0, 0.0f, 0.0f, 
    {3.0f, -4.0f, 3.0f}, glm::vec3(1.0f)));
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthCubeMap);
}

void Play::play_init()
{
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
        frame_instance->width(), frame_instance->height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("depthMap", 10);
}

void Play::draw()
{
    play_init();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)frame_instance->width() / (float)frame_instance->height(), near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    glViewport(0, 0, frame_instance->width(), frame_instance->height());
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    simpleDepthShader.use();
    for (unsigned int i = 0; i < 6; ++i)
        simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    simpleDepthShader.setVec3("lightPos", lightPos);
    simpleDepthShader.setFloat("far_plane", far_plane);
    simu_box.draw(simpleDepthShader);
    for (auto& box : boxes)
    {
        box.draw(simpleDepthShader);
    }
    for (auto& bullet : bullets)
    {
        bullet.draw(simpleDepthShader);
    }
    tank.draw(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, frame_instance->width(), frame_instance->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 
    (float)frame_instance->width() / (float)frame_instance->height(), 0.1f, 100.0f);
    auto view = follow_tank_camera.get_view();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", {0.0f, 0.0f, 0.0f});
    shader.setInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
    shader.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    simu_box.draw(shader);
    for (auto& box: boxes)
    {
        box.draw(shader);
    }
    for (auto& bullet : bullets)
    {
        bullet.draw(shader);
    }
    tank.draw(shader);
    glfwSwapBuffers(frame_instance->window);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Play::~Play()
{
    glBindBuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDeleteBuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthCubeMap);
}

Base* Play::update(Base* another_status)
{
    
    GameLib3D::InputKey input_key = frame_instance->read_once_input();
    if (input_key == GameLib3D::P)
    {
        return another_status;
    }
    double delta_time = frame_instance->average_delta_time() / 1000;

    cool_down -= delta_time;
    if (cool_down < 0)
    {
        cool_down = 0;
    }
    if (cool_down <= 0.0f && input_key == GameLib3D::SPACE)
    {
        cool_down = 0.5f;
        bullets.push_back(ParamAsset("resources/bullet.jpg", 0, 0.0f, 
        3.0f, tank.pos_vec + tank.front_vec() + glm::vec3({0.0f, 0.5f, 0.0f}), {0.05f, 0.05f, 0.05f}, 
        0.0f, tank.front_vec()));
    }
    
    GameLib3D::InputKey continous_key = frame_instance->read_input();
    
    if (continous_key == GameLib3D::UP)
    {
        auto delta_vec = tank.front_vec() * glm::vec3(tank.speed * delta_time);
        tank.pos_vec += delta_vec;
        if (!(this->in_box(&tank)))
        {
            tank.pos_vec -= delta_vec;
        }
    }
    if (continous_key == GameLib3D::DOWN)
    {
        auto delta_vec = tank.front_vec() * glm::vec3(tank.speed * delta_time);
        tank.pos_vec -= delta_vec;
        if (!(this->in_box(&tank)))
        {
            tank.pos_vec += delta_vec;
        }
    }

    if (continous_key == GameLib3D::LEFT)
    {
        tank.rotated_angle += (tank.rotate_speed * delta_time);
        while (tank.rotated_angle >= 360.0f)
        {
            tank.rotated_angle -= 360.0f;
        }
    }

    if (continous_key == GameLib3D::RIGHT)
    {
        tank.rotated_angle -= (tank.rotate_speed * delta_time);
        while (tank.rotated_angle < 0.0f)
        {
            tank.rotated_angle += 360.0f;
        }
    }
    
    auto bullet_iter = bullets.begin();
    while (bullet_iter != bullets.end())
    {
        (*bullet_iter).pos_vec += ((*bullet_iter).front_vec() * glm::vec3(bullet_iter->speed) * 
        glm::vec3(delta_time));
        if (!in_box(&(*bullet_iter)))
        {
            bullet_iter = bullets.erase(bullet_iter);
        }
        else
        {
            bullet_iter++;
        }
    }

    bullet_iter = bullets.begin();
    while (bullet_iter != bullets.end())
    {
        auto box_iter = boxes.begin();
        bool crash_flag = false;
        while(box_iter != boxes.end())
        {
            if (is_crash(&(*box_iter), &(*bullet_iter)))
            {
                box_iter = boxes.erase(box_iter);
                bullet_iter = bullets.erase(bullet_iter);
                crash_flag = true;
                break;
            }
            else
            {
                box_iter++;
            }
        }
        if (!crash_flag)
        {
            bullet_iter++;
        }
    }
    
    this->draw();
    return this;
}

bool Play::in_box(BasicAsset* asset)
{
    auto asset_edges = asset->edge_vector();
    auto box_edges = simu_box.edge_vector();

    if (asset_edges[0] <= box_edges[0] && asset_edges[1] >= box_edges[1] && 
    asset_edges[2] <= box_edges[2] && asset_edges[3] >= box_edges[3] && asset_edges[4] <= box_edges[4] &&
    asset_edges[5] >= box_edges[5])
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Play::is_crash(const BasicAsset * asset1, const BasicAsset * asset2)
{
    auto edge1 = asset1->edge_vector();
    auto edge2 = asset2->edge_vector();
    if (edge1[1] <= edge2[0] && edge2[1] <= edge1[0] && edge1[3] <= edge2[2] && 
    edge2[3] <= edge1[2] && edge1[5] <= edge2[4] && edge2[5] <= edge1[4])
    {
        return true;
    }
    else
    {
        return false;
    }
}

