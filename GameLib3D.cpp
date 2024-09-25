#include "GameLib3D.hpp"
#include <array>

GameLib3D::Framework * GameLib3D::Framework::frame = NULL;

void GameLib3D::Framework::clear(std::array<unsigned char, 3> color)
{
    int i;
    int max_i = frame_height * frame_width * 3;
    for (i = 0; i < max_i; i+=3)
    {
        frame_data[i] = color[0];
        frame_data[i + 1] = color[1];
        frame_data[i + 2] = color[2];
    }
}

GameLib3D::ImageMat::ImageMat(const char* image_path)
{
    cv::Mat image = cv::imread(image_path);
    if (image.empty() || image.channels() == 1)
    {
        std::cout << "Please input ture color image path!" << std::endl;
        exit(-1);
    }
    image_data = new UINT[image.cols * image.rows];
    int i, j;
    image_height = image.rows;
    image_width = image.cols;
    bool has_alpha = true;
    unsigned char b, g, r, alpha;

    /// If image has no alpha channel, we set alpha value of pixels as 255.
    if (image.channels() == 3)
    {
        has_alpha = false;  
    }
    
    for (i = 0; i < image_height; i++)
    {
        for (j = 0; j < image_width; j++)
        {
            if (has_alpha)
            {
                b = image.at<cv::Vec4b>(i, j)[0];
                g = image.at<cv::Vec4b>(i, j)[1];
                r = image.at<cv::Vec4b>(i, j)[2];
                alpha = image.at<cv::Vec4b>(i, j)[3];
            }
            else
            {
                b = image.at<cv::Vec3b>(i, j)[0];
                g = image.at<cv::Vec3b>(i, j)[1];
                r = image.at<cv::Vec3b>(i, j)[2];
            }

            /// convert coordinates in cv::Mat coordinate system into coordinates in ImageMat
            /// coordinate system
            int x_index = j;
            int y_index = image_height - i - 1;

            int index = x_index + y_index * image_width;

            image_data[index] = r;
            image_data[index] <<= 8;
            image_data[index] += g;
            image_data[index] <<= 8;
            image_data[index] += b;
            image_data[index] <<= 8;

            if (has_alpha)
            {
                image_data[index] += alpha;
            }
            else
            {
                image_data[index] += 0xFF;
            }
        }
    }
}

GameLib3D::ImageMat::ImageMat(unsigned int h, unsigned int w, const std::array<unsigned char, 4>& color):
image_height(h), image_width(w) 
{
    image_data = new UINT[w * h];
    int i, j;
    for (i = 0; i < w * h; i++)
    {
        image_data[i] = color[0];
        image_data[i] << 8;
        image_data[i] += color[1];
        image_data[i] << 8;
        image_data[i] += color[2];
        image_data[i] << 8;
        image_data[i] += color[3];
        image_data[i] << 8;
    }
}

GameLib3D::ImageMat::ImageMat(const GameLib3D::ImageMat& image_mat)
{
    image_width = image_mat.image_width;
    image_height = image_mat.image_height;
    int length = image_mat.image_height * image_mat.image_width;
    image_data = new UINT[length];
    int i;
    for (i = 0; i < length; i++)
    {
        image_data[i] = image_mat.image_data[i];
    }
}

const GameLib3D::ImageMat& GameLib3D::ImageMat::operator=(const GameLib3D::ImageMat& image_mat)
{
    delete image_data;
    image_width = image_mat.image_width;
    image_height = image_mat.image_height;
    int length = image_mat.image_height * image_mat.image_width;
    image_data = new UINT[length];
    int i;
    for (i = 0; i < length; i++)
    {
        image_data[i] = image_mat.image_data[i];
    }
    return image_mat;
}

const unsigned int GameLib3D::ImageMat::width() const
{
    return image_width;
}

const unsigned int GameLib3D::ImageMat::height() const
{
    return image_height;
}

UINT * const GameLib3D::ImageMat::data() const
{
    return image_data;
}

GameLib3D::ImageMat::~ImageMat()
{
    delete[] image_data;
}

