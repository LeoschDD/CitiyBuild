#pragma once

#include "Tile.h"
#include "Ecs.h"

class Game;

class World
{
private:
    Game *m_game;
    ecs::Registry m_reg;

    std::string m_name{};

	size_t m_width{};
	size_t m_height{};

    std::vector<Tile> m_tileMap;

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

public:
    World(Game *game, std::string name);
    ~World() = default;

    void update();
    void render();
};