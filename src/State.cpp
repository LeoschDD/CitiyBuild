#include "State.h"
#include "Game.h"

static int circleCounter = 0;

StartState::StartState(Game *game) : State(game) 
{   

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

}

void StartState::render()  
{
    SDL_SetRenderDrawColor(m_game->renderer(), 10, 150, 255, 255);
    SDL_RenderClear(m_game->renderer());

    SDL_RenderPresent(m_game->renderer());
}