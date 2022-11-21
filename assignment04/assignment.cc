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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <iostream>

namespace
{
glm::mat4 viewMatrix(1.f);
glm::mat4 projectionMatrix(1.f);

glm::mat4 getRotationMatrixZAxis(float angle)
{
    glm::mat4 r(1.f);
    r[0][0] = std::cos(angle);
    r[1][0] = -std::sin(angle);
    r[0][1] = std::sin(angle);
    r[1][1] = std::cos(angle);
    r[2][2] = 1.0;
    r[3][3] = 1.0;
    return r;
}

glm::mat4 getRotationMatrixYAxis(float angle)
{
    glm::mat4 r(1.f);
    r[0][0] = std::cos(angle);
    r[2][0] = std::sin(angle);
    r[1][1] = 1.0;
    r[0][2] = -std::sin(angle);
    r[2][2] = std::cos(angle);
    r[3][3] = 1.0;
    return r;
}

float scalar(glm::vec4 v, glm::vec4 w){
    float res = 0;
    res += v[0]*w[0];
    res += v[1]*w[1];
    res += v[2]*w[2];
    res += v[3]*w[3];

    return res;
}

float norm(glm::vec4 v){
    float r;
    r = std::pow(v[0], 2);
    r += std::pow(v[1], 2);
    r += std::pow(v[2], 2);
    r += std::sqrt(r);
    return r;
}

glm::vec4 normalize(glm::vec4 v){
    if(norm(v) != 0) {
        return v.operator*=(std::pow(norm(v), -1));
    }
    return v;
}

glm::vec3 crossProduct(glm::vec3 v, glm::vec3 w){
    glm::vec3 p;
    p[0] = v[1]*w[2] - v[2]*w[1];
    p[1] = v[2]*w[0] - v[0]*w[2];
    p[2] = v[0]*w[1] - v[1]*w[0];

    return p;
}

glm::vec4 crossProduct(glm::vec4 v, glm::vec4 w){
    glm::vec4 p;
    p[0] = v[1]*w[2] - v[2]*w[1];
    p[1] = v[2]*w[0] - v[0]*w[2];
    p[2] = v[0]*w[1] - v[1]*w[0];
    p[3] = 0;

    return p;
}

glm::vec4 toVector(glm::vec3 v){
    glm::vec4 r;
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = 0;

    return r;
}

glm::vec4 toPoint(glm::vec3 v){
    glm::vec4 r;
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = 0;

    return r;
}

glm::mat4x4 translationMatrix(float x, float y, float z){
    glm::mat4 matrix = glm::mat4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 x, y, z, 1);
    return matrix;
}

/*
 * A white circle represents the track center line
 */
void drawTrackMarks(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(1.0, 1.0, 1.0), model, view, projection);
}

/*
 * A Blue circle represents the tracks outer boundary
 */
void drawOuterTrackMarks(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(0.0, 0.0, 1.0), model, view, projection);
}

/*
 * A gray circle representing the grandstand
 */
void drawGrandstand(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(0.4, 0.4, 0.4), model, view, projection);
}

void drawTrack()
{
    for (unsigned int i = 0; i < 30; ++i)
    {
        // Compute rotation for the marks
        float angle = float(i) * 4.0f * M_PI / 60.0f;

        float next_angle = float(i + 1) * 4.0f * M_PI / 60.0f;

        float height = std::sin(angle * 2.0) * 0.1;

        float next_height = std::sin(float(i + 1) * 4.0f * M_PI / 60.0f * 2.0) * 0.1;

        glm::mat4 markRotation = getRotationMatrixZAxis(angle);

        // =====================================================
        // Track Center
        // =====================================================

        // Scaling
        glm::mat4 scaleTrack(1.f);
        scaleTrack[0][0] = 0.03;
        scaleTrack[1][1] = 0.01;
        scaleTrack[2][2] = 0.01;

        // translate
        glm::mat4 translateTrack(1.f);
        translateTrack[3][1] = 0.8;    // Radius of the circle
        translateTrack[3][2] = height; // Height

        // rotate around y axis to follow track height change
        glm::mat4 heightRotation = getRotationMatrixYAxis(-(M_PI / 2.0 - std::acos((height - next_height) / (2.0 * std::sin((next_angle - angle) / 2.0)))));

        drawTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);

        // =====================================================
        // Track Boundaries
        // =====================================================

        // Outer Circle
        translateTrack[3][1] = 0.9; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);

        // Inner Circle
        translateTrack[3][1] = 0.7; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);
    }
}

void drawGrandstand()
{
    glm::mat4 transformStand(1.f);

    // Scale circle to form grandstand
    transformStand[0][0] = 0.05;
    transformStand[1][1] = 0.6;
    transformStand[2][2] = 0.07;

    // Place on the left of the track
    transformStand[3][0] = -0.95;

    drawGrandstand(transformStand, viewMatrix, projectionMatrix);
}

