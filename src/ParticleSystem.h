//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "Particle.h"


class ParticleSystem
{
public:
    ParticleSystem();

    virtual ~ParticleSystem();

    void update();
    void draw() const;

    std::vector<std::shared_ptr<Particle>> particles;

};
