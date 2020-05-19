#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacionMadera();
void animacionPuerta();
void animacionSillon();
void animacionCajon();
void animacionAntena();
void animacionLibros();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

//Animación del dispensador de madera
float movCaida = -2.5f; //Movimiento de caída de dispensador o consumo de la madera
bool activo = false; //Indica si se activa la animación
bool seleccion = false; //Indica qué movimiento se realizará

//Animación de la puerta
float rotP = 180.0f; //Rotación que realiza
bool activoP = false; //Indica si se activa la animación
bool abierta = false; //Indica si la puerta está abierta o cerrada

//Animación del sillón verde
float desplazamiento = -100.0f; //Desplazamiento
bool activoS = false; //Indica si se activa la animación
bool desplazado = false; //Indica si ya se movió

//Animación de los cajones
float movi = -106.7f; //Desplazamiento
bool activoC = false; //Indica si se activa la animación
bool dentro = true; //Indica si el cajón está adentro o afuera

//Animación de la antena
glm::vec3 antMov(-105.3f, -2.09f, -85.2f); //Desplazamiento
bool antActX = false; //Indica si se activa la animación para moverla en X
bool antActZ = false; //Indica si se activa la animación para moverla en Z
bool antActC = false; //Indica si se activa la animación para centrarla
bool antDX = false; //Indica si ya se movió en X
bool antDZ = false; //Indica si ya se movíó en Z

