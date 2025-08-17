#include "State.h"
#include "Game.h"

static int circleCounter = 0;

StartState::StartState(Game *game) : State(game) 
{   
    ECS_REGISTER(m_reg, c::pos);
    ECS_REGISTER(m_reg, c::vel);
    ECS_REGISTER(m_reg, c::circle);
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
    case SDL_EVENT_KEY_DOWN:
        if (event.key.scancode == SDL_SCANCODE_SPACE) {std::cout << circleCounter << std::endl; for(int i = 0; i < 10000; i++) spawnCircle();}
    default:
        break;
    }
}

void StartState::update(const float dt)
{
    m_reg.update();
    sMove(dt);
}

void StartState::render()
{
    SDL_SetRenderDrawColor(m_game->renderer(), 10, 150, 255, 255);
    SDL_RenderClear(m_game->renderer());
    SDL_SetRenderDrawColor(m_game->renderer(), 255, 255, 255, 255);
    sRender();
    SDL_RenderPresent(m_game->renderer());
}

void StartState::sRender()
{
    m_reg.each<c::pos, c::circle>([this](ecs::Entity e)
    {
        auto& cCircle = *m_reg.getComponent<c::circle>(e);
        auto& cPos = *m_reg.getComponent<c::pos>(e);
        DrawCircle(m_game->renderer(), (int)cPos.x, (int)cPos.y, (int)cCircle.radius);
    });
}

void StartState::sMove(const float dt)
{
    m_reg.each<c::pos, c::vel>([this, dt](ecs::Entity e)
    {
        auto& cVel = *m_reg.getComponent<c::vel>(e);
        auto& cPos = *m_reg.getComponent<c::pos>(e);

        cPos.x += cVel.x * dt;
        cPos.y += cVel.y * dt;
    });
}

void StartState::sCollision()
{
    m_reg.each<c::pos, c::vel, c::circle>([this](ecs::Entity e)
    {
        auto& cVel = *m_reg.getComponent<c::vel>(e);
        auto& cPos = *m_reg.getComponent<c::pos>(e);
        auto& cCircle = *m_reg.getComponent<c::circle>(e);
    });
}

void StartState::spawnCircle()
{
    auto circle = m_reg.create();

    m_reg.addComponent<c::pos>(circle, 5.f, 5.f);
    m_reg.addComponent<c::vel>(circle, 20.f, 20.f);
    m_reg.addComponent<c::circle>(circle, 50.f);

    circleCounter++;
}

void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) 
{
        SDL_RenderPoint(renderer, centerX, centerY);
}