void drawFinish()
{
    glm::mat4 transformFinish(1.f);

    // Scaling
    transformFinish[0][0] = 0.005;
    transformFinish[1][1] = 0.03;
    transformFinish[2][2] = 0.01;

    // Initialize translation (Left boundary of track)
    transformFinish[3][0] = -0.9;

    // Draw nine marks from left to right
    for (unsigned int i = 0; i < 9; ++i)
    {
        // Translate to next mark
        transformFinish[3][0] += 0.02;

        drawTrackMarks(transformFinish, viewMatrix, projectionMatrix);
    }
}

void drawCar(float angle, int lane, glm::vec3 color)
{
    // Start with identity
    glm::mat4 transformCar(1.f);
    float height = std::sin(angle * 2.0) * 0.1;

    // Scale second car to fit onto track
    transformCar[0][0] *= 0.04;
    transformCar[1][1] *= 0.1;
    transformCar[2][2] *= 0.03;
    transformCar[3][2] = -height;

    // Transform second car to position 0 (x-direction)
    if (lane == 1)
    {
        transformCar[3][0] = -0.85;
    }
    else
    {
        transformCar[3][0] = -0.75;
    }

    glm::mat4 rotation = getRotationMatrixZAxis(angle);
    rotation *= transformCar;

    drawCircle(color, rotation, viewMatrix, projectionMatrix);
}


glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far)
{
    glm::mat4 fm(1.f);

    // ====================================================================
    // buildFrustum function for programming exercise part b:
    // Add your code here:
    // ====================================================================

    //setup matrix as in slide 44, lecture 03 perspektive
    float t = near*std::tan(phiInDegree/2);
    float b = -t;
    float T = t*(far/near);
    float B = b*(far/near);

    //compute r, l according to aspect ratio
    float heigth = 2*(near*std::tan(phiInDegree/2));
    float width = aspectRatio*heigth;
    float r = width;
    float l = -width;

    glm::mat4 res(0.0f);
    res[0][0] = (2*near)/(r-l);
    res[1][1] = (2*near)/(t-b);
    res[0][2] = (r+l)/(r-l);
    res[1][2] = (t+b)/(t-b);
    res[2][2] = -(far+near)/(far-near);
    res[3][2] = -1;
    res[2][3] = -(2*far*near)/(far-near);

    // ====================================================================
    // End Exercise code
    // ====================================================================

    return res;
}

glm::mat4 lookAt(const glm::vec3& camPos, const glm::vec3& viewDirection, const glm::vec3& up)
{
    // ====================================================================
    // Lookat for programming exercise part a:
    // Add your code here:
    // ====================================================================

    //transform to homogeneous coords
    glm::vec4 C = toPoint(camPos);
    C = normalize(C);
    glm::vec4 D = toVector(viewDirection);
    D = normalize(D);
    glm::vec4 U = toVector(up);
    C = normalize(C);

    //calculate the camera coordinate system
    glm::vec4 R = crossProduct(D, U);
    R = normalize(R);
    U = crossProduct(R, D);
    U = normalize(U);

//    glm::mat4 T = translationMatrix(-C[0], -C[1], -C[2]);

    //new approach with script page 23
    glm::mat4 T(0.0f);
    T[0][0] = R[0];
    T[0][1] = R[1];
    T[0][2] = R[2];

    T[1][0] = U[0];
    T[1][1] = U[1];
    T[1][2] = U[2];

    T[2][0] = -D[0];
    T[2][1] = -D[1];
    T[2][2] = -D[2];

    T[0][3] = scalar(-R, C);
    T[1][3] = scalar(-U, C);
    T[2][3] = scalar(D, C);

    T[3][3] = 1;

    return T;

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

}


void task::resizeCallback(int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);

    // ====================================================================
    // projection matrix setup for programming exercise part d:
    // Add your code here:
    // ====================================================================

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

