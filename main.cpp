#include <iostream>

#include <openGL/open_gl.h>
#include <canvas.h>



const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const float PIXEL_SCALE = 3.5;

double mouseX, mouseY;

SpriteSheet spriteSheet = SpriteSheet("Green_Marble_Alien_Face_128x128.png");
Sprite mousePointer = spriteSheet.getSprite(0, 0, 30, 30);



void show(Canvas *canvas)
{
    // canvas->drawImage(30, 30, "Green_Marble_Alien_Face_128x128.png");
    // canvas->drawSprite(mouseX, mouseY, &mousePointer);
    canvas->drawCircle(mouseX, mouseY, 5, 0xffffff00);
    canvas->floodFill(mouseX, mouseY, 0xff000088);
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
    mouseY = openGL->height - mouseY / openGL->pixelScale;
}

int main()
{
    Canvas canvas = Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    OpenGL openGL = OpenGL(WINDOWED, canvas.width, canvas.height, canvas.pixels, PIXEL_SCALE);

    while (!openGL.shouldClose())
    {
        canvas.clearCanvas();

        show(&canvas);
        userControl(&openGL);

        std::cout << openGL.deltaTime << std::endl;

        openGL.update();
    }

    return 0;
}
