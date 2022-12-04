/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.hh"
#include <algorithm>

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
        if(inTriangle(vertices[i], vertices[i_a], vertices[i_b], vertices[i_c]) && i != i_a && i != i_b && i != i_c){
            return false;
        }
    }
    return true;
}

size_t prevnode(const size_t& i, const std::vector<bool>& clipped)
{
    size_t n = clipped.size();
    size_t res;

    if(i == 0){
        res = n-1;
    }else{
        res = i-1;
    }
    while(clipped[res]){
        if(res == 0){
            res = n-1;
        }else{
            res -= 1;
        }
    }
    return res;
}

size_t nextnode(const size_t& i, const std::vector<bool>& clipped)
{
    size_t n = clipped.size();
    size_t res;

    if(i == n-1){
        res = 0;
    }else{
        res = i+1;
    }
    while(clipped[res]){
        if(res == n-1){
            res = 0;
        }else{
            res +=1;
        }
    }
    return res;
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

    size_t i = 0;

    while(std::count(clipped.begin(), clipped.end(), false) > 2){
        if(!clipped[i]) {
            size_t curr = i;
            size_t prev = prevnode(i, clipped);
            size_t next = nextnode(i, clipped);


            /* std::cout << "previous: " << prev << ", current: " << curr << ", next: " << next << std::endl;
            for(int l = 0; l < n; l++){
                std::cout << clipped[l] << ",";
            }
            std::cout << std::endl;
            */
            if(convex(vertices[prev], vertices[curr], vertices[next]) && triangleEmpty(prev, curr, next, vertices)){
                triangles.push_back(prev);
                triangles.push_back(curr);
                triangles.push_back(next);

                clipped[curr] = true;
            }
            i = next;
        }else{
            if(i = n-1){
                i = 0;
            }else{
                i++;
            }
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
