#include "State.h"
#include "Game.h"

static int circleCounter = 0;

StartState::StartState(Game *game) : State(game) 
{   
    m_world = new World(m_game, "world3", 20, 20);
}

StartState::~StartState()
{
    delete m_world;
}

void StartState::handleInput(const float dt)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
        m_game->running() = false;
        break;
    default:
        break;
    }
}

void StartState::update(const float dt)
{
    m_world->update();
}

void StartState::render()  
{
    SDL_SetRenderDrawColor(m_game->renderer(), 10, 150, 255, 255);
    SDL_RenderClear(m_game->renderer());
    m_world->render();
    SDL_RenderPresent(m_game->renderer());
}