//
//  ParticleEmitter.h
//  Project2
//
//  Created by Andrei Titoruk on 4/14/22.
//
#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"

class ParticleEmitter {
public:

    vector<Particle> particles;
    vector<Particle> rays;
    float strength = 10.0;

    ParticleEmitter() {}

    void explode(glm::vec3 pos) {
        for (int i = 0; i < strength * 3; i++) {
            float rand1 = strength * ofRandom(-10, 10);
            float rand2 = strength * ofRandom(-10, 10);
            glm::vec3 force = glm::vec3(rand1, rand2, 0);
            glm::vec3 velocity = glm::vec3(rand1, rand2, 0);
            Particle particle = Particle(pos, force, velocity);
            particle.birthtime  = ofGetElapsedTimeMillis();
            particles.push_back(particle);
        }
    }

    void update() {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].update();
        }
        for (int i = 0; i < particles.size(); i++) {
            if (particles[i].age() > particles[i].lifespan){
                particles.erase(particles.begin() + i);
            }
        }
        for (int i = 0; i < rays.size(); i++) {
              rays[i].update();
        }
        for (int i = 0; i < rays.size(); i++) {
            if (rays[i].age() > rays[i].lifespan){
                rays.erase(rays.begin() + i);
            }
        }
    }

    void draw() {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].draw();
        }
        for (int i = 0; i < rays.size(); i++) {
            rays[i].draw();
        }
    }
    
    void shoot(glm::vec3 plyrPos, glm::vec3 direction){
        glm::vec3 velocity = direction * 500;
        glm::vec3 force = glm::vec3(0, 0, 0);
        Particle particle = Particle(plyrPos, force, velocity);
        particle.color = ofColor::red;
        particle.lifespan = 1;
        particle.birthtime = ofGetElapsedTimeMillis();
        rays.push_back(particle);
    
    }

};
