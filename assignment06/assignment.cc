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
    glm::vec2 diffPrevCurr = curr - prev;
    glm::vec2 diffNextCurr = next - curr;

    float isConvex = diffPrevCurr[0]*diffNextCurr[1] - diffPrevCurr[1]*diffNextCurr[0];

    if(isConvex > 0){
        return true;
    }
    else{
        return false;
    }
}

bool inTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    // True iff the point p lies within the triangle a, b, c.
    // Assume counter-clockwise vertex order.

    float alpha = ((b[1] - c[1])*(p[0] - c[0]) + (c[0] - b[0])*(p[1] - c[1])) /
                  ((b[1] - c[1])*(a[0] - c[0]) + (c[0] - b[0])*(a[1] - c[1]));
    float beta = ((c[1] - a[1])*(p[0] - c[0]) + (a[0] - c[0])*(p[1] - c[1])) /
                 ((b[1] - c[1])*(a[0] - c[0]) + (c[0] - b[0])*(a[1] - c[1]));
    float gamma = 1.0f - alpha - beta;

    if(alpha > 0 && beta > 0 && gamma > 0){
        return true;
    }
    else{
        return false;
    }
}

bool triangleEmpty(const int i_a, const int i_b, const int i_c, const std::vector<glm::vec2>& vertices)
{
    // True iff there is no other vertex inside the triangle a, b, c.
    for(int i = 0; i < vertices.size(); i++){
        if(!inTriangle(vertices[i], vertices[i_a], vertices[i_b], vertices[i_c]) && i != i_a && i != i_b && i != i_c){
            return false;
        }
    }
    return true;
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

    for(int i=0; i < n; i++){
        int curr = i;
        int next = i;
        int prev = i;

        if(curr == 0){
            prev = n - 1;
        }
        else{
            prev = curr - 1;
        }
        if(curr = n-1){
            next = 0;
        }
        else{
            next = curr + 1;
        }
    }

}

void initCustomResources()
{
}

void deleteCustomResources()
{
}

}
