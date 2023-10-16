#include <glad/glad.h>
#include <stdio.h>
#include <iostream>

#include "../headers/helpers.h"
#include "../headers/Shader.h"

int hello_triangle()
{
	// Init glfw
	if (glfwInit() != GL_TRUE)
	{
		printf("Error initializing GLFW!");
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create glfw window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window!");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	Shader shader_1("vertex_shaders/hello_triangles.vs", "fragment_shaders/hello_triangles_1.fs");
	Shader shader_2("vertex_shaders/hello_triangles.vs", "fragment_shaders/hello_triangles_2.fs");

	// Vertex input
	float vertices_1[] = {
		// triangle 1
		-0.5f, 0.5f, 0.0f,	// top 
		-1.0f, -0.5f, 0.0f,	// left
		0.0f, -0.5f, 0.0f	// right
	};

	float vertices_2[] = {
		0.5f, 0.5f, 0.0f,	// top
		0.0f, -0.5f, 0.0f,	// left
		1.0f, -0.5f, 0.0f	// right
	};

	// Bind data for triangle 1
	unsigned int VAO_1;
	glGenVertexArrays(1, &VAO_1);
	glBindVertexArray(VAO_1);

	unsigned int VBO_1;
	glGenBuffers(1, &VBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

	// Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Bind data for triangle 2
	unsigned int VAO_2;
	glGenVertexArrays(1, &VAO_2);
	glBindVertexArray(VAO_2);

	unsigned int VBO_2;
	glGenBuffers(1, &VBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

	// Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		shader_1.Use();

		// Draw triangle 1
		glBindVertexArray(VAO_1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		shader_2.Use();

		// Draw triangle 2
		glBindVertexArray(VAO_2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &VBO_1);
	glDeleteVertexArrays(1, &VAO_2);
	glDeleteBuffers(1, &VBO_2);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
