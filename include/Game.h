#pragma once

#include "State.h"
#include "Data.h"

class Game
{
private:
    SDL_Window *m_window; 
    SDL_Renderer *m_renderer;

    Data *m_data;

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

    bool initVideo();
    bool initAudio();
    bool initData();
    void init();

    void clean();

public:
    Game();
    ~Game();

    void run();

    SDL_Window *window() {return m_window;}
    SDL_Renderer *renderer() {return m_renderer;}

    bool &running() {return m_running;}
};
