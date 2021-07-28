#version 330 core
out vec4 FragColor;//colore del frag

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


in vec2 TexCoords;
//quello che mi serve per il calcolo della luce è ora nello spazio tangente
//la normale la prendo dalla normal map passatta dall'obj
in vec3 TangentViewPos;
in vec3 TangentFragPos;
//passo la struttura delle posizioni delle luci rispetto allo spazio tangente
in PointLight StrutturaTBN[NR_POINT_LIGHTS];
in SpotLight TBNSpotLight;

//feature della luce che passo come uniform allo shader dato che le ho settate nel mio programma
//---------------------------------------------vanno settate una volta quindo potresti toglierle dal loop
uniform float shininess;//quanta lucentezza do al mio materiale
//uniform PointLight pointLights[NR_POINT_LIGHTS];//luce ambientale, diffusiva,speculare,....

//testure map del mio modello 
uniform sampler2D texture_diffuse1;//collegata allo shader nel file mesh.h grazie alla Draw
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;


//pototipi di funzione
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 result;
     // ottengo le normali dalla normal map che sono nello spazio tangente 
     vec3 normal = texture(texture_normal1, TexCoords).rgb;// qui le normali sono stae shiftate nel range [0 1]
     // transform normal vector to range [-1,1], giustamente le riporto nel loro range originale
    normal = normalize(normal * 2.0 - 1.0);

    vec3 TviewDir=normalize(TangentViewPos - TangentFragPos); //ricavo la direzione da dove sto guardadno 

    // Per ogni luce che ho vado a calcolarmi il valore del frage sommo tutte le componenti 
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        //quindi per calclolarmi il valore da assergnare devo avere: PointLight(caratteristiche luce),StrutturaTBN(posizione delle luci nello spazio tangente)
        //le normali, la posizione del frag nello spazio tangente e il mio punto di vista nello spazio tangente
        result += CalcPointLight(StrutturaTBN[i], normal, TangentFragPos, TviewDir);   
    
    // aggiungo luce spot:
    result += CalcSpotLight(TBNSpotLight, normal, TangentFragPos, TviewDir);

    FragColor = vec4(result, 1.0);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
     // ambient:
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

    // diffuse:
    //colpisce una superficie lambertiana, quindi la luce si riflette in maniera uniforme
    //dipende dal coeff K del materiale dato dal modellatore, qui ricavato dalla texture
    
    vec3 lightDir = normalize( light.TangentLightPos - fragPos);//Ricavo la direzione da cui proviene la luce e la normalizzo perchè  versore
    float diff = max(dot(normal, lightDir), 0.0);// ricavo cos dell'angolo tra la norma e la direzione della luce', mi dice come la sup è orientata rispetto alla luce
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;  
    //Idiff=Ilucediff*K_materialdiff*cos(theta)

    // specular
    
    vec3 reflectDir = reflect(-lightDir, normal);//ricavo direzione raggio riflesso, grazie alla direzione da cui proviene la luce e la normale alla sup
    //-L+2(L*N)*N
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);//come è orientata la sup e quanto forte è la mia riflessione, il mio spot
    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;  
    
    // attenuation
    float distance = length(light.TangentLightPos - fragPos);// distanza tra la luce e il frammento
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    //sommo le mie tre componenti per ottenre il valore finale della luce 
    return (ambient + diffuse + specular);
}

// calcolo del lighting quando ho luce spot
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //ambientale
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

    //luce diffusiva:
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);//come è orientata la sup
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    
    // luce speculare
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    // attenuazione in base alla distanza dall'oggetto
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // spotlight intensity: in base a dove e come il mio cono di luce colpisce 
    float theta = dot(lightDir, normalize(-light.direction)); //prodotto scalare tra dove punto la luce e dove la luce colpisce --cos
    float epsilon = light.cutOff - light.outerCutOff;// di quanto il cono esterno è piu grande 
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);//intensità in base se sono nel cono no 
    //nel cono =1 fuori =0 e tra esterno ed interno medio tra 0 e 1
    // combine results
    
    
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
    