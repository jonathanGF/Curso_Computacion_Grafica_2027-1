#version 330 core
out vec4 color;

uniform vec3 objectColor; // Color enviado desde C++

void main()
{
    color = vec4(objectColor, 1.0f);
}