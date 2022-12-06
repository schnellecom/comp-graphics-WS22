/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                            DO NOT EDIT THIS FILE!                         *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

#include <fstream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

/// decide whether to build solution or task. only works if you have the solution file :^)
#if VIEW_SOLUTIONS
using namespace solution;
#else
using namespace task;
#endif

namespace
{
auto windowWidth = 1024u;
auto windowHeight = 1024u;
int vSync = 1;
int current_key_press;

GLFWwindow* window;

// assignment specific variables:
GLuint teapot_vbo;
GLuint teapot_ea;
GLuint vao;
}

glm::mat4 modelViewMatrix;
glm::mat4 projectionMatrix;

auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void
{
    (void)mods;
    (void)scancode;
    if (action == GLFW_PRESS || action == GLFW_REPEAT || action == GLFW_KEY_DOWN)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        else if (key == GLFW_KEY_V)
        {
            vSync = (vSync + 1) % 2;
            std::cout << "vsync is ";
            if (vSync)
                std::cout << "on";
            else
                std::cout << "off";
            std::cout << std::endl;
            glfwSwapInterval(vSync);
        }
        else
            current_key_press = key;
    }
}


std::string getFileContent(const std::string& fileName)
{
    std::string line = "";
    std::string fileContent = "";

    std::ifstream fileStream(fileName.c_str(), std::ifstream::in);

    if (fileStream.is_open())
    {
        while (fileStream.good())
        {
            std::getline(fileStream, line);
            fileContent += line + "\n";
        }
        fileStream.close();
    }
    else
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
        exit(-1);
    }
    return fileContent;
}

glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far)
{
    float phiHalfInRadians = 0.5f * phiInDegree * (M_PI / 180.0f);
    float t = near * tan(phiHalfInRadians);
    float b = -t;
    float left = b * aspectRatio;
    float right = t * aspectRatio;

    return glm::frustum(left, right, b, t, near, far);
}

void resizeCallback(int newWidth, int newHeight)
{
    // define the part of the screen OpenGL should draw to (in pixels):
    glViewport(0, 0, newWidth, newHeight);
    projectionMatrix = buildFrustum(70.0f, (newWidth / (float)newHeight), 0.1f, 100.0f);
}

void setupShaderUniforms(GLuint _program, int _scene)
{
    GLint projectionMatrixLocation = -1;
    GLint modelViewMatrixLocation = -1;
    GLint lightPosition = -1;
    GLint lightColor = -1;
    GLint lightSpotLightFactor = -1;
    GLint ambientMaterial = -1;
    GLint diffuseMaterial = -1;
    GLint specularMaterial = -1;
    GLint specularityExponent = -1;

    // Get uniform locations
    projectionMatrixLocation = glGetUniformLocation(_program, "uProjectionMatrix");
    modelViewMatrixLocation = glGetUniformLocation(_program, "uModelViewMatrix");
    lightPosition = glGetUniformLocation(_program, "uLightPosition");
    lightColor = glGetUniformLocation(_program, "uLightColor");
    lightSpotLightFactor = glGetUniformLocation(_program, "uLightSpotLightFactor");
    ambientMaterial = glGetUniformLocation(_program, "uAmbientMaterial");
    diffuseMaterial = glGetUniformLocation(_program, "uDiffuseMaterial");
    specularMaterial = glGetUniformLocation(_program, "uSpecularMaterial");
    specularityExponent = glGetUniformLocation(_program, "uSpecularityExponent");

    // Set uniform data
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    if (_scene != 3)
    {
        glUniform3fv(lightPosition, 1, glm::value_ptr(vLightPosition1));
        glUniform3fv(lightColor, 1, glm::value_ptr(vLightColor1));
        glUniform1f(lightSpotLightFactor, fLightSpotLightFactor1);
    }
    else
    {
        glUniform3fv(lightPosition, 1, glm::value_ptr(vLightPosition2));
        glUniform3fv(lightColor, 1, glm::value_ptr(vLightColor2));
        glUniform1f(lightSpotLightFactor, fLightSpotLightFactor2);
    }
    glUniformMatrix3fv(ambientMaterial, 1, GL_FALSE, glm::value_ptr(mAmbientMaterial));
    glUniformMatrix3fv(diffuseMaterial, 1, GL_FALSE, glm::value_ptr(mDiffuseMaterial));
    glUniformMatrix3fv(specularMaterial, 1, GL_FALSE, glm::value_ptr(mSpecularMaterial));
    glUniform1f(specularityExponent, mSpecularityExponent);
}

