#pragma once

#include "Headers.h"
#include "Ecs.h"
#include "Components.h"
#include "Animation.h"

class Game;

class State
{
protected:
    Game *m_game;

    State(Game *game) : m_game(game) {}

public:
    virtual ~State() = default;

    virtual void handleInput(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void render() = 0;
};

class StartState : public State
{
private: 

public:
    StartState(Game *game);

    void handleInput(const float dt) override;
    void update(const float dt) override;
    void render() override;
};
