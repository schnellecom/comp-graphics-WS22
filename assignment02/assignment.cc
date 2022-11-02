/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           */
/*===========================================================================*/

#include <cmath>

#include "assignment.hh"

glm::mat4x4 scalingMatrix(float scale){
    glm::mat4 matrix = glm::mat4(scale, 0, 0, 0,
                                 0, scale, 0, 0,
                                 0, 0, scale, 0,
                                 0, 0, 0, 1);
    return matrix;
}

glm::mat4x4 scalingMatrix(float x, float y, float z){
    glm::mat4 matrix = glm::mat4(x, 0, 0, 0,
                                 0, y, 0, 0,
                                 0, 0, z, 0,
                                 0, 0, 0, 1);
    return matrix;
}

glm::mat4x4 translationMatrix(float x, float y, float z){
    glm::mat4 matrix = glm::mat4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 x, y, z, 1);
    return matrix;
}

glm::mat4x4 rotationMatrix(float a){
    glm::mat4 matrix = glm::mat4(std::cos(a), std::sin(a), 0, 0,
                                 -std::sin(a), std::cos(a), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);
    return matrix;
}

void task::drawScene(int scene, float runTime) {
    //(a) draw racetrack with borders
    drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), scalingMatrix(0.9));
    drawCircle(glm::vec3(0.1f, 0.1f, 0.1f), scalingMatrix(0.89));
    drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), scalingMatrix(0.69));
    drawCircle(glm::vec3(0.0f, 0.0f, 0.0f), scalingMatrix(0.68));

    //(b) draw stand on the left
    glm::mat4x4 standMatrix = translationMatrix(-0.95, 0, 0)*scalingMatrix(0.05, 0.7, 1);
    drawCircle(glm::vec3(0.3f, 0.3f, 0.3f), standMatrix);

    //(c) draw a start/finish line
    glm::mat4x4 startLine = scalingMatrix(0.005, 0.05, 1);
    for(int i=0; i<9; i++){
        drawCircle(glm::vec3(1, 1, 1), translationMatrix(-(0.70f+0.02f*i), 0, 0)*startLine);
    }

    //(d) add middle lines
    int numberMiddleLines = 20;
    glm::mat4x4 middleLine = translationMatrix(-0.78f, 0, 0)*scalingMatrix(0.005, 0.05, 1);
    float angles = 2*M_PI/numberMiddleLines;
    for(int i=0; i<numberMiddleLines; i++){
        drawCircle(glm::vec3(1,1,1), rotationMatrix(i*angles)*middleLine);
    }

    //(e) add moving cars
    glm::mat4x4 carA = translationMatrix(-0.73f, 0, 0)*scalingMatrix(0.01, 0.05, 1);
    glm::mat4x4 carB = translationMatrix(-0.83f, 0, 0)*scalingMatrix(0.01, 0.05, 1);

    drawCircle(glm::vec3(1,0,0), rotationMatrix(-runTime)*carA);
    drawCircle(glm::vec3(0,1,0), rotationMatrix(-2*runTime)*carB);

}

void task::initCustomResources() {}
void task::deleteCustomResources() {}

