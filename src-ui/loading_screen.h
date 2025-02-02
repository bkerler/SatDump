#pragma once

#include <GLFW/glfw3.h>
#include "logger.h"

namespace satdump
{
    class LoadingScreenSink : public slog::LoggerSink
    {
    public:
        LoadingScreenSink(GLFWwindow* window, float scale, GLFWimage* img);
        ~LoadingScreenSink();
    protected:
        void receive(slog::LogMsg log);
    private:
        void push_frame(std::string str);
        float scale;
        GLFWwindow* window;
        GLuint image_texture;
    };
}