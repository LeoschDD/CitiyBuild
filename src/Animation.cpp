#include "Animation.h"

Animation::Animation(SDL_Texture *texture, std::string key, float animationTimer, int startFrameX, int startFrameY, int Frames, int width, int height) :
	m_texture(texture),
	m_sprite(texture),
	m_startRect({ float(startFrameX * width), float(startFrameY * height), (float)width, (float)height }),
	m_endRect({ float((startFrameX + Frames - 1) * width), float(startFrameY * height) , (float)width, (float)height }),
	m_currentRect(m_startRect),
	m_key(key),
	m_width(width),
	m_height(height),
	m_animationTimer(animationTimer),
	m_timer(0.0f)
{
	m_sprite.setTextureRect(m_startRect);
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