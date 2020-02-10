//
//
//

// GLEW_STATIC force le linkage statique
// c-a-d que le code de glew est directement injecte dans l'executable
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ViewerObj.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// les repertoires d'includes sont:
// ../libs/glfw-3.3/include			fenetrage
// ../libs/glew-2.1.0/include		extensions OpenGL
// ../libs/stb						gestion des images (entre autre)

// les repertoires des libs sont (en 64-bit):
// ../libs/glfw-3.3/lib-vc2015
// ../libs/glew-2.1.0/lib/Release/x64

// Pensez a copier les dll dans le repertoire x64/Debug, cad:
// glfw-3.3/lib-vc2015/glfw3.dll
// glew-2.1.0/bin/Release/x64/glew32.dll		si pas GLEW_STATIC

// _WIN32 indique un programme Windows
// _MSC_VER indique la version du compilateur VC++
#if defined(_WIN32) && defined(_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")			// glew32.lib si pas GLEW_STATIC
#pragma comment(lib, "opengl32.lib")
#elif defined(__APPLE__)
#elif defined(__linux__)
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
using namespace std;

#include "GLShader.h"
#include "VertexCustomClass.h"
#include "MeshCustomClass.h"

GLShader BasicShader;
GLuint VBO;
GLuint VBO_DIFFUSE;
GLuint IBO;
GLuint VAO;
Mesh m;
GLuint g_TextureBatman;

GLuint LoadTexture(const char* path) 
{
	int w, h, c;
	uint8_t* data = stbi_load(path, &w, &h, &c, STBI_rgb_alpha);
	if (!data) 
	{
		cout << "Failed to load texture at path : " << path << endl;
		return 0;
	}
	GLuint TextureID = 0;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return TextureID;
}

void DestroyTexture(GLuint* textureID) 
{
	glDeleteTextures(1, textureID);
	textureID = 0;
}

void Initialize()
{
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "erreur d'initialisation de GLEW!"
			<< std::endl;
	}

	std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;


	BasicShader.LoadVertexShader("basic.vs");
	BasicShader.LoadFragmentShader("basic.fs");
	BasicShader.Create();

	ViewerObj viewer;
	viewer.LoadObj(m);

	g_TextureBatman = LoadTexture("batman_logo.png");

	Vertex* diffuseLightVertex = new Vertex[1];
	diffuseLightVertex[0]._position._x = 0.0;
	diffuseLightVertex[0]._position._y = 0.0;
	diffuseLightVertex[0]._position._z = 1.0;
	diffuseLightVertex[0]._color._x = 1.0;
	diffuseLightVertex[0]._color._y = 0.0;
	diffuseLightVertex[0]._color._z = 0.0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.getVertexCount(), m.getVertices(), GL_STATIC_DRAW);

	int loc_position = glGetAttribLocation(BasicShader.GetProgram(), "a_position");
	glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _position));
	glEnableVertexAttribArray(loc_position);

	int color_loc = glGetAttribLocation(BasicShader.GetProgram(), "a_color");
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _color));
	glEnableVertexAttribArray(color_loc);

	int normal_loc = glGetAttribLocation(BasicShader.GetProgram(), "a_normal");
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normal));
	glEnableVertexAttribArray(normal_loc);

	int text_coord_loc = glGetAttribLocation(BasicShader.GetProgram(), "a_text_coord");
	glVertexAttribPointer(text_coord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _textcoords));
	glEnableVertexAttribArray(text_coord_loc);

	glGenBuffers(1, &VBO_DIFFUSE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DIFFUSE);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), diffuseLightVertex, GL_STATIC_DRAW);

	int diffuse_color_loc = glGetAttribLocation(BasicShader.GetProgram(), "a_diffuse_color");
	glVertexAttribPointer(diffuse_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _color));
	glEnableVertexAttribArray(diffuse_color_loc);

	glBindVertexArray(0);
#ifdef WIN32
	wglSwapIntervalEXT(1);
#endif // WIN32

}

void Shutdown()
{
	BasicShader.Destroy();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_DIFFUSE);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
	DestroyTexture(&g_TextureBatman);
}

