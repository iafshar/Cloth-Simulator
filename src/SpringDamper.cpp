#include "SpringDamper.h"

void SpringDamper::applyForce() {
    glm::vec3 e = p2->position - p1->position;
    float l = glm::length(e);
    e = glm::normalize(e);

    float vclose = glm::dot((p1->velocity - p2->velocity),e);

    float f = ((-springConstant) * (restLength - l)) - (dampConstant * vclose);
    glm::vec3 F1 = e * f;
    glm::vec3 F2 = -F1;

    p1->applyForce(F1);
    p2->applyForce(F2);
}