#include <glad/glad.h>
#include <GLFW/glfw3.h>//per gestire finestre etc..
#define STB_IMAGE_IMPLEMENTATION // per definire l'header in modo che contenga solo la parte rilevante
#include <stb_image.h>

#include<iostream>
#include <shader_s.h> //header dello shader
#include <stdio.h>      /* printf */
#include <math.h>  
using namespace std;


//prima qui avevo i miei shader come stringhe, ora ho un file apposito

void framebuffer_size_callback(GLFWwindow* window, int width, int height);// callback ridimensionamento finestra
void processInput(GLFWwindow* window);

int main() {

	//codice standard per inizializzare il progetto
	glfwInit();//inizializzo glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);// configuro glfw settando la versione
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);// definisco la versione opengl da usare, cioè la 3.3.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// tipo di profilo (core)= nuova versione

	//creo l'oggetto finestra e richiede la dimensione di essa, il terzo elemento è il nome delle finestra
	GLFWwindow* window = glfwCreateWindow(800, 600, "different wrapping methods", NULL, NULL);

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

	// build and compile our shader program, faccio il link con il programma
	// ------------------------------------
	Shader ourShader("Capitolo_1/esercizi_texture/4.1.shader_texture.vs", "Capitolo_1/esercizi_texture/4.1.shader_texture.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions      // colors         // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//gestione della memoria da CPU a GPU, grazie ad opengl ora aggiungo un buffer
	unsigned int VBO, VAO, EBO;// il buffer ha id unico, puntatore al buffer
	glGenVertexArrays(1, &VAO);// genero VAO
	glGenBuffers(1, &VBO);// genera vbo
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//faccio binding, associo al vbo una porzione
	//di memoria che definisco essere buffer di array(più comodo per vertici)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// dico dimensione della memoria da allocare e li passo la struttura
	//va a copiare il contenuto della memoria principale dove viene allocata la memoria per i vertici e lo copia nella gpu

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);// associo memoria a EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);// vado a dire come sono fatti i trinagoli

	//devo lincare gli attributi dei vertici(pos, color, etc..)
	//il vertex shader non sa come prendere i dati dal vertex buffer object
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// genero la texture
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1); // primo input è quante texture uso, e le carica in un unsigned int dato come secono argomento
	// faccio il bind
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//carico l'immagine
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data) //gestisco il corretto caricamento dell'immagine
	{
		//genero la mia texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);// pag 60
		glGenerateMipmap(GL_TEXTURE_2D); // attivo mipmap
		// ora l'oggetto texture ha l'immagine allegata ad essa
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);// libero la memoria dell'immagine

	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to(only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	//due modi alternativi per settarela variabile uniform texture
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	//ourShader.ID = vertex + fragment
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	// creo render loop
	while (!glfwWindowShouldClose(window))// controllo che la finestra non sia stata chiusa ogni volta che inizio il loop
	{
		//render loop
		processInput(window);//close window

		//renderig commands

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//rosso, verde, blu
		glClear(GL_COLOR_BUFFER_BIT);// aggiorno buffer del colore

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffer
		glfwSwapBuffers(window);// mando nella finestra quello che nel buffer
		//ho doppio buffer e quindi faccio swap
		glfwPollEvents();// intercetto eventi, es: ridimensione finestra, tastiere, movimenti mouse
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


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