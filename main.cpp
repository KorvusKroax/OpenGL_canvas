#include <openGL/open_gl.h>
#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const float PIXEL_SCALE = 4;

Canvas *mouseSprite;

void show(OpenGL *openGL)
{
    openGL->canvas->drawImage(10, 10, "Green_Marble_Alien_Face_128x128.png");

    Canvas cut = openGL->canvas->getPixels(15, 15, 30, 30);
    openGL->canvas->setPixels(openGL->canvas->width - 120, openGL->canvas->height - 70, &cut);

    openGL->canvas->drawCircle(openGL->canvas->width - 125, openGL->canvas->height - 70, 10, 0xff00ffff, true);
    openGL->canvas->spanFill(openGL->canvas->width - 125, openGL->canvas->height - 70, 0x8800ffff, true);

    openGL->canvas->drawCircle(openGL->canvas->width - 5, 50, 15, 0xff0000ff, true);
    openGL->canvas->floodFill(openGL->canvas->width - 5, 50, 0x880000ff, true);

    openGL->canvas->drawLine(0, 0, openGL->canvas->width - 1, openGL->canvas->height - 1, 0x4400ff00, true);
    openGL->canvas->drawCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 40, 0x66ff4444, true);

    openGL->canvas->setPixels(openGL->mouseX - (mouseSprite->width >> 1), openGL->mouseY - (mouseSprite->height >> 1), mouseSprite, true);
    openGL->canvas->setPixel(openGL->mouseX, openGL->mouseY, 0xffffffff);
}

int main()
{
    OpenGL openGL = OpenGL(FULLSCREEN, PIXEL_SCALE);//, CANVAS_WIDTH, CANVAS_HEIGHT);

    mouseSprite = new Canvas(11, 11);
    mouseSprite->clearCanvas();
    mouseSprite->drawCircle(5, 5, 5, 0xffffff00);
    mouseSprite->floodFill(5, 5, 0x44ffff00);
    mouseSprite->setPixel(5, 5, 0xffffff00);

    while (!openGL.shouldClose())
    {
        openGL.canvas->clearCanvas();

        show(&openGL);

        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        openGL.update();
    }

    return 0;
}
