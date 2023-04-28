#pragma once
#include "Particle.h"
using namespace std;

class SpringDamper
{

public:
    float springConstant = 200.0;
    float dampConstant = 50.0;
    float restLength = 0.66;
    Particle *p1;
    Particle *p2;

    void applyForce();

};
