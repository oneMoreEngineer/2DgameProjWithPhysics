//
//  Particle.h
//  Project2
//
//  Created by Andrei Titoruk on 4/14/22.
//

#include "ofMain.h"

class Particle {
public:

    glm::vec3 position;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofVec3f force;
    float    damping;
    float   mass;
    float   lifespan;
    float   radius;
    float   birthtime;
    ofColor color;

    Particle(glm::vec3 pos, glm::vec3 force, glm::vec3 velocity) {
        this->velocity = velocity;
        acceleration.set(0, 0, 0);
        position = pos;
        this->force = force;
        mass = 1;
        lifespan = 2;
        birthtime = 0;
        radius = 5.0;
        damping = 0.98;
        color = ofColor::orange;
    }

    // Implement physics
    void update() {
        float dt = ofGetLastFrameTime();

        // Update position
        ofVec3f accel = acceleration;
        accel += force * (1.0 / mass);
        position += (velocity * dt);
        velocity += accel * dt;

        // Damping
        velocity *= damping;

        // Zero out forces
        force.set(0, 0, 0);
        
        
    }

    //  return age in seconds
    //
    float age() {
        return (ofGetElapsedTimeMillis() - birthtime) / 1000.0;
    
    }

    void draw() {
        ofSetColor(color);
        ofDrawSphere(position, radius);
    }

};