float DotProduct(Vector3 vector1, Vector3 vector2) {
	int product = (vector1._x * vector2._x) + (vector1._y * vector2._y) + (vector1._z * vector2._z);
	return product;
}

Vector3 CrossProduct(Vector3 vector1, Vector3 vector2) {
	Vector3 cross;

	cross._x = vector1._y * vector2._z - vector1._z * vector2._y;
	cross._y = vector1._x * vector2._z - vector1._z * vector2._x;
	cross._z = vector1._x * vector2._y - vector1._y * vector2._x;

	return cross;
}

Vector3 Normalize(Vector3 vec)
{
	Vector3 norm;
	float length = sqrt((vec._x * vec._x) + (vec._y * vec._y) + (vec._z * vec._z));

	norm._x = vec._x / length;
	norm._y = vec._y / length;
	norm._z = vec._z / length;
	
	return norm;
}

float *LookAt(Vector3 position, Vector3 target, Vector3 up) {
	Vector3 camForward;
	camForward._x = position._x - target._x;
	camForward._y = position._y - target._y;
	camForward._z = position._z - target._z;

	camForward = Normalize(camForward);

	Vector3 camRight = CrossProduct(up, camForward);

	camRight = Normalize(camRight);

	Vector3 camUp = CrossProduct(camForward, camRight);

	float scalePositionUp = DotProduct(position, camUp);
	float scalePositionFront = DotProduct(position, camForward);
	float scalePositionRight = DotProduct(position, camRight);

	float matrice[] = {
		camRight._x,	camRight._y,	camRight._z,	-scalePositionRight,
		camUp._x,		camUp._y,		camUp._z,		-scalePositionUp,
		camForward._x,	camForward._y,	camForward._z,	-scalePositionFront,
		0.0f,			0.0f,			0.0f,			1.0f
	};

	return matrice;
}

void Display(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	int textureUnit = 0;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_TextureBatman);

	Vector3 cameraPos = Vector3(0.0f, 0.0f, 10.0f);
	Vector3 cameraTarget = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

	float *viewMatrice = LookAt(cameraPos, cameraTarget, cameraUp);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Definit le viewport en pleine fenetre
	glViewport(0, 0, width, height);
	uint32_t basicProgram = BasicShader.GetProgram();
	glUseProgram(basicProgram);

	int texture_location = glGetUniformLocation(basicProgram, "u_texture_sampler");
	glUniform1i(texture_location, textureUnit);

	float currentTime = (float)glfwGetTime();
	int time_loc = glGetUniformLocation(basicProgram, "u_time");
	glUniform1f(time_loc, currentTime);

	float intensity = 0.5;
	int intensity_loc = glGetUniformLocation(basicProgram, "u_ambient_intensity");
	glUniform1f(intensity_loc, intensity);

	float matrice[] = { 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float aspect = width / height;
	float fovy = 100 * (M_PI / 180);
	float f = 1 / tan(fovy / 2);
	float far = 1000.0;
	float near = 5.0;

	float projMatrice[] = {
		f / aspect,		0.0,	0.0,						0.0,
		0.0,			f,		0.0,						0.0,
		0.0,			0.0,	-far/(near-far),	-(near * far)/ (near - far),
		0.0,			0.0,	-1.0,						0.0,
	};

	int matrice_loc = glGetUniformLocation(basicProgram, "u_matrix");
	glUniformMatrix4fv(matrice_loc, 1, GL_FALSE, matrice);

	int matriceViewLoc = glGetUniformLocation(basicProgram, "u_view_matrix");
	glUniformMatrix4fv(matriceViewLoc, 1, GL_FALSE, viewMatrice);

	int matriceProjLoc = glGetUniformLocation(basicProgram, "u_projection_matrix");
	glUniformMatrix4fv(matriceProjLoc, 1, GL_FALSE, projMatrice);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glDrawArrays(GL_TRIANGLES, 0, m.getVertexCount());
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// toutes nos initialisations vont ici
	Initialize();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		Display(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// ne pas oublier de liberer la memoire etc...
	Shutdown();

	glfwTerminate();
	return 0;
}