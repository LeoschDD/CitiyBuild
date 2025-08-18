#pragma once

#include "Ecs.h"
#include "Animation.h"

namespace c
{
    ECS_COMPONENT(pos)
    {
        float x;
        float y;

        pos(float x, float y) : x(x), y(y) {}
    };

    ECS_COMPONENT(vel)
    {
        float x;
        float y;

        vel(float x, float y) : x(x), y(y) {}
    };

    ECS_COMPONENT(circle)
    {
        float radius;

        circle(float r) : radius(r) {}
    };

    ECS_COMPONENT(cAnimation)
    {
        bool loop;
        Animation anim;

        cAnimation(bool loop, Animation anim) : loop(loop), anim(anim) {}
    };
} 


