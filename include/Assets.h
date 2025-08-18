#pragma once

#include "Animation.h"

class Assets
{
private:
    SDL_Renderer *m_renderer;

	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, Animation> m_animations;

public:
    Assets(SDL_Renderer *renderer) : m_renderer(renderer) {}
	~Assets();

	void addTexture(std::string textureKey, std::string texturePath);
	void addAnimation(std::string animationKey, std::string textureKey, float animationTimer, int startFrameX, int startFrameY, int Frames, int width, int height);

	SDL_Texture *getTexture(std::string textureKey);
	Animation *getAnimation(std::string animationKey);
};


