#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Light mainLight;

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indicesCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticesCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
 
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
    //  x       y       z   u       v   nx      ny    nz
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* object = new Mesh();
    object->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(object);

    Mesh* object_2 = new Mesh();
    object_2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(object_2);
}

void CreateShaders()
{
    Shader* shader = new Shader();
    shader->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader);
}

int main()
{
    mainWindow = GLWindow(1366, 768);
    mainWindow.Initialize();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    brickTexture = Texture((char*)"Textures/brick.png");
    brickTexture.LoadTexture();

    dirtTexture = Texture((char*)"Textures/dirt.png");
    dirtTexture.LoadTexture();

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 
        2.0f, -1.0f, -2.0f, 0.3f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformAmbientIntensity = 0;
    GLuint uniformAmbientColor = 0;
    GLuint uniformDiffuseIntensity = 0;
    GLuint uniformDirection = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.KeyControl(mainWindow.GetKeys(), deltaTime);
        camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        uniformView = shaderList[0]->GetViewLocation();
        uniformEyePosition = shaderList[0]->GetEyePositionLocation();
        uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();
        uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
        uniformDiffuseIntensity = shaderList[0]->GetAmbientDiffuseLocation();
        uniformDirection = shaderList[0]->GetDirection();
        uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
        uniformShininess = shaderList[0]->GetShininess();

        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dirtTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[1]->RenderMesh();

        glUseProgram(0);

        mainWindow.SwapBuffers();
    }

    return 0;
}
