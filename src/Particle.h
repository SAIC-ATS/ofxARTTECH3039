//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofTypes.h"


class Particle
{
public:
    Particle();
    Particle(const glm::vec2& position);
    Particle(const glm::vec2& position, const glm::vec2& velocity);

    virtual ~Particle();

    virtual void update();
    virtual void draw() const;

    virtual bool isDead() const;

    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 gravity;
    glm::vec2 impulse;
    float drag = 0.98;

    uint64_t age = 0;
};

