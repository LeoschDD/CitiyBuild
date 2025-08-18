#pragma once

#include "World.h"

class Game;

class State
{
protected:
    Game *m_game;
    World *m_world;

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
    ~StartState();

    void handleInput(const float dt) override;
    void update(const float dt) override;
    void render() override;
};
