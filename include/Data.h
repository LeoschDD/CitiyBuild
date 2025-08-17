#pragma once

#include "Headers.h"

struct TexData
{
	std::string key{};
	std::string path{};
};

struct AniData
{
	std::string key{};
	std::string textureKey{};
	float animationTimer{};
	int startFrameX{}, startFrameY{}, Frames{}, width{}, height{};
};

struct WindowConfig
{
	std::string title{};
	int width{}, height{};
	int fpsLimit{};
	bool fullscreen{};
	bool vsync{};
};

struct GameConfig
{
	int renderDistance{};
};

typedef std::map<std::string, std::unique_ptr<TexData>> TextureData;
typedef std::map<std::string, std::unique_ptr<AniData>> AnimationData;

class Data
{
private:
	TextureData m_textureData;
	AnimationData m_animationData;
	std::unique_ptr<WindowConfig> m_windowConfig;
	std::unique_ptr<GameConfig> m_gameConfig;

public:
	Data() = default;
	~Data() = default;

    bool loadAssetsData(std::string assetsPath);
	bool loadConfigData(std::string configPath);

	TextureData &textureData() { return m_textureData; }
	AnimationData &animationData() { return m_animationData; }
	WindowConfig &windowConfig() { return *m_windowConfig; }
	GameConfig &gameConfig() { return *m_gameConfig; }
};