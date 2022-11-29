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
#include <vector>

namespace task
{
void triangulate(const std::vector<glm::vec2>& vertices, std::vector<int>& triangles);
void initCustomResources();
void deleteCustomResources();
}

namespace solution
{
void triangulate(const std::vector<glm::vec2>& vertices, std::vector<int>& triangles);
void initCustomResources();
void deleteCustomResources();
}
