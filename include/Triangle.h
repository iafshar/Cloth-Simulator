#pragma once
#include "Particle.h"
#include <vector>
#include <iostream>
using namespace std;

class Triangle {
    public:
        Particle *p1;
        Particle *p2;
        Particle *p3;
        glm::vec3 velocity;
        glm::vec3 normal;
        float crossArea;

        void computeThings(glm::vec3 vAir) {
            glm::vec3 vSurf = (p1->velocity + p2->velocity + p3->velocity) * (float)(1.0/3.0);
            velocity = vSurf - vAir;

            normal = glm::normalize(glm::cross((p2->position - p1->position),(p3->position - p1->position)));

            float a0 = 0.5 * glm::length(glm::cross((p2->position - p1->position),(p3->position - p1->position)));
            crossArea = a0 * (glm::dot(glm::normalize(velocity),normal));
        }
        void applyForce(glm::vec3 vAir) {
            //figure out drag
            glm::vec3 F = normal * (float)(-0.5 * 1.225 * (glm::length(velocity) * glm::length(velocity)) * 1.2 * crossArea);
            F *= (1.0/3.0);
            p1->applyForce(F);
            p2->applyForce(F);
            p3->applyForce(F);
            if (p1->onGround)
            {
                glm::vec3 v = p1->velocity - vAir;
                glm::vec3 e = -glm::normalize(v);
                float pCrossArea = M_PI;
                glm::vec3 FDrag = e * (float)(0.5 * 1.225 * (glm::length(v) * glm::length(v)) * 0.47 * pCrossArea);
                p1->applyForce(FDrag);
            }
            if (p2->onGround)
            {
                glm::vec3 v = p2->velocity - vAir;
                glm::vec3 e = -glm::normalize(v);
                float pCrossArea = M_PI;
                glm::vec3 FDrag = e * (float)(0.5 * 1.225 * (glm::length(v) * glm::length(v)) * 0.47 * pCrossArea);
                p2->applyForce(FDrag);
                
            }
            if (p3->onGround)
            {
                glm::vec3 v = p3->velocity - vAir;
                glm::vec3 e = -glm::normalize(v);
                float pCrossArea = M_PI;
                glm::vec3 FDrag = e * (float)(0.5 * 1.225 * (glm::length(v) * glm::length(v)) * 0.47 * pCrossArea);
                p3->applyForce(FDrag);
            }
        }
};