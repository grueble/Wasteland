#version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TexCoord;

out vec3 ex_Color;
out vec2 TexCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(in_Position, 0.0, 1.0);

    ex_Color = in_Color;

    TexCoord = in_TexCoord;
}