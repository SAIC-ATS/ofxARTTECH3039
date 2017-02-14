//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "Particle.h"
#include "ofGraphics.h"


Particle::Particle()
{
}


Particle::Particle(const glm::vec2& _position): position(_position)
{
}


Particle::Particle(const glm::vec2& _position, const glm::vec2& _velocity):
    position(_position),
    velocity(_velocity)
{
}


Particle::~Particle()
{
}


void Particle::update()
{
    age++;
    velocity += (gravity + impulse);
    impulse *= 0.0;
    velocity *= drag;
    position += velocity;
}


void Particle::draw() const
{
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::blueSteel);
    ofDrawCircle(position, 30);
    ofPopStyle();
}


bool Particle::isDead() const
{
    return age > 100;
}
