#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float visibility;

void main()
{
    FragColor = mix(texture(texture_1, TexCoord), texture(texture_2, vec2(TexCoord.x * -1.0, TexCoord.y)), visibility);// * vec4(ourColor, 1.0);
}