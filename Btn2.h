#pragma once

#include "Rectangle.h"
#include "text.h"

struct Btn2 : Rectangle
{
    std::string text = "F";
    unsigned int textShader;

    Btn2() : Rectangle()
    {
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

    void switch_()
    {
        Rectangle::switch_();
        if (text == "F")
            text = "T";
        else
            text = "F";
    }

};