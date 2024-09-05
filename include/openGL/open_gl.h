#pragma once

#include <openGL/glad/glad.h>
#include <openGL/GLFW/glfw3.h>
#include <openGL/shader.h>



enum ScreenMode {
    WINDOWED,
    FULLSCREEN
};



class OpenGL
{
    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int renderbuffer;
        Shader *quadShader;

        float quadVertices[24] =  {
            // positions    // texCoords
            -1.0f,  1.0f,   0.0f, 1.0f,     // left top
            -1.0f, -1.0f,   0.0f, 0.0f,     // left bottom
             1.0f, -1.0f,   1.0f, 0.0f,     // right bottom

            -1.0f,  1.0f,   0.0f, 1.0f,     // left top
             1.0f, -1.0f,   1.0f, 0.0f,     // right bottom
             1.0f,  1.0f,   1.0f, 1.0f      // right top
        };

        float currentTime, lastTime;

        void initWindowed()
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // frame on/off
            window = glfwCreateWindow(width * pixelScale, height * pixelScale, title, NULL, NULL);
        }

        void initFullscreen()
        {
            GLFWmonitor *primaryMonitor =  glfwGetPrimaryMonitor();
            const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
            window = glfwCreateWindow(videoMode->width, videoMode->height, title, primaryMonitor, NULL);

            pixelScale = (width < height) ?
                (float)videoMode->width / width :
                (float)videoMode->height / height;

            for (int i = 0; i < 24; i += 4) {
                quadVertices[i + 0] *= (float)width / videoMode->width * pixelScale;
                quadVertices[i + 1] *= (float)height / videoMode->height * pixelScale;
            }
        }

        void initQuad()
        {
            glGenVertexArrays(1, &quadVAO);
            glBindVertexArray(quadVAO);

            glGenBuffers(1, &quadVBO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

            glGenTextures(1, &textureColorbuffer);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

            glGenRenderbuffers(1, &renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void terminate()
        {
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
            glDeleteFramebuffers(1, &framebuffer);
            glDeleteTextures(1, &textureColorbuffer);
            glDeleteRenderbuffers(1, &renderbuffer);

            glfwTerminate();
        }

    public:
        unsigned int width;
        unsigned int height;
        unsigned int offsetX;
        unsigned int offsetY;
        float pixelScale;
        int *pixels = nullptr;
        const char *title;
        GLFWwindow *window;

        float deltaTime;

        OpenGL(ScreenMode screenMode, unsigned int width, unsigned int height, int *pixels, float pixelScale = 1, const char *title = "OpenGL - 2D window")
        {
            this->width = width;
            this->height = height;
            this->pixelScale = pixelScale;
            this->pixels = pixels;
            this->title = title;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            if (screenMode == WINDOWED) {
                initWindowed();
            } else if (screenMode == FULLSCREEN) {
                initFullscreen();
            }

            if (window == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }
            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize GLAD" << std::endl;
                return;
            }

            initQuad();

            quadShader = new Shader("shaders/canvas.vert", "shaders/canvas.frag");
        }

        ~OpenGL() { terminate(); }

        void update()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glClearColor(.1f, .2f, .2f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            quadShader->use();
            glBindVertexArray(quadVAO);
            glDisable(GL_DEPTH_TEST);

            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();

            currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;
        }

        bool shouldClose() { return glfwWindowShouldClose(window); }
};
