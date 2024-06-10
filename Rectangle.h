#pragma once

#include "platform.h"

struct Rectangle
{
    glm::vec2 pos, size;
    float color[4];

    bool value = false;

    unsigned int VBO, VAO, EBO;

    glm::vec2 tl() { return pos; }
    glm::vec2 tr() { return pos + glm::vec2{ size.x, 0 }; }
    glm::vec2 bl() { return pos + glm::vec2{ 0, size.y }; }
    glm::vec2 br() { return pos + size; }

    glm::vec2 ndc(glm::vec2 p)
    {
        return (2.0f * (p / scrSize) - glm::vec2{ 1, 1 }) * glm::vec2 { 1, -1 };
    }

    glm::vec2 ntl() { return ndc(tl()); }
    glm::vec2 ntr() { return ndc(tr()); }
    glm::vec2 nbl() { return ndc(bl()); }
    glm::vec2 nbr() { return ndc(br()); }

    bool contains(glm::vec2 p)
    {
        return p.x >= pos.x && p.x <= pos.x + size.x && p.y >= pos.y && p.y <= pos.y + size.y;
    }

    void switch_()
    {
        setValue(!value);
    }

    void setValue(bool value)
    {
        this->value = value;
        color[0] = value ? 0.0f : 1.0f;
        color[1] = value ? 1.0f : 0.0f;
        update();
    }

    void update()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            tr().x, tr().y, 0.0f, color[0], color[1], color[2], color[3], // top right
            br().x, br().y, 0.0f, color[0], color[1], color[2], color[3], // bottom right
            bl().x, bl().y, 0.0f, color[0], color[1], color[2], color[3], // bottom left
            tl().x, tl().y, 0.0f, color[0], color[1], color[2], color[3]  // top left
        };

        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void setup()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        update();
    }

    void draw()
    {
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time
    }

    void remove()
    {
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};