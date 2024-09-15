#pragma once

struct ColorRGBA
{
    public:
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;
        unsigned int value;

        ColorRGBA(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0, unsigned int a = 0)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
            updateValue();
        }

        void updateValue()
        {
            value = r | (g << 8) | (b << 16) | (a << 24);
        }
};
