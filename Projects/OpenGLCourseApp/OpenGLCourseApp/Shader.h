#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

    std::string ReadFile(const char* fileLocation);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();
    GLuint GetEyePositionLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColorLocation();
    GLuint GetAmbientDiffuseLocation();
    GLuint GetDirection();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininess();

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID; 
    GLuint uniformProjection;
    GLuint uniformModel;
    GLuint uniformView;
    GLuint uniformEyePosition;
    GLuint uniformAmbientIntensity;
    GLuint uniformAmbientColor;
    GLuint uniformDiffuseIntensity;
    GLuint uniformDirection;
    GLuint uniformSpecularIntensity; 
    GLuint uniformShininess;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

