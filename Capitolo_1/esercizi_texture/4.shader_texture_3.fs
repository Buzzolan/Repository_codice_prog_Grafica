#version 330 core
out vec4 FragColor;
in vec3 ourColor;// in realtà non mi servirebbe, solo per esempio 
in vec2 TexCoord;
uniform sampler2D texture1;// passo il texture object al framgmet shader
uniform sampler2D texture2;

uniform float mixValue;
void main()
{
FragColor = mix(texture(texture1, TexCoord), texture(texture2,TexCoord), mixValue);
}