#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <string>

#include "platform.h"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct TexLed
{
    unsigned int texture;
    int nrChannels;

    std::string path;
    glm::vec2 pos, size;

    unsigned int VBO, VAO, EBO;
    unsigned int shader;

    bool value1 = false;
    glm::vec4 color1 = { 1.0f, 0.0f, 0.0f, 1.0f };
    int id;

    glm::vec2 tl() { return pos; }
    glm::vec2 tr() { return pos + glm::vec2{ size.x, 0 }; }
    glm::vec2 bl() { return pos + glm::vec2{ 0, size.y }; }
    glm::vec2 br() { return pos + size; }

    glm::vec2 ndc(glm::vec2 p)
    {
        return (2.0f * (p / scrSize) - glm::vec2{ 1, 1 }) * glm::vec2{ 1, -1 };
    }

    glm::vec2 ntl() { return ndc(tl()); }
    glm::vec2 ntr() { return ndc(tr()); }
    glm::vec2 nbl() { return ndc(bl()); }
    glm::vec2 nbr() { return ndc(br()); }

    void load(unsigned int shader, int id)
    {
        this->shader = shader;

        std::ifstream f("assets/layout.jsonc");
        json dataj = json::parse(f, nullptr, true, true);

        this->id = id;
        std::string ledId = "led" + std::to_string(id);
        pos = { dataj[ledId][0], dataj[ledId][1] };
        path = dataj[ledId][2];


        // texture
        // ---------
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        int width, height;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            size = { width, height };
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        update();
    }

    void remove()
    {
        glDeleteTextures(1, &texture);
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


    void update()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions            // texcoords
            tr().x, tr().y, 0.0f,   1.0f, 1.0f,
            br().x, br().y, 0.0f,   1.0f, 0.0f,
            bl().x, bl().y, 0.0f,   0.0f, 0.0f,
            tl().x, tl().y, 0.0f,   0.0f, 1.0f,
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // texcoords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(shader);
        glUniform4fv(glGetUniformLocation(shader, "color1"), 1, glm::value_ptr(color1));
    }

    void setPos(glm::vec2 pos)
    {
        this->pos = pos;
        update();
    }

    void setSize(glm::vec2 size)
    {
        this->size = size;
        update();
    }

    void setValue1(bool v)
    {
        value1 = v;
        color1 = value1 ? glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f } : glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
        update();
    }

    void draw(unsigned int shaderProgram)
    {
        // render container
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(shaderProgram);
        glUniform4fv(glGetUniformLocation(shader, "color1"), 1, glm::value_ptr(color1));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    bool contains(glm::vec2 p)
    {
        return p.x >= pos.x && p.x <= pos.x + size.x && p.y >= pos.y && p.y <= pos.y + size.y;
    }
};