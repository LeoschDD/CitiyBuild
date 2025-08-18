#include "State.h"
#include "Game.h"

static int circleCounter = 0;

StartState::StartState(Game *game) : State(game) 
{   
    ECS_REGISTER(m_reg, c::pos);
    ECS_REGISTER(m_reg, c::vel);
    ECS_REGISTER(m_reg, c::circle);
    ECS_REGISTER(m_reg, c::cAnimation);
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
        if (event.key.scancode == SDL_SCANCODE_SPACE) {std::cout << circleCounter << std::endl; for(int i = 0; i < 100; i++) spawnCircle();}
    default:
        break;
    }
}

void StartState::update(const float dt)
{
    m_reg.update();
    sMove(dt);
    sCollision();
}

void StartState::render()  
{
    SDL_SetRenderDrawColor(m_game->renderer(), 10, 150, 255, 255);
    SDL_RenderClear(m_game->renderer());

    sRender();

    SDL_RenderPresent(m_game->renderer());
}

void StartState::sRender()
{
    m_reg.each<c::pos, c::cAnimation>([this](ecs::Entity e)
    {
        auto& cAnim = *m_reg.getComponent<c::cAnimation>(e);
        auto& cPos = *m_reg.getComponent<c::pos>(e);
        
        const SDL_FRect *dRect = new SDL_FRect({cPos.x, cPos.y, (float)cAnim.anim.getSprite()->getRect()->w, cAnim.anim.getSprite()->getRect()->h});
        SDL_RenderTexture(m_game->renderer(), cAnim.anim.getSprite()->getTexture(), cAnim.anim.getSprite()->getRect(), dRect);
        delete dRect;
    });
}

void StartState::sMove(const float dt)
{
    m_reg.each<c::pos, c::vel, c::cAnimation>([this, dt](ecs::Entity e)
    {
        auto& cVel = *m_reg.getComponent<c::vel>(e);
        auto& cPos = *m_reg.getComponent<c::pos>(e);
        auto& cAnim = *m_reg.getComponent<c::cAnimation>(e);

        cAnim.anim.update(dt);

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

        if (cPos.x < 0) 
        {
            cPos.x = 0;
            cVel.x = -cVel.x;
        }

        if (cPos.x > m_game->data().windowConfig().width)
        {
            cPos.x = m_game->data().windowConfig().width;
            cVel.x = -cVel.x;
        }
        if (cPos.y < 0)
        {
            cPos.y = 0;
            cVel.y = -cVel.y;
        }
        if (cPos.y > m_game->data().windowConfig().height)
        {
            cPos.y = m_game->data().windowConfig().height;
            cVel.y = -cVel.y;
        }
    });
}

void StartState::spawnCircle()
{
    auto e = m_reg.create();

    m_reg.addComponent<c::pos>(e, float(rand()%1920), float(rand()%1080));
    m_reg.addComponent<c::vel>(e, float(rand()%200) - 100.f, float(rand()%200) - 100.f);
    m_reg.addComponent<c::cAnimation>(e, true, *m_game->assets().getAnimation("player_runDown"));
    circleCounter++;
}