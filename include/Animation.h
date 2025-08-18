#pragma once 

#include "Data.h"

class Sprite
{
private:
    SDL_Texture *m_texture{};
    SDL_FRect m_rect{};
    int m_x{}, m_y{};
public:
    Sprite() = default;
    Sprite(SDL_Texture *texture) : m_texture(texture) {}
    Sprite(SDL_Texture *texture, float rectX, float rectY, float rectWidth, float rectHeight) : m_texture(texture), m_rect({rectX, rectY, rectWidth, rectHeight}) {}
    Sprite(SDL_Texture *texture, float rectX, float rectY, float rectWidth, float rectHeight, int x, int y) : m_texture(texture), m_rect({rectX, rectY, rectWidth, rectHeight}), m_x(x), m_y(y) {}
    ~Sprite() = default;

    void setPos(int x, int y) {m_x = x, m_y = y;}
    void setTextureRect(float x, float y, float width, float height) {m_rect = SDL_FRect({x, y, width, height});}
    void setTextureRect(SDL_FRect rect) {m_rect = rect;}

    SDL_Texture *getTexture() {return m_texture;}
    const SDL_FRect *getRect() const {return &m_rect;}
};

class Animation
{
private:
	SDL_Texture *m_texture{};
	Sprite m_sprite{};
	SDL_FRect m_startRect{};
	SDL_FRect m_endRect{};
	SDL_FRect m_currentRect{};

	std::string m_key{};

	int m_width{};
	int m_height{};

	float m_timer{};
	float m_animationTimer{};

	bool m_hasEnded{ false };

public:
	Animation() = default;
	Animation(SDL_Texture *texture, std::string key, float animationTimer, int startFrameX, int startFrameY, int Frames, int width, int height);
	~Animation() = default;

	void update(const float dt);
	bool end() const { return m_hasEnded; }

	Sprite *getSprite() { return &m_sprite; }
	std::string *getKey() { return &m_key; }
};
