#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool useUniformColor;
uniform vec3 customColor;

void main()
{
    if (useUniformColor)
    {
        color = vec4(customColor, 1.0f);
    }
    else
    {
        color = texture(texture_diffuse1, TexCoords);
    }
}