#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

// esercizio 2) passo da spazio vista a spazio mondo:
/* out vec3 LightPos;
uniform vec3 lightPos; 
void main()
{
    // la posizione della luce era nello spazio mondo
    LightPos= vec3(view*vec4(lightPos,1.0));

    //sposto anche il fragment nello spazio camera
    FragPos = vec3(view* model * vec4(aPos, 1.0));

    //anche la normale la sposto
    Normal = mat3(transpose(inverse(view*model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
*/

//esercizio 3: calcolo colore nel verte shader, quindi a livello di vertice
/*
out vec3 LightingColor;

uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    vec3 PosizioneV = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos - PosizioneV);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - PosizioneV);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    LightingColor = (ambient + diffuse + specular);
    
} 
*/