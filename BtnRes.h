#pragma once

#include "Rectangle.h"
#include "text.h"

struct BtnRes : Rectangle
{
    std::string text = "F";
    unsigned int textShader;

    BtnRes() : Rectangle()
    {
        pos = { 340,400 };
        size = { 50, 50 };
        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 0.0;
        color[3] = 1.0;
    }

    void draw()
    {
        Rectangle::draw();
        auto tw = ComputeTextWidth(text, 1.0);
        auto th = ComputeTextHeight(text, 1.0);
        auto textpos = pos + glm::vec2{ size.x / 2 - tw / 2, size.y / 2 - th / 2 };
        RenderText(textShader, text, textpos.x, textpos.y, 1.0, glm::vec3{ 1.0, 1.0, 1.0 });
    }

    void setValue(bool value)
    {
        Rectangle::setValue(value);
        text = value ? "T" : "F";
    }
};