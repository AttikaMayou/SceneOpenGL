//
//
//

// GLEW_STATIC force le linkage statique
// c-a-d que le code de glew est directement injecte dans l'executable
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ViewerObj.h"

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

#include <iostream>
#include "GLShader.h"
#include "VertexCustomClass.h"
#include "MeshCustomClass.h"

GLShader BasicShader;
GLuint VBO;
GLuint VBO_DIFFUSE;
GLuint IBO;
GLuint VAO;
Mesh m;

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

	glGenBuffers(2, &VBO_DIFFUSE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DIFFUSE);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), diffuseLightVertex, GL_STATIC_DRAW);

	int diffuse_loc = glGetAttribLocation(BasicShader.GetProgram(), "a_diffuse_position");
	glVertexAttribPointer(diffuse_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _position));
	glEnableVertexAttribArray(diffuse_loc);

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
	glDeleteBuffers(2, &VBO_DIFFUSE);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);

}

void Display(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Definit le viewport en pleine fenetre
	glViewport(0, 0, width, height);
	
	uint32_t basicProgram = BasicShader.GetProgram();
	glUseProgram(basicProgram);

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
		0.0f, 0.0f, 1.0f, 1.0f
	};
	int matrice_loc = glGetUniformLocation(basicProgram, "u_matrix");
	glUniformMatrix4fv(matrice_loc, 1, GL_FALSE, matrice);

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