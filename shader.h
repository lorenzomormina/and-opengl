#pragma once

#include "platform.h"
#include <iostream>

const char *rectangle_vss = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
    layout (location = 1) in vec4 aColor; // the color variable has attribute position 1
    
    out vec4 ourColor; // output a color to the fragment shader

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(aPos, 0.0, 1.0);
        ourColor = aColor; // set ourColor to the input color we got from the vertex data
    }
)";

const char *rectangle_fss = R"(
    #version 330 core
    out vec4 FragColor;
    in vec4 ourColor;
    
    void main()
    {
        FragColor = ourColor;
    }
)";

const char *texture_vss = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec3 ourColor;
    out vec2 TexCoord;

    uniform mat4 projection;

    void main()
    {
	    gl_Position = projection * vec4(aPos, 0.0, 1.0);
	    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    }
)";

const char *texture_fss = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    // texture samplers
    uniform sampler2D texture1;

    void main()
    {

vec4 color = texture2D( texture1, TexCoord );
    FragColor = texture(texture1, TexCoord);
    }
)";

const char *ledTex_vss = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec4 fColor1;

uniform vec4 color1;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    fColor1 = vec4(color1.rgb, 1.0); 
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

)";

const char *ledTex_fss = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 fColor1;

// texture samplers
uniform sampler2D texture1;

void main()
{

vec4 color = texture2D( texture1, TexCoord );
vec3 original_color1 = vec3(0.99607843137, 0.00392156862, 0.99607843137);
vec3 diff1 = color.rgb - original_color1;
float equality1 = float(dot(diff1,diff1) < 0.00001);
color.rgb = mix( color.rgb, fColor1.rgb, equality1 );
    FragColor = color ;
//FragColor = texture(texture1, TexCoord);
}
)";

const char *text_vss = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
)";

const char *text_fss = R"(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }
)";

unsigned int buildShader(const char *vss, const char *fss);
void buildShaders();
void setProjection();

unsigned int rectangleShader;
unsigned int textureShader;
unsigned int textShader;
unsigned int ledTexShader;

unsigned int buildShader(const char *vss, const char *fss)
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vss, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fss, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void buildShaders()
{
    rectangleShader = buildShader(rectangle_vss, rectangle_fss);
    textureShader = buildShader(texture_vss, texture_fss);
    textShader = buildShader(text_vss, text_fss);
    ledTexShader = buildShader(ledTex_vss, ledTex_fss);
    setProjection();
}

void setProjection()
{
    glm::mat4 projection = glm::ortho(0.0f, (float)scrSize.x, (float)scrSize.y, 0.0f);
    glUseProgram(rectangleShader);
    glUniformMatrix4fv(glGetUniformLocation(rectangleShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(textureShader);
    glUniformMatrix4fv(glGetUniformLocation(textureShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(textShader);
    glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(ledTexShader);
    glUniformMatrix4fv(glGetUniformLocation(ledTexShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}
