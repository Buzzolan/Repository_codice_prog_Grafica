#include <glad/glad.h>
#include <GLFW/glfw3.h>//per gestire finestre etc..
#include<iostream>
using namespace std;


//per shader semplici posso passarli sottoforma di stringa
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"//variabile di output con key word 'out'
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//ad ogni frammento genere un colore fissato
//variabile di 4 elementi colore e livello di trasparenza
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);// callback ridimensionamento finestra
void processInput(GLFWwindow* window);

int main() {
	float red;
	//codice standard per inizializzare il progetto
	glfwInit();//inizializzo glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);// configuro glfw settando la versione
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);// definisco la versione opengl da usare, cioè la 3.3.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// tipo di profilo (core)= nuova versione

	//creo l'oggetto finestra e richiede la dimensione di essa, il terzo elemento è il nome delle finestra
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)// gestisco se la finestra non è stata creata
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);// il nostro contesto farà riferimento a quella finestra

	//inizializzo GLAD prima di chiamare funzioni openGl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		/*passiamo GLAD alla funzione per caricare l'indirizzo dei puntatoria alla funzione 
		OpenGL che è specifico del SO. GLFW ci fornisce ProcAdress che definisce
		la funzione corretta in base al SO per cui stiamo compilando*/
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}
	// primi due parametri è posizione angolo inferiore sx della finestra
	//stesse dim della finestra, setta il viewport
	glViewport(0, 0, 800, 600);

	// chiamo la funzione su ogni ridemensionamento della finestra
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	
	//definsico vertexshader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);// creo vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // associo il vertex shader al sorgente
	glCompileShader(vertexShader);//compila a run time il vertex shader

	//definisco frament shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//ore ho i due shader compilati e devo lincarli al programma:
	//creo shader program che mette inseieme più shader e poi viene collegato al resto del programma
	// l'output di uno shader è l'input per quello successivo

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//creo il programa

	glAttachShader(shaderProgram, vertexShader);//attacco i vari shader
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//faccio il link tra gli shader attaccati

	

	//ora devo cancellare gli oggetti shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
	   0.0f, -0.5f, 0.0f,  // left
	   0.9f, -0.5f, 0.0f,  // right
	   0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(2, VBOs);
	// first triangle setup
	// --------------------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
	// second triangle setup
	// ---------------------
	glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glEnableVertexAttribArray(0);
	// glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)



	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
																
    // creo render loop
	while (!glfwWindowShouldClose(window))// controllo che la finestra non sia stata chiusa ogni volta che inizio il loop
	{	
		//render loop
		processInput(window);//close window

		//renderig commands
		//red = (1 + sin(glfwGetTime())) / 2.0;
		glClearColor(0.2, 0.2f, 0.2f, 1.0f);//rosso, verde, blu
		glClear(GL_COLOR_BUFFER_BIT);// aggiorno buffer del colore

		glUseProgram(shaderProgram); //lo vado ad attivare
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// check and call events and swap the buffer
		glfwSwapBuffers(window);// mando nella finestra quello che nel buffer
		//ho doppio buffer e quindi faccio swap
		glfwPollEvents();// intercetto eventi, es: ridimensione finestra, tastiere, movimenti mouse
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);

	glfwTerminate();// pulisco/cancello tutte le risorse allocate per GLFW
	return 0;

}

void processInput(GLFWwindow* window)// chiudo finestra se premo esc
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)// aggiorno viewport
{
	glViewport(0, 0, width, height);
}