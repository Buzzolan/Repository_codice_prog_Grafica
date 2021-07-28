#version 330 core
out vec4 FragColor;

uniform float ourColorR; //variabile globale
uniform float ourColorG;
uniform float ourColorB;
// prima era il vertex shader che passava il colore con in vec3 ourClor;

void main()
{
    FragColor = vec4(ourColorR, ourColorG, ourColorB,1.0) ;
}
