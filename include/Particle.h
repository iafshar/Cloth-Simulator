#pragma once
#include "core.h"
#include <iostream>
using namespace std;

class Particle
{
public:
    bool top = false;
    bool onGround = false;
    float mass = 0.2;
    glm::vec3 position = glm::vec3(0); 
    glm::vec3 normal = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 force = glm::vec3(0,0,0);

    void applyForce(glm::vec3 &f) {
        if (top == false)
        {
            force += f;
        }
        
         
    }
    void integrate(float deltaTime) {
        if (top == false)
        {
            glm::vec3 accel = (1/mass) * force;
            velocity += accel * deltaTime;
            position += velocity * deltaTime;
            force = glm::vec3(0,0,0);
       }
    }
    
};

