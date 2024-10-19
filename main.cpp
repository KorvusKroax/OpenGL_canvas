#include <openGL/open_gl.h>
#include <iostream>

Canvas *mouseSprite;

void show(OpenGL *openGL)
{
    // openGL->canvas->drawImage(10, 10, "Green_Marble_Alien_Face_128x128.png");

    // Canvas cut = openGL->canvas->getPixels(15, 15, 30, 30);
    // openGL->canvas->setPixels(openGL->canvas->width - 120, openGL->canvas->height - 70, &cut);

    // openGL->canvas->drawCircle(openGL->canvas->width - 125, openGL->canvas->height - 70, 10, ColorRGBA(255, 255, 0));
    // openGL->canvas->spanFill(openGL->canvas->width - 125, openGL->canvas->height - 70, ColorRGBA(255, 255, 0, 128));

    // openGL->canvas->drawCircle(openGL->canvas->width - 5, 50, 15, ColorRGBA(255, 0, 0));
    // openGL->canvas->floodFill(openGL->canvas->width - 5, 50, ColorRGBA(255, 0, 0, 128));

    // openGL->canvas->drawLine(0, 0, openGL->canvas->width - 1, openGL->canvas->height - 1, ColorRGBA(0, 255, 0, 64));

    openGL->canvas->drawCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 40, ColorRGBA(64, 64, 255, 96));
    openGL->canvas->drawMidpointCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 35, ColorRGBA(255, 64, 64, 96));

    openGL->canvas->drawFilledCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 30, ColorRGBA(64, 64, 255, 128));
    // openGL->canvas->drawFilledCircle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 2, ColorRGBA(64, 64, 255, 128));

    // openGL->canvas->drawRectangle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 15, 30, ColorRGBA(128, 0, 64, 128));
    // openGL->canvas->drawFilledRectangle(openGL->canvas->width >> 1, openGL->canvas->height >> 1, 20, 30, ColorRGBA(128, 0, 64, 128));

    // openGL->canvas->drawAALine(openGL->canvas->width >> 1, openGL->canvas->height >> 1, openGL->mouseX, openGL->mouseY, ColorRGBA(255, 128, 0));

    // openGL->canvas->setPixels(openGL->mouseX - (mouseSprite->width >> 1), openGL->mouseY - (mouseSprite->height >> 1), mouseSprite);
    openGL->canvas->setPixel(openGL->mouseX, openGL->mouseY, ColorRGBA(255, 255, 255));

    openGL->canvas->drawBezierCurve_quadratic(
        10, 10,
        openGL->canvas->width >> 1, openGL->canvas->height - 10,
        openGL->canvas->width - 10, 10,
        15, ColorRGBA(255, 255, 0));

    openGL->canvas->drawBezierCurve_cubic(
        0, 0,
        openGL->canvas->width >> 2, openGL->canvas->height - 1,
        (openGL->canvas->width >> 2) * 3, 10,
        openGL->canvas->width - 1, openGL->canvas->height - 1,
        15, ColorRGBA(255, 0, 255));
}

int main()
{
    OpenGL openGL = OpenGL(FULLSCREEN, 10);//, 320, 200);
    // OpenGL openGL = OpenGL(WINDOWED, 4, 320, 200);

    mouseSprite = new Canvas(11, 11);
    mouseSprite->clearCanvas();
    mouseSprite->drawFilledCircle(5, 5, 5, ColorRGBA(0, 255, 255, 128));
    // mouseSprite->drawCircle(5, 5, 5, ColorRGBA(0, 255, 255));
    // mouseSprite->floodFill(5, 5, ColorRGBA(0, 255, 255, 128));
    mouseSprite->setPixel(5, 5, ColorRGBA(255, 255, 255));

    glfwSetInputMode(openGL.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(openGL.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