void task::drawScene(int scene, float runTime)
{
    float angle1 = -2.0f * M_PI * runTime / 60.0f;

    if (scene != 4)
    {
        // =====================================================
        // static camera for programming exercise part c:
        // Add your code here:
        // =====================================================
        viewMatrix = lookAt(glm::vec3(0, -1, 1),
                            glm::vec3(0, 1, -1),
                            glm::vec3(0,1,0));

        // =====================================================
        // End Exercise code
        // =====================================================
    }
    else
    {
        // rotate around track for the other parts, looking at the center:
        glm::vec3 pos = glm::vec3(1.5f * std::sin(runTime), 1.5f * std::cos(runTime), 1.0f);
        viewMatrix = lookAt(pos, glm::vec3(0, 0, 0) - pos, glm::cross(glm::cross(glm::vec3(0, 0, 0) - pos, glm::vec3(0, 0, 1)), glm::vec3(0, 0, 0) - pos));
    }

    if (scene == 5)
    {
        float height = -std::sin(angle1 * 2.0) * 0.1;


        // =====================================================
        // Moving camera for programming exercise part e:
        // Add your code here:
        // =====================================================


        // =====================================================
        // End Exercise code
        // =====================================================
    }


    // Draw Track (Border and inner track marks)
    drawTrack();

    // Draw finish line
    drawFinish();

    // Draw Grandstand on the left
    drawGrandstand();


    drawCar(angle1, 1, glm::vec3(0, 1, 0));

    float angle2 = 0.5f * -2.0f * M_PI * runTime / 60.0f;
    drawCar(angle2, 2, glm::vec3(1, 1, 0));

    if (scene == 3)
    {
        // optional bonus fun :-D

        // state:
        static float carPosition = 0.0f;
        static float lastRunTime = runTime;
        static float carSpeed = 0.0f;
        static int carLane = 1;

        static float spectatorPos = 0.9f;
        static bool spectatorAlive = true;
        static float spectatorDirection = -1.0f;

        float timeDiff = runTime - lastRunTime;

        // car control:
        if (arrowKeyUpPressed)
            carSpeed += timeDiff;
        if (arrowKeyDownPressed)
            carSpeed -= 2.0f * timeDiff;
        carSpeed = glm::clamp(carSpeed, 0.0f, 1.0f);

        if (arrowKeyRightPressed)
            carLane = 2;
        if (arrowKeyLeftPressed)
            carLane = 1;

        carPosition += -2.0f * M_PI * timeDiff * 0.5 * carSpeed;
        if (carPosition < -2.0 * M_PI)
            carPosition += 2.0 * M_PI;

        // car rendering:
        drawCar(carPosition, carLane, glm::vec3(1, 0, 0));

        // car crash detection:
        float distToCar;
        float crashPos;
        if (carLane == 1)
        {
            distToCar = std::abs(carPosition - angle1);
            crashPos = 0.5 * (carPosition + angle1);
        }
        else
        {
            distToCar = std::abs(carPosition - angle2);
            crashPos = 0.5 * (carPosition + angle2);
        }
        if (distToCar < 0.24f)
        {
            // crash!
            carSpeed = 0.0f;

            // Start with identity
            glm::mat4 crash(1.f);

            // Scale second car to fit onto track
            crash[0][0] *= 0.02 + 0.5 * (0.24f - distToCar);
            crash[1][1] *= 0.02 + 0.5 * (0.24f - distToCar);
            crash[2][2] *= 0.02 + 0.5 * (0.24f - distToCar);

            if (carLane == 1)
            {
                crash[3][0] = -0.85;
            }
            else
            {
                crash[3][0] = -0.75;
            }
            glm::mat4 rotation = getRotationMatrixZAxis(crashPos);
            rotation *= crash;

            drawCircle(glm::vec3(0.8, 0.8, 0.8), rotation, viewMatrix, projectionMatrix);
        }

        float spectatorSize = 0.025;
        float distToSpectator = std::abs(carPosition);
        if (distToSpectator < 0.12f)
        {
            if ((carLane == 1) && (std::abs(spectatorPos + 0.85) < 0.75 * spectatorSize))
            {
                spectatorAlive = false;
            }
            if ((carLane == 2) && (std::abs(spectatorPos + 0.75) < 0.75 * spectatorSize))
            {
                spectatorAlive = false;
            }
        }


        glm::mat4 transformSpectator(1.f);

        // spectator rendering:
        glm::vec3 spectatorColor = glm::vec3(0.8, 0.2, 1.0);
        if (spectatorAlive)
        {
            spectatorPos += 0.03f * timeDiff * spectatorDirection;
            if (spectatorPos <= -0.9f)
            {
                spectatorPos = -0.9f;
                spectatorDirection = 1.0f;
            }
            if (spectatorPos >= -0.7f)
            {
                spectatorPos = -0.7f;
                spectatorDirection = -1.0f;
            }
            transformSpectator[2][2] = 0.06;
        }
        else
        {
            spectatorColor = glm::vec3(0.8, 0.0, 0.0);
            spectatorSize *= 2.0f;
            transformSpectator[2][2] = 0.005;
        }


        transformSpectator[0][0] = spectatorSize;
        transformSpectator[1][1] = spectatorSize;
        transformSpectator[3][0] = spectatorPos;

        drawCircle(spectatorColor, transformSpectator, viewMatrix, projectionMatrix);

        lastRunTime = runTime;
    }
}

void task::initCustomResources() {}

void task::deleteCustomResources() {}
