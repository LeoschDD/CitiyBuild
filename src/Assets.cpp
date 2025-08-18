#include "Assets.h"

Assets::~Assets()
{
	m_animations.clear();

    for (auto& [key, tex] : m_textures) delete tex;
	m_textures.clear();
}

//--------------------------------------------
// Asset hinzufügen
//--------------------------------------------

void Assets::addTexture(std::string textureKey, std::string texturePath)
{
	if (m_textures.find(textureKey) == m_textures.end())
	{
        SDL_Surface *img = IMG_Load(texturePath.c_str());
        if (!img) throw std::runtime_error("Error: Image " + textureKey + " could not load!");

		SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, img);
		if (!texture) throw std::runtime_error("Error: Texture " + textureKey + " could not be created!");
		
		m_textures[textureKey] = texture;
	}
}

void Assets::addAnimation(std::string animationKey, std::string textureKey, float animationTimer, int startFrameX, int startFrameY, int Frames, int width, int height)
{
	if (m_animations.find(animationKey) == m_animations.end())
	{
		SDL_Texture *texture = getTexture(textureKey);
		if (!texture)
		{
			throw std::runtime_error("Error: Texture " + textureKey + " not found!");
		}
		Animation animation(texture, animationKey, animationTimer, startFrameX, startFrameY, Frames, width, height);
		m_animations[animationKey] = std::move(animation);
	}
}

//--------------------------------------------
// Asset abrufen
//--------------------------------------------

SDL_Texture *Assets::getTexture(std::string textureKey)
{
	if (m_textures.find(textureKey) == m_textures.end()) return nullptr;
	else return m_textures[textureKey];
}


Animation *Assets::getAnimation(std::string animationKey)
{
	if (m_animations.find(animationKey) == m_animations.end()) return nullptr;
	else return &m_animations[animationKey];
}