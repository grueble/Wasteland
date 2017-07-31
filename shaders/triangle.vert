#version 330 core

layout(location = 0) in vec2 in_Position; // vertexPosition_modelspace
layout(location = 1) in vec3 in_Color;

uniform mat4 MVP;

out vec3 ex_Color;

void main()
{
   gl_Position = MVP * vec4(in_Position, 0.0, 1.0);
   // gl_Position = vec4(in_Position, 0.0, 1.0);

   ex_Color = in_Color;
}