#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // da dove arriva la luce

    float diff = max(dot(norm, lightDir), 0.0); //prodotto scalre tra norma e direzione della luce(mi dice come sono orienatiti tra di loro)
    vec3 diffuse = diff * lightColor; // K=1, diff maggiore se dir e norm sono nello stesso verso 
            
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 