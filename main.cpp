#include <openGL/open_gl.h>
#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 150;
const float PIXEL_SCALE = 5;

double mouseX, mouseY;
Canvas *mouseSprite;

void show(OpenGL *openGL)
{
    // openGL->canvas->drawRectangle(0, 0, openGL->canvas->width, openGL->canvas->height, 0xffff0000);

    openGL->canvas->drawImage(10, 10, "Green_Marble_Alien_Face_128x128.png");

    openGL->canvas->drawCircle(openGL->canvas->width - 125, openGL->canvas->height - 60, 10, 0xff00ffff, true);
    openGL->canvas->spanFill(openGL->canvas->width - 125, openGL->canvas->height - 60, 0x8800ffff, true);

    openGL->canvas->drawCircle(openGL->canvas->width - 50, 50, 15, 0xff0000ff, true);
    openGL->canvas->floodFill(openGL->canvas->width - 50, 50, 0x880000ff, true);

    openGL->canvas->drawLine(0, 0, openGL->canvas->width - 1, openGL->canvas->height - 1, 0x4400ff00, true);
    openGL->canvas->drawCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 40, 0x66ff4444, true);

    openGL->canvas->setPixels(mouseX, mouseY, mouseSprite, true);
}

void userControl(OpenGL *openGL)
{
    // keys
    if (glfwGetKey(openGL->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(openGL->window, true);
    }

    // mouse
    glfwGetCursorPos(openGL->window, &mouseX, &mouseY);
    mouseX = mouseX / openGL->pixelScale;
    mouseY = openGL->canvas->height - mouseY / openGL->pixelScale;
}

int main()
{
    OpenGL openGL = OpenGL(WINDOWED, PIXEL_SCALE, CANVAS_WIDTH, CANVAS_HEIGHT);

    mouseSprite = new Canvas(11, 11);
    mouseSprite->clearCanvas();
    mouseSprite->drawCircle(5, 5, 5, 0xffffff00);
    mouseSprite->floodFill(5, 5, 0x44ffff00);

    while (!openGL.shouldClose())
    {
        openGL.canvas->clearCanvas();

        show(&openGL);
        userControl(&openGL);

        // std::cout << openGL.deltaTime << std::endl;

        openGL.update();
    }

    return 0;
}