GameLib3D::Framework::Framework(const unsigned int w, const unsigned int h, GLFWwindow * windows):frame_width(w),
frame_height(h), finish(false), window(windows), texture_shader("framework.vs", "framework.fs")
{
    GLfloat vertices[] = {
        // positions        // texture coords
         1.f,  1.f, 0.0f,   1.0f, 1.0f, // top right
         1.f, -1.f, 0.0f,   1.0f, 0.f, // bottom right
        -1.f, -1.f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.f,  1.f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    frame_data = new unsigned char[w * h * 3];
    frame_input.resize(7);
    time_queue.resize(11);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GameLib3D::Framework::~Framework()
{
    delete []frame_data;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
}

void GameLib3D::Framework::record_time()
{
    /// record current time and delete the earliest time in queue
    time_queue.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
    time_queue.erase(time_queue.begin());
}

double GameLib3D::Framework::average_delta_time()
{
    double deltas = time_queue[10] - time_queue[0];
    return deltas / 10;
}

GameLib3D::Framework * GameLib3D::Framework::instance(const int w, const int h, GLFWwindow * const windows)
{
    if (!frame)
    {
        frame = new Framework(w, h, windows);
    }
    return frame;
}

unsigned char * GameLib3D::Framework::data()
{
    return frame_data;
}

const int GameLib3D::Framework::height()
{
    return frame_height;
}

const int GameLib3D::Framework::width()
{
    return frame_width;
}

bool GameLib3D::Framework::is_finish()
{
    return finish;
}

void GameLib3D::Framework::destory()
{
    finish = true;
    glfwSetWindowShouldClose(window, finish);
}

void GameLib3D::Framework::draw_image(const ImageMat& image, int x, int y)
{
    int half_image_height = image.height() / 2;
    int half_image_width = image.width() / 2;
    int x_start = x - half_image_width;
    int y_start = y - half_image_height;
    int x_max = x_start + image.width();
    int y_max = y_start + image.height();
    int x_fin = x_max <= this->width() ? x_max : this->width();
    int y_fin = y_max <= this->height() ? y_max : this->height();
    int x_index;
    int y_index;

    /// if image out of screen, do not draw
    if (x_start >= this->width() || y_start >= this->height() || x_max < 0 || y_max < 0)
    {
        return;
    }

    /// draw the pixels if it on screen
    for (y_index = y_start >= 0 ? y_start : 0; y_index < y_fin; y_index++)
    {
        for (x_index = x_start >= 0 ? x_start : 0; x_index < x_fin; x_index++)
        {
            
            /// convert coordinates in screen coordinates system into coordinates in ImageMat
            /// coordinate system
            int mat_i = x_index - x_start;
            int mat_j = y_index - y_start;
            
            UINT value = image.data()[mat_i + mat_j * image.width()];
            unsigned char alpha = value % 0x100;
            value >>= 8;
            unsigned char b = value % 0x100;
            value >>= 8;
            unsigned char g = value % 0x100;
            value >>= 8;
            unsigned char r = value;
            int fra_index = (x_index + y_index * this->width()) * 3;
            double fact = double(alpha) / 255;
            double left_fact = 1 - fact;

            /// draw the pixel by using alpha value
            this->frame_data[fra_index] = fact * r + this->frame_data[fra_index] * left_fact;
            this->frame_data[fra_index + 1] = fact * g + this->frame_data[fra_index + 1] * left_fact;
            this->frame_data[fra_index + 2] = fact * b + this->frame_data[fra_index + 2] * left_fact;
        }
    }
    this->init_gl();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame_data);
    draw();
}

GameLib3D::InputKey GameLib3D::Framework::read_input() const
{
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            return GameLib3D::UP;
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            return GameLib3D::DOWN;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            return GameLib3D::LEFT;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            return GameLib3D::RIGHT;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            return GameLib3D::ESCAPE;
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            return GameLib3D::P;
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            return GameLib3D::SPACE;
        }
        else
        {
            return GameLib3D::NUL;
        }
}

GameLib3D::InputKey GameLib3D::Framework::read_once_input() const
{
        static bool is_released[7] = {true, true, true, true, true, true, true};
        /// if you pressing a key continously, is_released[key] would be false until you release
        /// the key.
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && is_released[UP])
        {
            is_released[UP] = false;
            return GameLib3D::UP;
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && is_released[DOWN])
        {
            is_released[DOWN] = false;
            return GameLib3D::DOWN;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && is_released[LEFT])
        {
            is_released[LEFT] = false;
            return GameLib3D::LEFT;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && is_released[DOWN])
        {
            is_released[RIGHT] = false;
            return GameLib3D::RIGHT;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && is_released[ESCAPE])
        {
            is_released[ESCAPE] = false;
            return GameLib3D::ESCAPE;
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && is_released[P])
        {
            is_released[P] = false;
            return GameLib3D::P;
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && is_released[SPACE])
        {
            is_released[SPACE] = false;
            return GameLib3D::SPACE;
        }
        else
        {
            /// If key is realeased, set is_released[key] as true
            if (!is_released[UP] && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
            {
                is_released[UP] = true;
            }
            if (!is_released[DOWN] && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
            {
                is_released[DOWN] = true;
            }
            if (!is_released[LEFT] && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
            {
                is_released[LEFT] = true;
            }
            if (!is_released[RIGHT] && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
            {
                is_released[RIGHT] = true;
            }
            if (!is_released[ESCAPE] && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
            {
                is_released[ESCAPE] = true;
            }
            if (!is_released[P] && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
            {
                is_released[P] = true;
            }
            if (!is_released[SPACE] && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            {
                is_released[SPACE] = true;
            }
            return GameLib3D::NUL;
        }
}

void GameLib3D::Framework::init_gl()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->width(), this->height());
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.);
    glClear(GL_COLOR_BUFFER_BIT); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  
}

void GameLib3D::Framework::draw()
{
    texture_shader.use();
    texture_shader.setInt("texture1", 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}