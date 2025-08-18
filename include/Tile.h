#pragma once

#include "Animation.h"
#include "Components.h"

struct Tile
{
private:
    bool m_hovered{};
    uint32_t m_entity{ecs::NONE};
    uint32_t m_occupant{ecs::NONE};
    
public:
    Tile() = default;
    ~Tile() = default;

    bool &hovered() {return m_hovered;}
    uint32_t &entity() {return m_entity;}
    uint32_t &occupant() {return m_occupant;}
};