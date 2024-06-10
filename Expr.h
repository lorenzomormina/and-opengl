#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <glm/glm.hpp>

struct Expr
{
    Btn1 btn1;
    BtnOp btnOp;
    Btn2 btn2;
    BtnRes btnRes;
    glm::vec2 pos;
    glm::vec2 spacing;
    float height;

    void load(unsigned int textShader)
    {

        std::ifstream f("assets/layout.jsonc");
        json dataj = json::parse(f, nullptr, true, true);

        pos = { dataj["expr"][0], dataj["expr"][1] };
        spacing = glm::vec2{ dataj["expr-sp"][0], 0 };
        height = dataj["op"][1];

        btn1.pos = pos;
        btn1.size = { dataj["op"][0], dataj["op"][1] };
        btn1.setup();
        btn1.textShader = textShader;

        glm::vec2 opnSize = { dataj["operand"][0], dataj["operand"][1] };
        btnOp.pos = btn1.tr() + spacing;
        btnOp.size = opnSize;
        btnOp.setup();
        btnOp.textShader = textShader;

        btn2.pos = btnOp.tr() + spacing;
        btn2.size = btn1.size;
        btn2.setup();
        btn2.textShader = textShader;

        auto wi = ComputeTextWidth("=", 1.0);
        btnRes.pos = btn2.tr() + 2.0f*spacing + glm::vec2{wi, 0};
        btnRes.size = btn1.size;
        btnRes.setup();
        btnRes.textShader = textShader;

    }

    void draw()
    {
        glUseProgram(rectangleShader);
        btn1.draw();
        glUseProgram(rectangleShader);
        btnOp.draw();
        glUseProgram(rectangleShader);
        btn2.draw();


        auto pos = btn2.tr() + spacing;
        // the vertical size of "=" is the same as "A".
        // so, even though "=" is smaller than "A", the texture height is the same.
        auto th = ComputeTextHeight("A", 1.0);
        pos.y = pos.y + btn2.size.y / 2 - th / 2;
        RenderText(textShader, "=", pos.x, 405, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

        glUseProgram(rectangleShader);
        btnRes.draw();
    }
};