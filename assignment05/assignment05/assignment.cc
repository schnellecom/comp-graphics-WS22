/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

extern int g_bunnyStrideSize;
extern float g_bunnyMesh[];
extern int g_bunnyColorStrideSize;
extern int g_numberOfBunnyVertices;
extern unsigned char g_bunnyColor[];

namespace
{
glm::mat4 VPMatrix(1.f);
glm::mat4 modelViewMatrix(1.f);
glm::mat4 projectionMatrix(1.f);

int windowHeight = 512;
int windowWidth = 512;

glm::vec3 eyePos = glm::vec3(0, 0, -2.3);
glm::vec3 viewDir(0, 0, 0);


// Implicit edge representation for one triangle edge.
// p1 and p2 define the edge.
// Evaluate the function at point.
// If the result is negative, you are inside the triangle edge otherwise outside
float evaluateF(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p)
{
    // =========================================================================
    // Rasterization based on Pineda
    // Assignment section a
    // Add your code here:
    // ====================================================================

    //calculate the line
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float m = dy/dx;
    float t = -p1[0]*m + p1[1];

    float res = p[0]*m + t - p[1];

    return res;

    // ====================================================================
    // End Exercise code
    // ====================================================================
}


// Rasterize one triangle
// the input points are given in counterclockwise order
void drawTriangle(const glm::vec4& p0_in, const glm::vec4& p1_in, const glm::vec4& p2_in, const glm::vec3& normal)
{
    // We use a hardcoded color
    const glm::vec3 color = glm::vec3(0.6, 0.6, 0.6);

    // We work in 2D when rasterizing, ... so we switch to 2D vectors
    const glm::vec2 v0 = glm::vec2(p0_in[0], p0_in[1]);
    const glm::vec2 v1 = glm::vec2(p1_in[0], p1_in[1]);
    const glm::vec2 v2 = glm::vec2(p2_in[0], p2_in[1]);

    // rasterizer initialized from minX to maxX and minY to MaxY
    int minX = floor(p0_in[0]);
    int minY = floor(p0_in[1]);
    int maxX = ceil(p0_in[0]);
    int maxY = ceil(p0_in[1]);

    // Diffuse lighting coefficient
    float diffuse = 1.0;

    // =========================================================================
    // Rasterization based on Pineda. Restrict rasterization range to bounding box and screen
    // Assignment section b
    // Add your code here:
    // ====================================================================
    //for minx
    if(p1_in[0] < (float)minX){
        minX = floor(p1_in[0]);
    }
    else if(p2_in[0] < (float)minX){
        minX = floor(p2_in[0]);
    }
    else if(0 > minX){
        minX = 0;
    }
    //maxX
    if(p1_in[0] > (float)maxX){
        maxX = ceil(p1_in[0]);
    }
    else if(p2_in[0] > (float)maxX){
        maxX = ceil(p2_in[0]);
    }
    else if(windowWidth < maxX){
        maxX = windowWidth;
    }

    //for miny
    if(p1_in[1] < (float)minY){
        minY = floor(p1_in[1]);
    }
    else if(p2_in[1] < (float)minY){
        minY = floor(p2_in[1]);
    }
    else if(0 > minY){
        minY = 0;
    }
    //maxY
    if(p1_in[1] > (float)maxY){
        maxY = ceil(p1_in[1]);
    }
    else if(p2_in[1] > (float)maxY){
        maxY = ceil(p2_in[1]);
    }
    else if(windowHeight < maxY){
        maxY = windowHeight;
    }
    // ====================================================================
    // End Exercise code
    // ====================================================================


    // =========================================================================
    // Rasterization based on Pineda
    // Assignment section f
    // Add your code here:
    // ====================================================================
    diffuse = normal[2];
    if(diffuse<0){
        return;
    }
    // ====================================================================
    // End Exercise code
    // ====================================================================


    // Rasterization
    glm::vec2 p;
    for (p[1] = minY; p[1] <= maxY; p[1]++)
    {
        for (p[0] = minX; p[0] <= maxX; p[0]++)
        {
            // =========================================================================
            // Draw current pixel?
            // Assignment section c
            // Add your code here:
            // ====================================================================

            // Use this function to draw the pixel
            // Do not modify it, just call it if you want to draw the pixel given by p

            if(evaluateF(v0, v1, p)<0) {
                if(evaluateF(v1, v2, p)<0){
                    if(evaluateF(v2, v0, p)<0){
                        setPixel(p[0], p[1], diffuse * color);
                    }
                }
            }

            // ====================================================================
            // End Exercise code
            // ====================================================================
        }
    }

    // ====================================================================
    // End Exercise code
    // ====================================================================
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
}
void task::drawScene(int _scene, float _runTime)
{
    viewDir = glm::normalize(glm::vec3(0, 0, 0) - eyePos);

    // =========================================================================
    // Clear Screen by using the setPixel Function
    // Assignment section e
    // Add your code here:
    // ====================================================================
    int P0;
    int P1;
    for (P1 = 0; P1 <= windowWidth; P1++)
    {
        for (P0 = 0; P0 <= windowHeight; P0++)
        {
            // =========================================================================
            // Draw current pixel?
            // Assignment section c
            // Add your code here:
            // ====================================================================

            // Use this function to draw the pixel
            // Do not modify it, just call it if you want to draw the pixel given by p

            setPixel(P0, P1, glm::vec3(0,0,0));

            // ====================================================================
            // End Exercise code
            // ====================================================================
        }
    }

    // ====================================================================
    // End Exercise code
    // ====================================================================


    // Viewport transformation
    VPMatrix = glm::scale(glm::mat4(1.), glm::vec3(windowWidth / 2.0, windowHeight / 2.0, 1.0)) * glm::translate(glm::mat4(1.), glm::vec3(1.0, 1.0, 0.0));

    // Create modelview matrix
    // First: Create transformation into eye-space
    // glm::lookAt needs the eye position, the center to look at and the up vector
    modelViewMatrix = glm::lookAt(eyePos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // Create a projection matrix
    projectionMatrix = buildFrustum(70.0f, (windowWidth / (float)windowHeight), 0.1f, 100.0f);

    // one vertex consists of 9 floats:
    g_bunnyStrideSize = 9;


    if ((_scene == 2) || (_scene == 3))
    {
        // =========================================================================
        // Let the bunny rotate around y-Axis
        // Assignment section d
        // Add your code here:
        // ====================================================================
//        glm::mat4 rotate = glm::mat4(std::cos(std::floor(_runTime)),     0,  -std::sin(std::floor(_runTime)),  0,
//                                     0,                         1,  0,                       0,
//                                     std::sin(std::floor(_runTime)),     0,  std::cos(std::floor(_runTime)),   0,
//                                     0,0,0,1);
        //modelViewMatrix = modelViewMatrix*rotate;
        modelViewMatrix.operator*=(glm::mat4(std::cos(std::floor(_runTime)),     0,  -std::sin(std::floor(_runTime)),  0,
                                             0,                         1,  0,                       0,
                                             std::sin(std::floor(_runTime)),     0,  std::cos(std::floor(_runTime)),   0,
                                             0,0,0,1));
        // ====================================================================
        // End Exercise code
        // ====================================================================
    }

    glm::mat4 transformation = projectionMatrix * modelViewMatrix;

    // Iterate over all triangles in the bunny
    for (int i = 0; i < g_numberOfBunnyVertices; i += 3)
    {
        // Get points and normals from bunny data array:
        glm::vec4 p1 = glm::vec4(g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 0], g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 1],
                                 g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 2], g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 3]);

        glm::vec4 n1 = glm::vec4(g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 4], g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 5],
                                 g_bunnyMesh[(i + 0) * g_bunnyStrideSize + 6], 0.0);

        glm::vec4 p2 = glm::vec4(g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 0], g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 1],
                                 g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 2], g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 3]);

        glm::vec4 n2 = glm::vec4(g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 4], g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 5],
                                 g_bunnyMesh[(i + 1) * g_bunnyStrideSize + 6], 0.0);

        glm::vec4 p3 = glm::vec4(g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 0], g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 1],
                                 g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 2], g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 3]);

        glm::vec4 n3 = glm::vec4(g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 4], g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 5],
                                 g_bunnyMesh[(i + 2) * g_bunnyStrideSize + 6], 0.0);

        // Apply transformations to points
        p1 = transformation * p1;
        p2 = transformation * p2;
        p3 = transformation * p3;

        // Compute normal of the current triangle
        glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(p2 - p1), glm::vec3(p3 - p1)));

        // Transform normals by transposed inverse Modelview
        glm::mat4 invMod = glm::transpose(glm::inverse(modelViewMatrix));
        n1 = invMod * n1;
        n2 = invMod * n2;
        n3 = invMod * n3;

        // Dehomogenization
        if (p1.w != 0.0)
            p1 /= p1.w;
        if (p2.w != 0.0)
            p2 /= p2.w;
        if (p3.w != 0.0)
            p3 /= p3.w;

        // Apply Viewport transformation
        p1 = VPMatrix * p1;
        p2 = VPMatrix * p2;
        p3 = VPMatrix * p3;

        if (_scene <= 2)
        {
            // No shading. Uniform normals in eye direction.
            normal = glm::vec3(0, 0, 1);
        }


        // Rasterize Triangle
        // triangles are defined in counterclockwise order
        drawTriangle(p1, p2, p3, normal);
    }
}

void task::initCustomResources() {}

void task::deleteCustomResources() {}
