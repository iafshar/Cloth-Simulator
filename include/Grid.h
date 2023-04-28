#pragma once

#include "SpringDamper.h"
#include "Triangle.h"
#include <vector>
#include <iostream>
using namespace std;

/*
Problems:
Normals
opposite order
drag force
how to handle top

why is 5x5 so big
*/

class Grid
{
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals; // How?
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(0.8,0,0); //red
    
public:
    std::vector<std::vector<Particle*>> particles;
    std::vector<SpringDamper*> springDampers; // first (width-1) sds will be top horizontal, then first verticle, then second row first horizontal, then second verticle, then DL then DR
    std::vector<Triangle*> triangles;
    glm::vec3 vAir = glm::vec3(0,0,0.1);
    void update(); // apply rest of forces
    void draw(const glm::mat4& viewProjMtx, GLuint shader); // do buffer stuff
    Grid(int h, int w);
    ~Grid();
};