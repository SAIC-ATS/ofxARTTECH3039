//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::update()
{
    auto iter = particles.begin();

    while (iter != particles.end())
    {
        (*iter)->update();

        if ((*iter)->isDead())
        {
            iter = particles.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}


void ParticleSystem::draw() const
{
    for (const auto& particle: particles)
    {
        particle->draw();
    }
}
