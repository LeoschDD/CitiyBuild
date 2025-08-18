#pragma once

#include "Tile.h"
#include "Ecs.h"

class Game;

class World
{
private:
    Game *m_game;
    ecs::Registry m_reg;

    Animation hoverAnim{};

    std::string m_name{};

	uint16_t m_width{};
	uint16_t m_height{};
    
    uint16_t m_tileWidth{64};
    uint16_t m_tileHeight{64};

    std::vector<std::vector<Tile>> m_tileMap;

    std::unordered_map<size_t, ecs::Entity> m_IDToEntity;
	std::queue<size_t> m_freeIDs{};
	size_t m_nextID{0};

private:
    void initRegistry();

    void loadWorldData(std::string path);
    void loadTiles(std::string path);
    void loadEntities(std::string path);
    void load(std::string worldPath);

    void saveWorldData(std::string path);
    void saveTiles(std::string path);
    void saveEntities(std::string path);
    void save(std::string worldPath);

    void generate(std::string worldPath);

    uint32_t getID();
    void freeID(uint32_t id);

public:
    World(Game *game, std::string name);
    World(Game *game, std::string name, uint16_t width, uint16_t height);
    ~World();

    void update();
    void render();
};