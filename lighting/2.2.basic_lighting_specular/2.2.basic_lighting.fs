#version 330 core
out vec4 FragColor;



in vec3 Normal;  
in vec3 FragPos;  
// esercizio 2: 
//in vec3 LightPos;
  
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    //vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 

/*
// esercizio 3
in vec3 Normal;  
in vec3 FragPos;
in vec3 LightingColor;

uniform vec3 objectColor;

void main()
{   
    //lightcolor= colore calcolato nel Vertex shader
    FragColor= vec4(LightingColor*objectColor,1.0);
}
*/