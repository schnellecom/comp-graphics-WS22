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

#include <glm/glm.hpp>

// draws a circle of radius 1.0 (screen is from -1..1) with a given transform and color (RGB)
void drawCircle(const glm::vec3& color, const glm::mat4& transform);