void createShaderProgram(GLuint& vs, GLuint& fs, GLuint& prog, const std::string& vsFileName, const std::string& fsFileName)
{
    // create shader & shader program:
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    prog = glCreateProgram();

    // compile sources and add to shader program:
    std::string vsrc = getFileContent(vsFileName);
    const char* vsrc_c = vsrc.c_str();
    glShaderSource(vs, 1, &vsrc_c, NULL);
    glCompileShader(vs);
    common::checkCompileErrors(vs);
    glAttachShader(prog, vs);

    std::string fsrc = getFileContent(fsFileName);
    const char* fsrc_c = fsrc.c_str();
    glShaderSource(fs, 1, &fsrc_c, NULL);
    glCompileShader(fs);
    common::checkCompileErrors(fs);
    glAttachShader(prog, fs);

    // Bind attrib locations:
    // NOTE: trying to bind locations that are not present in the shader might result in
    // warnings but should not be problematic, so we just try to bind all attributes here
    // independent on the actual shader!
    glBindAttribLocation(prog, 0, "aPosition");
    glBindAttribLocation(prog, 1, "aNormal");

    // link shader program (remember to do this _after_ binding attrib. locations!):
    glLinkProgram(prog);
    common::checkLinkErrors(prog);
}

bool prepareExercise()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create VBO:
    glGenBuffers(1, &teapot_vbo);

    // upload data to graphic card:
    glBindBuffer(GL_ARRAY_BUFFER, teapot_vbo);
    glBufferData(GL_ARRAY_BUFFER, g_numberOfTeapotVertices * g_teapotVertexStrideSize, (void*)g_teapotVertices, GL_STATIC_DRAW);

    // create element array for the elements
    glGenBuffers(1, &teapot_ea);

    // upload data to graphic card:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapot_ea);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_numberOfTeapotTriangles * g_teapotElementStrideSize, (void*)g_teapotElements, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);

    // Vertex positions in attrib 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, g_teapotVertexStrideSize, 0);
    glEnableVertexAttribArray(0);

    // Vertex normals in attrib 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, g_teapotVertexStrideSize, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    return true;
}

void cleanupExercise()
{
    glDeleteBuffers(1, &teapot_vbo);
    glDeleteBuffers(1, &teapot_ea);

    glDeleteVertexArrays(1, &vao);
}

void callStudentCode(int sceneToDraw, double runTime) { drawScene(sceneToDraw, runTime); }

void resizeCallback(GLFWwindow*, int newWidth, int newHeight)
{
    windowWidth = newWidth;
    windowHeight = newHeight;

    resizeCallback(windowWidth, windowHeight);
}


int main(int /*argc*/, char* /*argv*/[])
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "[Error] Init of GLFW failed. Terminating." << std::endl;
        glfwTerminate();
        return -1;
    }

    window = common::createWindow(windowWidth, windowHeight, "Basic Techniques in Computer Graphics - Assignment 8");

    if (!window)
        std::cerr << "[Error] Window could not be created!" << std::endl;

    if (!common::init(window))
    {
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, key_callback);

    // Enable vertical sync (on cards that support it)
    // vertical sync
    glfwSwapInterval(vSync);

    int sceneToDraw = 1;
    glGetError(); // clear errors

    if (!prepareExercise())
        exit(-1);

    initCustomResources();
    resizeCallback(windowWidth, windowHeight);

    double startTimeInSeconds = glfwGetTime();
    do
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double runTime = glfwGetTime() - startTimeInSeconds;
        callStudentCode(sceneToDraw, runTime);

        // Swap buffers
        glfwSwapBuffers(window);

        if (current_key_press == GLFW_KEY_A)
            sceneToDraw = 1;
        else if (current_key_press == GLFW_KEY_B)
            sceneToDraw = 2;
        else if (current_key_press == GLFW_KEY_C)
            sceneToDraw = 3;
        else if (current_key_press == GLFW_KEY_D)
            sceneToDraw = 4;

    } // Check if the window was closed
    while (!glfwWindowShouldClose(window));

    // clean up:
    deleteCustomResources();
    cleanupExercise();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    exit(0);
}
