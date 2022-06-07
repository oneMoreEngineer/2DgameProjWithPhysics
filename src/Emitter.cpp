#include "ofApp.h"
#include "Emitter.h"

//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
    sprites.push_back(s);
}

// Remove a sprite from the sprite system.
//
void SpriteList::remove(int i) {
    sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).
//
void SpriteList::update() {

    if (sprites.size() == 0) return;
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

   
    while (s != sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            //            cout << "deleting sprite: " << s->name << endl;
            tmp = sprites.erase(s);
            s = tmp;
        }
        else s++;
    }

    
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].pos += sprites[i].velocity / ofGetFrameRate();
    }
}


void SpriteList::draw() {
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].draw();
    }
}



Emitter::Emitter() {
    sys = new SpriteList();
    partEm = new ParticleEmitter();
    init();
}


void Emitter::init() {
    lifespan = 3000;    // default milliseconds
    started = false;

    lastSpawned = 0;
    rate = 1;    // sprites/sec
    haveChildImage = false;
    haveImage = false;
    velocity = ofVec3f(0, 0, 0);
    drawable = true;
    width = 50;
    height = 50;
    thrust = 150;
    hit.load("sound/hit.mp3");
    hit.setVolume(0.7);
    
    
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
    if (drawable) {

        if (haveImage) {
            image.draw(-image.getWidth() / 2.0 + pos.x, -image.getHeight() / 2.0 + pos.y);
        }
        else {
            ofSetColor(0, 0, 200);
            ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);
        }
    }

    // draw sprite system
    //
    sys->draw();
    partEm->draw();
}


void Emitter::update(glm::vec3 plyrPos, vector<glm::vec3> plyrVerts, glm::mat4 T) {
    if (!started) return;

    float time = ofGetElapsedTimeMillis();

    if ((time - lastSpawned) > (1000.0 / rate)) {

        // call virtual to spawn a new sprite
        //
        spawnSprite(plyrPos);
        lastSpawned = time;
    }



    // update sprite list
    //
    if (sys->sprites.size() == 0) return;
    vector<Sprite>::iterator s = sys->sprites.begin();
    vector<Sprite>::iterator tmp;

    
    
    while (s != sys->sprites.end()) {
        if (s->inside(plyrVerts, s->pos, T)) {
            energy--;
            hit.play();
            partEm->explode(s->pos);
        }
        for (int i = 0; i < partEm->rays.size(); i++) {
            if (glm::distance(s->pos, partEm->rays[i].position) < partEm->rays[i].radius * 2 + 5){
                energy++;
                hit.play();
                partEm->explode(s->pos);
//                tmp = sys->sprites.erase(s);
//                s = tmp;
                s->lifespan = -1;
                partEm->rays.erase(partEm->rays.begin() + i);
            }
        }
        if ((s->lifespan != -1 && s->age() > s->lifespan)|| s->inside(plyrVerts, s->pos, T) || s->lifespan == -1) {
            tmp = sys->sprites.erase(s);
            s = tmp;
        }
        else s++;
    }

    
    for (int i = 0; i < sys->sprites.size(); i++) {
        moveSprite(&sys->sprites[i], plyrPos, plyrVerts, T);
    }
    
    //shoot(plyrPos, glm::vec3(0,0,0));
    //partEm->shoot(plyrPos, glm::vec3(0,0,0));
    partEm->update();
}

void Emitter::moveSprite(Sprite *sprite, glm::vec3 plyr, vector<glm::vec3> plyrVerts, glm::mat4 T) {
    if (haveChildImage) sprite->setImage(childImage);
    sprite->rot = sprite->changeAngle(plyr);
    //sprite->pos += (150 * sprite->heading()) / ofGetFrameRate();
    sprite->force = sprite->heading() * thrust;
    sprite->intergrate();
}

// virtual function to spawn sprite (can be overloaded)
//
void Emitter::spawnSprite(glm::vec3 plyrPos) {
    Sprite sprite;
    if (haveChildImage) sprite.setImage(childImage);
    sprite.velocity = velocity;
    sprite.lifespan = lifespan;
    
    sprite.pos = glm::vec3(rand() % ofGetWidth(), rand() % ofGetHeight(), 0);
    sprite.birthtime = ofGetElapsedTimeMillis();
    sprite.rot = sprite.changeAngle(plyrPos);
    sys->add(sprite);
}

void Emitter::shoot(glm::vec3 plyrPos, glm::vec3 direction){
    partEm->shoot(plyrPos, direction);

}

// Start/Stop the emitter.
//
void Emitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
    started = false;
}


void Emitter::setLifespan(float life) {
    lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
    velocity = v;
}

void Emitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
    image = img;
}

void Emitter::setRate(float r) {
    rate = r;
}
