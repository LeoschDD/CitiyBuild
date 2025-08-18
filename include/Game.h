#pragma once

#include "State.h"
#include "Assets.h"

class Game
{
private:
    SDL_Window *m_window; 
    SDL_Renderer *m_renderer;

    Data *m_data;
    Assets *m_assets;

    std::stack<State*> m_states;

    bool m_running{true};
    
    float m_timeLast{};
    float m_timeThis{};
    float m_dt{};
    
    int m_fpsLimit{60};

private: 
    void pushState(State *state);
    void popState();
    void changeState(State *state);
    State *peekState();

    bool initData();
    bool initVideo();
    bool initAudio();
    bool initAssets();
    void init();

    void clean();

public:
    Game();
    ~Game();

    void run();

    SDL_Window *window() {return m_window;}
    SDL_Renderer *renderer() {return m_renderer;}

    Data &data() {return *m_data;}
    Assets &assets() {return *m_assets;}
    bool &running() {return m_running;}
};
