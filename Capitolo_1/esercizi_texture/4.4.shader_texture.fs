#version 330 core
out vec4 FragColor;
in vec3 ourColor;// in realtà non mi servirebbe, solo per esempio 
in vec2 TexCoord;
uniform sampler2D ourTexture;// passo il texture object al framgmet shader
void main()
{
FragColor = texture(ourTexture, TexCoord);
}