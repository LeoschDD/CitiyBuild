#include "Game.h"

// help functions for state stack

void Game::pushState(State *state)
{
    m_states.push(state);
}

void Game::popState()
{
    delete m_states.top();
    m_states.pop();
}

void Game::changeState(State *state)
{
    if (!m_states.empty()) popState();
    pushState(state);
}

State *Game::peekState()
{   
    if (m_states.empty()) return nullptr;
    else return m_states.top();
}

// initialisation

bool Game::initData()
{
    m_data = new Data;
    if (!m_data) 
    {
        std::cout << "Error: Data could not be initialized!" << std::endl;
        return false;
    }
    if (!m_data->loadAssetsData("../data/assets.txt")) return false;
    if (!m_data->loadConfigData("../data/config.txt")) return false;

    return true;
}

bool Game::initVideo()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Video initialised!..." << std::endl;

        const char* windowTitle = m_data->windowConfig().title.c_str();
        int windowWidth = m_data->windowConfig().width;
        int windowHeight = m_data->windowConfig().height;

        m_window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, 0);
        if (m_window)
        {
            std::cout << "Window created!..." << std::endl; 

            SDL_SetWindowFullscreen(m_window, m_data->windowConfig().fullscreen);
            m_fpsLimit = m_data->windowConfig().fpsLimit;
        } 
        
        m_renderer = SDL_CreateRenderer(m_window, NULL);
        if (m_renderer)
        {
            std::cout << "Renderer created!..." << std::endl;

            if (m_data->windowConfig().vsync) SDL_SetRenderVSync(m_renderer, -1);
        } 
        return true;
    }
    else 
    {
        std::cout << "Error: Video initialisation failed!" << std::endl;
        return false;
    }
    
}

bool Game::initAudio()
{
    if (SDL_Init(SDL_INIT_AUDIO))
    {
        std::cout << "Audio initialised!..." << std::endl;
        return true;
    }
    else 
    {
        std::cout << "Error: Audio initialisation failed!" << std::endl;
        return false;
    }
}

bool Game::initAssets()
{
    m_assets = new Assets(m_renderer);
    if (!m_assets)
    {
        std::cout << "Error: Assets could not be initialized!" << std::endl;
    }
    for (auto& [key, texture] : m_data->textureData())
    {
        m_assets->addTexture(texture->key, texture->path);
    }
    for (auto& [key, animation] : m_data->animationData())
    {
        m_assets->addAnimation(animation->key, animation->textureKey, animation->animationTimer, animation->startFrameX, animation->startFrameY, animation->Frames, animation->width, animation->height);
    }
    return true;
}

void Game::init()
{
    if (!initData()) m_running = false;
    if (!initVideo()) m_running = false;
    if (!initAudio()) m_running = false;
    if (!initAssets()) m_running = false;

    pushState(new StartState(this));
}

// destruct

void Game::clean()
{
    std::cout << "States cleaned!..." << std::endl;
    while (!m_states.empty()) popState();

    if (m_renderer) 
    {
        std::cout << "Renderer cleaned!..." << std::endl;
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        std::cout << "Window cleaned!..." << std::endl;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    delete m_data;

    SDL_Quit();
    std::cout << "Game cleaned!..." << std::endl;
}

Game::Game()
{
    init();
}

Game::~Game()
{
    clean();
}

// game loop

void Game::run()
{
    m_timeLast = SDL_GetPerformanceCounter();

    while (m_running)
    {   
        m_timeThis = SDL_GetPerformanceCounter();
        m_dt = (double)((m_timeThis - m_timeLast) / (double)SDL_GetPerformanceFrequency());
        m_timeLast = m_timeThis;

        if (peekState() != nullptr)
        {
            peekState()->handleInput(m_dt);
            peekState()->update(m_dt);
            peekState()->render();
        }

        float frameDelay = 1000.0f / m_fpsLimit;
        float frameTime = m_dt * 1000.0f;

        if (frameDelay > frameTime) SDL_Delay((Uint32)(frameDelay - frameTime));   
    }
}   