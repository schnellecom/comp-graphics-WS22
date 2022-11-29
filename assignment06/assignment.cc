/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.hh"

namespace task
{

bool convex(const glm::vec2& prev, const glm::vec2& curr, const glm::vec2& next)
{
    // True iff the vertex curr is a convex corner.
    // Assume counter-clockwise vertex order.

    return false;
}

bool inTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    // True iff the point p lies within the triangle a, b, c.
    // Assume counter-clockwise vertex order.

    return false;
}

bool triangleEmpty(const int i_a, const int i_b, const int i_c, const std::vector<glm::vec2>& vertices)
{
    // True iff there is no other vertex inside the triangle a, b, c.

    return false;
}

void triangulate(const std::vector<glm::vec2>& vertices, std::vector<int>& triangles)
{
    // Loop through vertices and clip away ears until only two vertices are left.
    // Input:  "vertices" contains the polygon vertices in counter-clockwise order.
    // Output: "triangles" contains indices into the "vertices" vector. Each triplet
    //         of consecutive indices specifies a triangle in counter-clockwise order.

    size_t n = vertices.size();
    if (vertices.size() < 3)
        return;

    std::cout << "Starting triangulation of a " << n << "-gon." << std::endl;

    // True iff the vertex has been clipped.
    std::vector<bool> clipped(n, false);

}

void initCustomResources()
{
}

void deleteCustomResources()
{
}

}
