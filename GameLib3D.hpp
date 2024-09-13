#ifndef GAMELIB3D_H_
#define GAMELIB3D_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include "shader.hpp"
#define UINT uint32_t
#include <array>
#include <chrono>
namespace GameLib3D
{
    /*image matrix based on OpenCV*/
    class ImageMat
    {
    private:
        UINT * image_data;
        unsigned int image_width;
        unsigned int image_height;
        
    public:

        /// @brief Every UINT data represents the value of a pixel. The first byte data is the value
        /// of red channel, the second byte is the value of green channel, the third byte is the 
        /// value of blue channel, the last byte is the value of alpha channel. 
        /// @return the address of the image data. The first data in this address is the value 
        /// of the pixel at the bottom-left corner. data[i + width * j] is the value of the pixel 
        /// whose row index is j and col index is i.
        UINT * const data() const;

        /// @brief Image Matrix created by the path of image
        /// @param image_path the path of image
        ImageMat(const char * image_path);

        /// @brief Image Matrix created by preset parameters
        /// @param h image height
        /// @param w image width
        /// @param color color of the image. Four elements represent the red, green, blue 
        /// and alpha value respectively
        ImageMat(unsigned int h, unsigned int w, const std::array<unsigned char, 4>& color);
        const unsigned int width() const;
        const unsigned int height() const;

        /// @brief deep copy
        ImageMat(const ImageMat&);
        ~ImageMat();

        /// @brief deep copy
        const ImageMat& operator=(const ImageMat&);
    };

    enum InputKey
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        SPACE,
        ESCAPE,
        P,
        NUL,
    };

    class Framework
    {
    private:
        Framework(const unsigned int width, const unsigned int height, GLFWwindow* const);
        static Framework * frame;
        std::vector<uint64_t> time_queue;
        const unsigned int frame_width, frame_height;
        std::vector<bool> frame_input;
        bool finish;
        
        Shader texture_shader;
        GLuint VAO, VBO, EBO, texture;

    public:
        unsigned char * frame_data;
        GLFWwindow * const window;
        void init_gl();
        /// @brief create a instance if no instance exist
        /// @param width the width of the window
        /// @param height the height of the window
        /// @param window the pointer of the GLFW Window
        /// @return the pointer of the FrameworK2D
        static Framework * instance(const int width, const int height, GLFWwindow * const window);
        
        /// @brief the main loop of the program. You can define this function to complete the 
        /// program.
        void update();

        /// @return the address of the frame buffer. The first data in the address is the red
        /// value of the pixel at the bottom-left corner. 
        unsigned char * data();
        const int width();
        const int height();

        /// @return return the key which is pressed
        InputKey read_input() const;

        /// @return the key you pressed. If you pressing a key continously, it only return the
        /// key value when you press the key.
        InputKey read_once_input() const;


        /// @return the signal to finish the program
        bool is_finish();

        /// @brief set signal to finish as true
        void destory();

        /// @brief record the time. The unit of time is ms. 
        void record_time();

        /// @brief pass image to frame buffer
        /// @param image passed image mat
        /// @param x abscissa in the frame coordinate system of the central pixel
        /// @param y vertical ordinate in the frame coordinate system of the central pixel
        void draw_image(const ImageMat& image, int x, int y);

        void draw();

        /// @return the average time interval between 11 recorded times. The unit of time is ms
        double average_delta_time();

        /// @brief clear screen
        void clear(std::array<unsigned char, 3> color);

        ~Framework();
    };
}
#endif