#version 330 core
//al vertex shader devo passare le coordiante texture
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord; //input per fragment shader
void main()
{
vec3 newPos=vec3(-aPos.x, aPos.y, aPos.z);
gl_Position = vec4(newPos, 1.0);
ourColor = aColor;
TexCoord = aTexCoord;
}