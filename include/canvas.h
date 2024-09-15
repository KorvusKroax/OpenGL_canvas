#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <iostream>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int *pixels;

        Canvas() { }

        Canvas(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            this->pixels = new int[this->width * this->height];
        }

        ~Canvas() { delete[] pixels; }

        void clearCanvas()
        {
            memset(pixels, 0, width * height * sizeof(int));
        }

        void fillCanvas(int color)
        {
            for (int i = 0; i < width * height; i++) {
                pixels[i] = color;
            }
        }

        void setPixel(int x, int y, int color, bool alphaMask = false)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if (alphaMask && (color >> 24) < 255) {
                    if ((color >> 24) == 0) {
                        return;
                    }

                    int pixel = pixels[x + y * width];

                    int alpha = (color & 0xff000000) >> 24;
                    float weight = (float)alpha / 255;

                    int r = int( (pixel & 0x000000ff)        * (1.0f - weight) + (color & 0x000000ff)         * weight);
                    int g = int(((pixel & 0x0000ff00) >> 8)  * (1.0f - weight) + ((color & 0x0000ff00) >> 8)  * weight);
                    int b = int(((pixel & 0x00ff0000) >> 16) * (1.0f - weight) + ((color & 0x00ff0000) >> 16) * weight);

                    pixels[x + y * width] = r | (g << 8) | (b << 16) | (alpha << 24);
                } else {
                    pixels[x + y * width] = color;
                }
            }
        }

        void setPixels(int x, int y, Canvas *canvas, bool alphaMask = false)
        {
            for (int i = 0; i < canvas->width; i++) {
                for (int j = 0; j < canvas->height; j++) {
                    setPixel(x + i, y + j, canvas->pixels[i + j * canvas->width], alphaMask);
                }
            }
        }

        bool getPixel(int x, int y, int *color)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                *(color) = pixels[x + y * width];
                return true;
            }

            *(color) = 0;
            return false;
        }

        Canvas getPixels(int x, int y, unsigned int w, unsigned int h)
        {
            int value;
            Canvas canvas = Canvas(w, h);
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    getPixel(x + i, y + j, &value);
                    canvas.pixels[i + j * w] = value;
                }
            }

            return canvas;
        }

        void drawRectangle(int x, int y, int w, int h, int color, bool alphaMask = false)
        {
            for (int i = 0; i < w; i++) {
                setPixel(x + i, y        , color, alphaMask);
                setPixel(x + i, y + h - 1, color, alphaMask);
            }
            for (int i = 1; i < h - 1; i++) {
                setPixel(x        , y + i, color, alphaMask);
                setPixel(x + w - 1, y + i, color, alphaMask);
            }
        }

        void drawLine(int x1, int y1, int x2, int y2, int color, bool alphaMask = false)
        {
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;

            int pos = x1 + y1 * width;

            if (dx > dy) {
                int y = y1;
                int d = dy * 2 - dx;
                for (int x = x1; x != x2; x += sx) {
                    setPixel(x, y, color, alphaMask);
                    if (d > 0) {
                        y += sy;
                        d -= dx * 2;
                    }
                    d += dy * 2;
                }
            } else {
                int x = x1;
                int d = dx * 2 - dy;
                for (int y = y1; y != y2; y += sy) {
                    setPixel(x, y, color, alphaMask);
                    if (d > 0) {
                        x += sx;
                        d -= dy * 2;
                    }
                    d += dx * 2;
                }
            }
            setPixel(x2, y2, color, alphaMask);
        }

        void drawLine_unsafe(int x1, int y1, int x2, int y2, int color)
        {
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? width : -width;

            if (dx > dy) {
                int y = y1;
                int d = dy * 2 - dx;
                for (int pos = x1 + y1 * width; pos != x2 + y2 * width; pos += sx) {
                    pixels[pos] = color;
                    if (d > 0) {
                        pos += sy;
                        d -= dx * 2;
                    }
                    d += dy * 2;
                }
            } else {
                int x = x1;
                int d = dx * 2 - dy;
                for (int pos = x1 + y1 * width; pos != x2 + y2 * width; pos += sy) {
                    pixels[pos] = color;
                    if (d > 0) {
                        pos += sx;
                        d -= dy * 2;
                    }
                    d += dx * 2;
                }
            }
        }

        void drawCircle(int x, int y, int r, int color, bool alphaMask = false)
        {
            int xx = -r;
            int yy = 0;
            int rr = 2 - 2 * r;
            do {
                setPixel(x - xx, y + yy, color, alphaMask);
                setPixel(x - yy, y - xx, color, alphaMask);
                setPixel(x + xx, y - yy, color, alphaMask);
                setPixel(x + yy, y + xx, color, alphaMask);
                r = rr;
                if (r <= yy) rr += ++yy * 2 + 1;
                if (r > xx || rr > yy) rr += ++xx * 2 + 1;
            } while (xx < 0);
        }

        void floodFill(int x, int y, int color, bool alphaMask = false)
        {
            int targetColor, pixel;
            if (!getPixel(x, y, &targetColor) || targetColor == color) return;

            int *next = new int[width * height * 2];
            int index = 0;

            int dir[] = {0,1, 1,0, 0,-1, -1,0};

            setPixel(x, y, color, alphaMask);
            while (true) {
                for (int i = 0; i < 8; i += 2) {
                    if (getPixel(x + dir[i], y + dir[i + 1], &pixel) && pixel == targetColor) {
                        setPixel(x + dir[i], y + dir[i + 1], color, alphaMask);
                        next[index] = x + dir[i];
                        next[index + 1] = y + dir[i + 1];
                        index += 2;
                    }
                }

                if (index == 0) break;
                index -= 2;
                x = next[index];
                y = next[index + 1];
            }

            delete[] next;
        }

        void spanFill(int x, int y, int color, bool alphaMask = false)
        {
            int targetColor, pixel;
            if (!getPixel(x, y, &targetColor) || targetColor == color) return;

            int *next = new int[width * height * 2];
            int index = 0;

            next[index] = x;
            next[index + 1] = x;
            next[index + 2] = y;
            next[index + 3] = 1;
            index += 4;

            int x1 = x;
            int x2 = x;
                y  = y - 1;
            int dy = -1;

            while (true) {
                x = x1;
                if (getPixel(x, y, &pixel) && pixel == targetColor) {
                    while (getPixel(x - 1, y, &pixel) && pixel == targetColor) {
                        setPixel(x - 1, y, color, alphaMask);
                        x--;
                    }
                    if (x < x1) {
                        next[index] = x;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y - dy;
                        next[index + 3] = -dy;
                        index += 4;
                    }
                }

                while (x1 <= x2) {
                    while (getPixel(x1, y, &pixel) && pixel == targetColor) {
                        setPixel(x1, y, color, alphaMask);
                        x1++;
                    }
                    if (x1 > x) {
                        next[index] = x;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y + dy;
                        next[index + 3] = dy;
                        index += 4;
                    }
                    if (x1 - 1 > x2) {
                        next[index] = x2 + 1;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y - dy;
                        next[index + 3] = -dy;
                        index += 4;
                    }
                    x1++;
                    while (x1 < x2 && getPixel(x1, y, &pixel) && pixel != targetColor) x1++;
                    x = x1;
                }
                if (index == 0) break;
                index -= 4;
                x1 = next[index];
                x2 = next[index + 1];
                y  = next[index + 2];
                dy = next[index + 3];
            }

            delete[] next;
        }

        void drawImage(int x, int y, const char *fileName, bool flipImageVertically = true)
        {
            int w, h, channelCount;
            stbi_set_flip_vertically_on_load(flipImageVertically);
            unsigned char *image = stbi_load(fileName, &w, &h, &channelCount, 0);

            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {

                    int color =
                        image[(i * channelCount + 0) + j * w * channelCount] |
                        image[(i * channelCount + 1) + j * w * channelCount] << 8 |
                        image[(i * channelCount + 2) + j * w * channelCount] << 16 |
                        255 << 24;

                    setPixel(x + i, y + j, color);
                }
            }
        }
};
