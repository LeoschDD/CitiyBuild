#include "World.h"
#include "Game.h"

// load world data -> tiles -> entities
// tiles and entities both save a id component and "reconnect" when loaded 
// with the m_IDToEntity hashmap so the tile knows wich entity connects to its id component

void World::initRegistry()
{
    ECS_REGISTER(m_reg, cID);
    ECS_REGISTER(m_reg, cPos);
    ECS_REGISTER(m_reg, cVel);
    ECS_REGISTER(m_reg, cAnimation);
}

void World::loadWorldData(std::string path)
{
    std::ifstream file;
    std::string line;
    file.open(path);

    if (file.is_open())
    {
        std::string x;
        std::string type;
        file >> type >> m_width >> m_height;
        if (!(file >> type >> m_nextID))
        {
            m_nextID = 0;
            file.clear();
        }
        if (file >> type && type == "freeGlobalIDs")
        {
        std::string line;
        while (std::getline(file, line))
            {   
                if (line.empty()) continue;
                std::stringstream lineStream(line);
                size_t id;
                if (lineStream >> id) m_freeIDs.push(id);
            }
        }
        file.close();
    }
    else throw std::runtime_error("Error: Could not open file: " + path);
}

void World::loadTiles(std::string path)
{   
    m_tileMap.resize(m_width * m_height);

    std::ifstream file;
    std::string line;
    file.open(path);

    if (file.is_open())
    {
        size_t tileKey;
        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            std::stringstream lineStream(line);
            std::string type;

            lineStream >> type;

            if (type == "key")
            {
                lineStream >> tileKey;
            }
            if (type == "entity")
            {
                uint32_t ID;
                lineStream >> ID;
                if (ID != ecs::NONE) m_tileMap[tileKey].entity() = ID;
            }
            if (type == "occupant")
            {
                uint32_t ID;
                lineStream >> ID;
                if (ID != ecs::NONE) m_tileMap[tileKey].occupant() = ID;
            }
        }
        file.close();
    }
    else throw std::runtime_error("Error: Could not open file: " + path);
}

void World::loadEntities(std::string path)
{
    std::ifstream file;
    std::string line;
    file.open(path);

    if (file.is_open())
    {
        ecs::Entity e;
        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            std::stringstream lineStream(line);
            std::string type;

            lineStream >> type;

            if (type == "#")
            {
                e = m_reg.create();
                continue;
            }
            if (type == "cID")
            {
                uint32_t ID;
                lineStream >> ID;
                m_reg.addComponent<cID>(e, ID);
            }
            if (type == "cPos")
            {
                int x, y;
                lineStream >> x >> y;
                m_reg.addComponent<cPos>(e, x, y);
            }
            if (type == "cVel")
            {
                int x, y;
                lineStream >> x >> y;
                m_reg.addComponent<cPos>(e, x, y);
            }
            if (type == "cAnimation")
            {
                std::string name;
                std::string startType;
                bool loop;
                lineStream >> name >> startType >> loop;
                m_reg.addComponent<cAnimation>(e, loop, m_game->assets(), name, startType);
            }
        }
        file.close();
    }
    else throw std::runtime_error("Error: Could not open file: " + path);

    m_reg.each<cID>([this](ecs::Entity e)
    {
        auto id = m_reg.getComponent<cID>(e)->id;
        m_IDToEntity[id] = std::move(e);
    });
}

void World::load(std::string worldPath)
{
    initRegistry();
    loadWorldData(worldPath + "/data.txt");
    loadTiles(worldPath + "/tiles.txt");
    loadEntities(worldPath + "/entities.txt");
}

// save world

void World::saveWorldData(std::string path)
{
    std::ofstream file;
    file.open(path);

    if (file.is_open())
    {
        file << "mapSize " << m_width << " " << m_height << std::endl;
        file << "nextGlobalID " << m_nextID << std::endl;
        file << "freeGlobalIDs" << std::endl;
        while (!m_freeIDs.empty())
        {
            file << m_freeIDs.front() << std::endl;
            m_freeIDs.pop();
        }
    }
}

void World::saveTiles(std::string path)
{
    std::ofstream file;
    file.open(path);

    if (file.is_open())
    {
        for (int i = 0; i < m_tileMap.size(); ++i)
        {
            auto& tile = m_tileMap[i];
            file << "key " << i << std::endl;
            file << "entity " << tile.entity() << std::endl;
            file << "occupant " << tile.occupant() << std::endl;
            file << std::endl;
        }
        file.close();
    }
    else throw std::runtime_error("Error: Could not open file: " + path);
}

void World::saveEntities(std::string path)
{
    std::ofstream file;
    file.open(path);

    if (file.is_open())
    {
        for (auto e : m_reg.alive())
        {
            file << "# " << e << std::endl;

            if (m_reg.hasComponent<cID>(e))
            {
                auto &id = *m_reg.getComponent<cID>(e);
                file << "cGlobalID " << id.id << std::endl;
            }
            if (m_reg.hasComponent<cPos>(e))
            {
                auto &pos = *m_reg.getComponent<cPos>(e);
                file << "cPos " << pos.x << " " << pos.y << std::endl;
            }
            if (m_reg.hasComponent<cVel>(e))
            {
                auto &vel = *m_reg.getComponent<cVel>(e);
                file << "cVelocity " << vel.x << " " << vel.y << std::endl;
            }
            if (m_reg.hasComponent<cAnimation>(e))
            {
                auto &anim = *m_reg.getComponent<cAnimation>(e);
                file << "cAnimation " << anim.name << " " << anim.type << " " << anim.loop << std::endl;
            }
            file << std::endl;
        }
        file.close();
    }
    else throw std::runtime_error("Error: Could not open file: " + path);
}

void World::save(std::string worldPath)
{
    saveWorldData(worldPath + "/data.txt");
    saveTiles(worldPath + "/tiles.txt");
    saveEntities(worldPath + "/entities.txt");
}

void World::generate(std::string worldPath)
{
    try
    {
        if (std::filesystem::create_directory(worldPath + m_name))
        {
            std::cout << "World is getting generated!..." << std::endl;
        }
        else std::cout << "Error: World generation failed!" << std::endl;
    } 
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

World::World(Game *game, std::string name) : m_game(game), m_name(name)
{
    const std::string worldPath = "../data/worlds/" + m_name;

    if (std::filesystem::exists(worldPath))
    {
        if (std::filesystem::is_directory("../data/worlds/" + m_name)) load(worldPath);
    }
    else 
    {
        generate(worldPath);
    }
}

World::~World()
{
    const std::string worldPath = "../data/worlds/" + m_name;
    save("../data/worlds/" + m_name);
}


