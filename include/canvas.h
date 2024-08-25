#pragma once

#include <sprite.h>



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

        void setPixel(int x, int y, int value)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                pixels[x + y * width] = value;
            }
        }

        int getPixel(int x, int y)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                return pixels[x + y * width];
            }

            return -1;
        }

        void drawRectangle(int x, int y, int w, int h, int color, bool centered = false)
        {
            if (centered) {
                x -= w >> 1;
                y -= h >> 1;
            }

            for (int i = 0; i < w; i++) {
                setPixel(x + i, y        , color);
                setPixel(x + i, y + h - 1, color);
            }
            for (int i = 1; i < h - 1; i++) {
                setPixel(x        , y + i, color);
                setPixel(x + w - 1, y + i, color);
            }
        }

        void drawLine(int x1, int y1, int x2, int y2, int color)
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
                    setPixel(x, y, color);
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
                    setPixel(x, y, color);
                    if (d > 0) {
                        x += sx;
                        d -= dy * 2;
                    }
                    d += dx * 2;
                }
            }
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

        void drawCircle(int x, int y, int r, int color)
        {
            int xx = -r;
            int yy = 0;
            int rr = 2 - 2 * r;
            do {
                setPixel(x - xx, y + yy, color);
                setPixel(x - yy, y - xx, color);
                setPixel(x + xx, y - yy, color);
                setPixel(x + yy, y + xx, color);
                r = rr;
                if (r <= yy) rr += ++yy * 2 + 1;
                if (r > xx || rr > yy) rr += ++xx * 2 + 1;
            } while (xx < 0);
        }

        void floodFill(int x, int y, int color)
        {
            int targetColor = getPixel(x, y);
            if (targetColor == color || targetColor == -1) return;

            int *next = new int[width * height * 2];
            int index = 0;

            while (true) {
                setPixel(x, y, color);

                if (getPixel(x, y + 1) == targetColor) {
                    next[index] = x;
                    next[index + 1] = y + 1;
                    index += 2;
                }
                if (getPixel(x + 1, y) == targetColor) {
                    next[index] = x + 1;
                    next[index + 1] = y;
                    index += 2;
                }
                if (getPixel(x, y - 1) == targetColor) {
                    next[index] = x;
                    next[index + 1] = y - 1;
                    index += 2;
                }
                if (getPixel(x - 1, y) == targetColor) {
                    next[index] = x - 1;
                    next[index + 1] = y;
                    index += 2;
                }

                if (index == 0) break;
                index -= 2;
                x = next[index];
                y = next[index + 1];
            }

            delete[] next;
        }

        void spanFill(int x, int y, int color)
        {
            unsigned int targetColor = getPixel(x, y);
            if (targetColor == color || targetColor == -1) return;

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
                if (getPixel(x, y) == targetColor) {
                    while (getPixel(x - 1, y) == targetColor) {
                        setPixel(x - 1, y, color);
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
                    while (getPixel(x1, y) == targetColor) {
                        setPixel(x1, y, color);
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
                    while (x1 < x2 && getPixel(x1, y) != targetColor) x1++;
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

        void drawSprite(int x, int y, Sprite *sprite, bool centered = false)
        {
            if (centered) {
                x -= sprite->width >> 1;
                y -= sprite->height >> 1;
            }

            for (int i = 0; i < sprite->width; i++) {
                int xx = x + i;
                if (xx >= 0 && xx < width) {
                    for (int j = 0; j < sprite->height; j++) {
                        int yy = y + j;
                        if (yy >= 0 && yy < height) {
                            pixels[xx + yy * width] = sprite->pixels[i + j * sprite->width];
                        }
                    }
                }
            }
        }
};
