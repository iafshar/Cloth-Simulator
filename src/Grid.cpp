#include "Grid.h"
Grid::Grid(int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        std::vector<Particle*> v;
        particles.push_back(v);
    }
    
    glm::vec3 currentPos = glm::vec3(0,0,0);
    float z = 0.001;
    
    for (int i = 0; i < height; i++)
    {
        currentPos = glm::vec3(-1,i*0.66,z);
        if (z == 0.001)
        {
            z = 0;
        }
        else
        {
            z = 0.001;
        }
        
        for (int j = 0; j < width; j++)
        {
            Particle* p = new Particle;
            particles[i].push_back(p); 
            p->position = currentPos;
            positions.push_back(p->position); 
            
            currentPos += glm::vec3(0.66,0,0);

            

            if (particles[i].size() > 1)
            {
                SpringDamper* sdH = new SpringDamper; // H for horizontal
                sdH->p1 = particles[i].rbegin()[1]; // points right
                sdH->p2 = p;
                sdH->restLength = glm::length(p->position - particles[i].rbegin()[1]->position);
                springDampers.push_back(sdH);
            }
            if (i > 0)
            {
                SpringDamper* sdV = new SpringDamper; // V for vertical
                sdV->p1 = p; // points up in vector but down on screen
                sdV->p2 = particles[i-1][j];
                sdV->restLength = glm::length(particles[i-1][j]->position - p->position);
                springDampers.push_back(sdV);
                if (j > 0)
                {
                    SpringDamper* sdDL = new SpringDamper; // DL for diagonal left (points to the top left)
                    sdDL->p1 = p; // points to the top left of vector but bottom left of screen
                    sdDL->p2 = particles[i-1][j-1];
                    sdDL->restLength = glm::length(particles[i-1][j-1]->position - p->position);
                    springDampers.push_back(sdDL);
                    
                    SpringDamper* sdDR = new SpringDamper; // DR for diagonal right
                    sdDR->p1 = particles[i][j-1]; // points to the top right of vector but bottom right of screen
                    sdDR->p2 = particles[i-1][j];
                    sdDR->restLength = glm::length(particles[i-1][j]->position - particles[i][j-1]->position);
                    springDampers.push_back(sdDR);
                    
        
                }
            }
            //if ((i == height-1 && j == width-1)||(i == 0 && j == width-1)||(i == 0 && j == 0)||(i == height-1 && j == 0))
            if (i == height-1)
            {
                p->top = true;
            }
        }
    }
    // set triangles
    for (int i = 1; i < height; i++) // triangles are created from diagonals from bottom right to top left.
    {
        for (int j = 1; j < width; j++)
        {
            Triangle* t1 = new Triangle;
            t1->p1 = particles[i][j];
            t1->p2 = particles[i][j-1];
            t1->p3 = particles[i-1][j-1];
            t1->computeThings(vAir);
            indices.push_back((i*width) + j);
            indices.push_back((i*width) + j-1);
            indices.push_back(((i-1)*width) + j-1);
            triangles.push_back(t1);
            t1->p1->normal += t1->normal;
            t1->p2->normal += t1->normal;
            t1->p3->normal += t1->normal;

            Triangle* t2 = new Triangle;
            t2->p1 = particles[i][j];
            t2->p3 = particles[i-1][j];
            t2->p2 = particles[i-1][j-1];
            t2->computeThings(vAir);
            indices.push_back((i*width) + j);
            indices.push_back(((i-1)*width) + j-1);
            indices.push_back(((i-1)*width) + j);
            
            triangles.push_back(t2);
            t2->p1->normal += t2->normal;
            t2->p2->normal += t2->normal;
            t2->p3->normal += t2->normal;

        }
    }

    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles.size(); j++)
        {
            particles[i][j]->normal = glm::normalize(particles[i][j]->normal);
            normals.push_back(particles[i][j]->normal);
        }
        
    }
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    //Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

Grid::~Grid()
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles[i].size(); j++)
        {
            delete particles[i][j];
        }
        
    }
    for (int i = 0; i < springDampers.size(); i++)
    {
        delete springDampers[i];
    }
    for (int i = 0; i < triangles.size(); i++)
    {
        delete triangles[i];
    }  
}


void Grid::update()
{
    // gravity 
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles[i].size(); j++)
        {
            glm::vec3 gravity = glm::vec3(0,-9.8,0) * particles[i][j]->mass;
            particles[i][j]->applyForce(gravity);
        }
    }

    // springDamping force
    for (int i = 0; i < springDampers.size(); i++)
    {
        springDampers[i]->applyForce();
    }
    
    // Aerodynamic force - Dont know if this works
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i]->computeThings(vAir);
        triangles[i]->applyForce(vAir);
    }

    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles[i].size(); j++)
        {
            for (int k = 0; k < 10; k++)
            {
                particles[i][j]->integrate(0.0005);
            }
            
            if (particles[i][j]->position.y <= -1.95)
            {
                particles[i][j]->position.y = (float)-1.95;
                particles[i][j]->onGround = true;
                // particles[i][j]->position.y = (float)-1.95 + ((i*particles[i].size()) + j)/(particles.size() * particles[i].size());
                // float friction = -particles[i][j]->force.y * 1.0f;
                // glm::vec3 frictionNew = glm::vec3(particles[i][j]->force.x,0,particles[i][j]->force.z) * friction;
                // glm::vec3 frictionNew = glm::vec3(-particles[i][j]->force.x,0,-particles[i][j]->force.z);
                // particles[i][j]->applyForce(frictionNew);
            }
            else
            {
                particles[i][j]->onGround = false;
            }
            
        }
    }
    
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles[i].size(); j++)
        {
            particles[i][j]->normal = glm::vec3(0);
        }
        
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i]->p1->normal += triangles[i]->normal;
        triangles[i]->p2->normal += triangles[i]->normal;
        triangles[i]->p3->normal += triangles[i]->normal;
    }
    
    

    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles[i].size(); j++)
        {
            particles[i][j]->normal = glm::normalize(particles[i][j]->normal);
            positions[(i * particles[0].size()) + j] = particles[i][j]->position;
            normals[(i * particles[0].size()) + j] = particles[i][j]->normal;
        }
        
    }
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);

    
    
}

void Grid::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}