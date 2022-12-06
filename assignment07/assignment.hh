/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                            DO NOT EDIT THIS FILE!                         *
 *                                                                           */
/*===========================================================================*/

#pragma once

#include "main.hh"

#include "common.hh"
#include "constants.hh"

namespace task
{
void drawScene(int scene, float runTime);
void initCustomResources();
void deleteCustomResources();
// void resizeCallback(int newWidth, int newHeight);
}

namespace solution
{
void drawScene(int scene, float runTime);
void initCustomResources();
void deleteCustomResources();
// void resizeCallback(int newWidth, int newHeight);
}

const glm::vec3 eyePos = glm::vec3(0.0f, 3.0f, 5.0f);
/*
 * Attention:
 * Light source positions are given in eye-space coordinates
 * since they are supposed to remain at a constant position
 * in the scene!
 */
const glm::vec3 vLightPosition1 = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 vLightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
const float fLightSpotLightFactor1 = 60.0f;

const glm::vec3 vLightPosition2 = glm::vec3(10.0f, 8.0f, -1.0f);
const glm::vec3 vLightColor2 = glm::vec3(1.0f, 0.9f, 0.4f);
const float fLightSpotLightFactor2 = 30.0f;

const glm::mat3 mAmbientMaterial = glm::mat3(0.2f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.2f);

const glm::mat3 mDiffuseMaterial = glm::mat3(0.7f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.7f);

const glm::mat3 mSpecularMaterial = glm::mat3(0.95f, 0.0f, 0.0f, 0.0f, 0.95f, 0.0f, 0.0f, 0.0f, 0.95f);

const float mSpecularityExponent = 40.0f;
