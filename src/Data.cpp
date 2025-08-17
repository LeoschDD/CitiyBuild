#include "Data.h"

bool Data::loadAssetsData(std::string assetsPath)
{
    std::ifstream assetsFile(assetsPath);
    std::string line;

    if (assetsFile.is_open())
    {
        while (std::getline(assetsFile, line))
        {
            std::stringstream lineStream(line);
            std::string type;
            lineStream >> type;

            if (type == "#" || type.empty()) continue;

            if (type == "Texture")
            {
                std::string key, path;
                lineStream >> key >> path;

                m_textureData.emplace(key, std::make_unique<TexData>(TexData{ key, path }));
            }
            else if (type == "Animation")
            {
                std::string key, textureKey;
                float animationTimer;
                int startFrameX, startFrameY, frames, width, height;

                lineStream >> key >> textureKey >> animationTimer >> startFrameX >> startFrameY >> frames >> width >> height;

                m_animationData.emplace(key, std::make_unique<AniData>(AniData{ key, textureKey, animationTimer, startFrameX, startFrameY, frames, width, height }));
            }
        }
        assetsFile.close();
        return true;
    }
    else
    {
        std::cout << "Error: Could not open assets file: " << assetsPath << std::endl;
        return false;
    }
}

bool Data::loadConfigData(std::string configPath)
{
    std::ifstream configFile(configPath);

    if (configFile.is_open())
    {
        std::string type;

        m_windowConfig = std::make_unique<WindowConfig>();
        m_gameConfig = std::make_unique<GameConfig>();

        configFile >> type >> m_windowConfig->title;
        configFile >> type >> m_windowConfig->width >> m_windowConfig->height;
        configFile >> type >> m_windowConfig->fpsLimit;
        configFile >> type >> m_windowConfig->fullscreen;
        configFile >> type >> m_windowConfig->vsync;

        configFile >> type >> m_gameConfig->renderDistance;

        configFile.close();
        return true;
    }
    else
    {
        std::cout << "Error: Could not open config file: " << configPath << std::endl;
        return false;
    }
}