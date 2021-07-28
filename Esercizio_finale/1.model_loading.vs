#version 330 core
//setto in base all posizioni in cui ho caricato nel VAO del modello su file mesh.h
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
//carico Tangente e Bitangente per calcolarmi matrice TBN
layout (location = 3) in vec3 aTangent;
//layout (location = 4) in vec3 aBitangent;

#define NR_POINT_LIGHTS 3
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
    
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 TangentLightPos;
    
};


//cosa passo al fragment
out vec2 TexCoords;

//mi sposto nello spazio tangente
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out SpotLight TBNSpotLight;


//passo la posione nel sistema di riferimento bitangente e tutte le proprietà per il calcolo della luce
out PointLight StrutturaTBN[NR_POINT_LIGHTS];



uniform PointLight pointLights[NR_POINT_LIGHTS];//struttura definita nella cpu
uniform SpotLight spotLight;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//ora sono nello spazio mondo
uniform vec3 viewPos;



void main()
{
      
    vec3 FragPos = vec3( model * vec4(aPos, 1.0));//posizione del fragment, sono nello spazio mondo, poi nello spazio tangente dove farò lighting
    TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));// cosi normale rimane ortogonale alla superficie dopo tutte le trasformazioni che ho fatto
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    //ora però T potrebbe non essere più ortogonale a N dato che ho mediato
    //allor ri-ortogonalizzo T rispetto a N
    T = normalize(T - dot(T, N) * N);
    //da T e N ricavo Bitangente
    vec3 B = cross(N, T);
    // TBN must form a right handed coord system.
    // Some models have symetric UVs. Check and fix.
    if (dot(cross(N, T), B) < 0.0)
                T = T * -1.0;

    //calcolo matrice TBN inversa ma essendo ortogonale posso fare la trasposta che è un calcolo meno oneroso
    //facendo l'inversa la moltiplico per i componenti che mi servono per il calcolo della luce cosi da portarli nello spazio tangete

    mat3 TBN = transpose(mat3(T, B, N));
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
    
        //pointLights[i].position non mi serve più
        StrutturaTBN[i].constant = pointLights[i].constant;
        StrutturaTBN[i].linear = pointLights[i].linear;
        StrutturaTBN[i].quadratic = pointLights[i].quadratic;
        StrutturaTBN[i].ambient = pointLights[i].ambient;
        StrutturaTBN[i].diffuse = pointLights[i].diffuse;
        StrutturaTBN[i].specular = pointLights[i].specular;
        StrutturaTBN[i].TangentLightPos = TBN * pointLights[i].position;
        }
        
        
            
    //quello che mi serve per il calcolo della luce lo porto nel tangent space
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;
    TBNSpotLight.position = TBN * spotLight.position;
    TBNSpotLight.direction = TBN * spotLight.direction;

    TBNSpotLight.cutOff = spotLight.cutOff;
    TBNSpotLight.outerCutOff = spotLight.outerCutOff;
  
    TBNSpotLight.constant = spotLight.constant;
    TBNSpotLight.linear = spotLight.linear;
    TBNSpotLight.quadratic = spotLight.quadratic;
  
    TBNSpotLight.ambient = spotLight.ambient;
    TBNSpotLight.diffuse = spotLight.diffuse;
    TBNSpotLight.specular = spotLight.specular;  
    

    
    gl_Position = projection * view * model * vec4(aPos, 1.0);// posizione del vertice che passo al fragment, normalizzato secondo le coordinate di proiezione
    
    
    
    
    
    //il FragPos viene passato al fragment che calola la colorazione dei punti nello spazio mondo e alla fine passo il colore alla variabile FragColor e mi porta il frammento dove ho gl_position
    //Per impostare l'output del vertex shader dobbiamo assegnare i dati di posizione 
    //alla variabile predefinita gl_Position che è un vec4 dietro le quinte. 
    //Alla fine della funzione principale, qualunque cosa impostiamo a gl_Position verrà utilizzata come output del vertex shader. 
    //Dato che il nostro input è un vettore di dimensione 3, dobbiamo convertirlo in un vettore di dimensione 4. 
    //Possiamo farlo inserendo i valori di vec3 all'interno del costruttore di vec4 e impostando il suo componente w a 1.0f 

}