//Animación de los libros
float libMov = -106.77f; //Desplazamiento
bool libAct = false; //Indica si se activa la animación
bool libDes = false; //Indica si ya se movieron los libros

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting4.frag");
	Shader SkyBoxShader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	
	Model Casa((char*)"Models/Casa/casa.obj");
	Model Puerta((char*)"Models/Puerta/puerta.obj");
	Model Buro((char*)"Models/Buro/buro.obj");
	Model Cajon((char*)"Models/Cajon/cajon.obj");
	Model Chimenea((char*)"Models/Chimenea/chimenea.obj");
	Model Madera((char*)"Models/Madera/madera.obj");
	Model Piano((char*)"Models/Piano/piano.obj");
	Model SillonVerde((char*)"Models/SillonVerde/sillonVerde.obj");
	Model SillonCafe((char*)"Models/SillonCafe/sillonCafe.obj");
	Model Lampara((char*)"Models/Lampara/lampara.obj");
	Model LamparaP((char*)"Models/LamparaPiso/lamparaP.obj");
	Model TV((char*)"Models/TV/TV.obj");
	Model CajaCable((char*)"Models/Caja del cable/cajaCable.obj");
	Model Antena((char*)"Models/Antena/antena.obj");
	Model Bocina((char*)"Models/Bocina/bocina.obj");
	Model Librero((char*)"Models/Librero/librero.obj");
	Model Libros((char*)"Models/Libros/libros.obj");

	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/derecha.tga");
	faces.push_back("SkyBox/izquierda.tga");
	faces.push_back("SkyBox/superior.tga");
	faces.push_back("SkyBox/inferior.tga");
	faces.push_back("SkyBox/atras.tga");
	faces.push_back("SkyBox/frente.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	GLuint texture1;
	glGenTextures(1, &texture1);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("Textura/mesa_cafe.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacionMadera();
		animacionPuerta();
		animacionSillon();
		animacionCajon();
		animacionLibros();
		animacionAntena();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f,1.0f,1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 100.0f, 50.0f, 100.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 10.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 20.0f);

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);
		glm::mat4 model(1);

		//Mesa
		model = glm::translate(model, glm::vec3(-106.8f, -1.47f, -76.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.9f, 0.05f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pata 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-107.34f, -1.795f, -77.08f));
		model = glm::scale(model, glm::vec3(0.05f, 0.6f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pata 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.276f, -1.795f, -77.08f));
		model = glm::scale(model, glm::vec3(0.05f, 0.6f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pata 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-107.34f, -1.795f, -75.32f));
		model = glm::scale(model, glm::vec3(0.05f, 0.6f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pata 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.276f, -1.795f, -75.32f));
		model = glm::scale(model, glm::vec3(0.05f, 0.6f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Soporte 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.81f, -1.69f, -77.08f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.02f, 1.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Soporte 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.81f, -1.69f, -75.32f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.02f, 1.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Carga de modelos
		//Casa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-100.0f, -2.5f, -80.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casa.Draw(lightingShader);

		//Piano
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-105.7f, -2.11f, -76.9f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piano.Draw(lightingShader);

		//Chimenea
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-99.97f, -2.497f, -80.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Chimenea.Draw(lightingShader);

		//Sillón Verde
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(desplazamiento, -2.51f, -80.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillonVerde.Draw(lightingShader);

		//Madera
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-100.0f, movCaida, -79.99f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Madera.Draw(lightingShader);

		//Buro 1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.7f, -2.105f, -75.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Buro.Draw(lightingShader);

		//Cajón 1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(movi, -2.705f, -75.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajon.Draw(lightingShader);

		//Lámpara 1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-102.3f, -2.105f, -77.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);

		//Silón Café
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.8f, -2.11f, -76.5f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillonCafe.Draw(lightingShader);

		//Buro 2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.7f, -2.105f, -69.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Buro.Draw(lightingShader);

		//Cajón 2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(movi, -2.705f, -69.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajon.Draw(lightingShader);

		//Lámpara 2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-102.3f, -2.105f, -71.1f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);

		//Puerta
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-99.5533f, 0.0548f, -70.8918f));
		model = glm::rotate(model, glm::radians(rotP), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		//Sillón Café (Segundo Cuarto)
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-104.8f, -2.11f, -85.7f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillonCafe.Draw(lightingShader);

		//TV
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-105.3f, -2.09f, -85.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TV.Draw(lightingShader);

		//Caja del cable
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-105.3f, -2.09f, -85.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CajaCable.Draw(lightingShader);

		//Antena
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(antMov.x, -2.09f, antMov.z));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Antena.Draw(lightingShader);

		//Lámpara de piso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-104.81f, -2.099f, -85.8f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LamparaP.Draw(lightingShader);

		//Bocina 1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.5f, -2.096f, -83.9f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.Draw(lightingShader);

		//Librero
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-106.5f, -2.096f, -83.9f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Librero.Draw(lightingShader);

		//Libros
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(libMov, -2.096f, -83.9f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Libros.Draw(lightingShader);

		//Bocina 2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-109.99f, -2.096f, -83.9f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.Draw(lightingShader);

		glBindVertexArray(0);

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	//Activa la animación de la madera
	if (keys[GLFW_KEY_R])
	{
		activo = true;
	}

	//Activa la animación de los cajones
	if (keys[GLFW_KEY_F])
	{
		activoC = true;
	}

	//Activa la animación del sillón verde
	if (keys[GLFW_KEY_E])
	{
		activoS = true;
	}

	//Activa la animación de la puerta
	if (keys[GLFW_KEY_SPACE])
	{
		activoP = true;
	}

	//Activa la animación de los libros
	if (keys[GLFW_KEY_G])
	{
		libAct = true;
	}

	//Activa el desplazamiento de la antena en X
	if (keys[GLFW_KEY_X])
	{
		antActX = true;
	}

	//Activa el desplazamiento de la antena en Z
	if (keys[GLFW_KEY_Z])
	{
		antActZ = true;
	}

	//Activa el desplazamiento de la antena para centrarla
	if (keys[GLFW_KEY_C])
	{
		antActC = true;
	}
}

//Animación del dispensador de madera
void animacionMadera() 
{
	if (activo)
	{
		if (seleccion)
		{
			if (movCaida < -2.5)
			{
				movCaida = -2.5f;
				activo = false;
				seleccion = false;
			}
			else
			{
				movCaida -= 0.05f;
			}
		}
		else
		{
			if (movCaida < -3.051)
			{
				movCaida = -0.823f;
				activo = false;
				seleccion = true;
			}
			else
			{
				movCaida -= 0.05f;
			}
		}
	}
}

//Animación Sillón Verde
void animacionSillon() {
	if (activoS) 
	{
		if (desplazado)
		{
			if (dentro) 
			{
				if (desplazamiento > -100)
				{
					desplazamiento = -100.0f;
					activoS = false;
					desplazado = false;
				}
				else
				{
					desplazamiento += 0.05f;
				}
			}
			else
			{
				activoS = false;
			}
		}
		else
		{
			if (desplazamiento < -101.68)
			{
				desplazamiento = -101.68f;
				activoS = false;
				desplazado = true;
			}
			else
			{
				desplazamiento -= 0.05f;
			}
		}
	}
}

//Animación de los cajones
void animacionCajon()
{
	if (activoC)
	{
		if (dentro)
		{
			if (desplazado) 
			{
				if (movi < -107.64)
				{
					movi = -107.64f;
					activoC = false;
					dentro = false;
				}
				else
				{
					movi -= 0.05f;
				}
			}
			else
			{
				activoC = false;
			}
		}
		else
		{
			if (movi > -106.7)
			{
				movi = -106.7f;
				activoC = false;
				dentro = true;
			}
			else
			{
				movi += 0.05f;
			}
		}
	}
}

//Animación de la puerta
void animacionPuerta()
{
	if (activoP)
	{
		if (abierta)
		{
			if (rotP > 180)
			{
				rotP = 180.0f;
				activoP = false;
				abierta = false;
			}
			else
			{
				rotP += 1.0f;
			}
		}
		else
		{
			if (rotP < 90)
			{
				rotP = 90.0f;
				activoP = false;
				abierta = true;
			}
			else
			{
				rotP -= 1.0f;
			}
		}
	}
}

//Animación de los libros
void animacionLibros()
{
	if (libAct)
	{
		if (libDes)
		{
			if (libMov < -106.77)
			{
				libMov = -106.77f;
				libAct = false;
				libDes = false;
			}
			else
			{
				libMov -= 0.05f;
			}
		}
		else
		{
			if (libMov > -105.03)
			{
				libMov = -105.03f;
				libAct = false;
				libDes = true;
			}
			else
			{
				libMov += 0.05f;
			}
		}
	}
}

//Animación de la antena
void animacionAntena()
{
	if (antActX)
	{
		if (antDX)
		{
			if (antMov.x < -105.45)
			{
				antMov.x = -105.45f;
				antActX = false;
				antDX = false;
			}
			else
			{
				antMov.x -= 0.01f;
			}
		}
		else
		{
			if (antMov.x > -105.17)
			{
				antMov.x = -105.17f;
				antActX = false;
				antDX = true;
			}
			else
			{
				antMov.x += 0.01f;
			}
		}
	}
	else if (antActZ)
	{
		if (antDZ)
		{
			if (antMov.z < -85.59)
			{
				antMov.z = -85.59f;
				antActZ = false;
				antDZ = false;
			}
			else
			{
				antMov.z -= 0.01f;
			}
		}
		else
		{
			if (antMov.z > -84.79)
			{
				antMov.z = -84.79f;
				antActZ = false;
				antDZ = true;
			}
			else
			{
				antMov.z += 0.01f;
			}
		}
	}
	else if (antActC)
	{
		if (antMov.x == -105.3f && antMov.z == -85.2f)
		{
			antActC = false;
		}
		else
		{
			if (antDX)
			{
				if (antMov.x < -105.3)
				{
					antMov.x = -105.3f;
					antDX = false;
				}
				else
				{
					antMov.x -= 0.01f;
				}
			}
			else
			{
				if (antMov.x > -105.3)
				{
					antMov.x = -105.3f;
					antDX = false;
				}
				else
				{
					antMov.x += 0.01f;
				}
			}

			if (antDZ)
			{
				if (antMov.z < -85.2)
				{
					antMov.z = -85.2f;
					antDZ = false;
				}
				else
				{
					antMov.z -= 0.01f;
				}
			}
			else
			{
				if (antMov.z > -85.2)
				{
					antMov.z = -85.2f;
					antDZ = false;
				}
				else
				{
					antMov.z += 0.01f;
				}
			}
		}
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}