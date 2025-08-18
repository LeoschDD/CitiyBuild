#pragma once

#include "Ecs.h"
#include "Animation.h"
#include "Assets.h"

ECS_COMPONENT(cID)
{
    uint32_t id;

    cID(uint32_t id) 
        : id(id) {}
};

ECS_COMPONENT(cPos)
{
    int x, y;

    cPos(int x, int y) 
        : x(x), y(y) {}
};

ECS_COMPONENT(cVel)
{
    int x, y;

    cVel(int x, int y) 
        : x(x), y(y) {}
};

ECS_COMPONENT(cAnimation)
{
    bool loop;
    Animation animation;
    std::string name;
    std::string type;

    cAnimation(bool loop, Assets* assets, std::string name, std::string type)
        : loop(loop), animation(*assets->getAnimation(name + "_" + type)), name(name), type(type) {}
};

