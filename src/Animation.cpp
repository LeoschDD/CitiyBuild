#include "Animation.h"

Animation::Animation(SDL_Texture *texture, std::string key, float animationTimer, int startFrameX, int startFrameY, int Frames, int width, int height) 
	: m_texture(texture), m_key(key), m_animationTimer(animationTimer), m_width(width), m_height(height)
{
	float startX = startFrameX * width;
	float startY = startFrameY * height;
	m_startRect = SDL_FRect({startX, startY, (float)width, (float)height });

	float endX = (startFrameX + Frames - 1) * width;
	float endY = startFrameY * height;
	m_endRect = SDL_FRect({endX, endY, (float)width, (float)height});

	m_currentRect = m_startRect;

	m_sprite = Sprite(texture, m_startRect);
}

void Animation::update(const float dt)
{
	m_timer += dt;

	if (m_timer >= m_animationTimer)
	{
		m_timer = 0.0f;
		m_currentRect.x += m_width;
		if (m_currentRect.x > m_endRect.x)
		{
			m_currentRect.x = m_startRect.x;
			m_hasEnded = true;
		}
		m_sprite.setTextureRect(m_currentRect);
	}	
}