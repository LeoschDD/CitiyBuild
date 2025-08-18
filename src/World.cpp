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
        if (file >> type && type == "freeIDs")
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
    m_tileMap = std::vector<std::vector<Tile>>(m_width, std::vector<Tile>(m_height, Tile()));

    std::ifstream file;
    std::string line;
    file.open(path);

    if (file.is_open())
    {
        size_t tileX;
        size_t tileY;
        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            std::stringstream lineStream(line);
            std::string type;

            lineStream >> type;

            if (type == "key")
            {
                lineStream >> tileX >> tileY;
            }
            if (type == "hovered")
            {
                bool hovered;
                lineStream >> hovered;
                m_tileMap[tileX][tileY].hovered() = hovered;
            }
            if (type == "entity")
            {
                uint32_t ID;
                lineStream >> ID;
                if (ID != ecs::NONE) m_tileMap[tileX][tileY].entity() = ID;
            }
            if (type == "occupant")
            {
                uint32_t ID;
                lineStream >> ID;
                if (ID != ecs::NONE) m_tileMap[tileX][tileY].occupant() = ID;
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
                m_reg.addComponent<cVel>(e, x, y);
            }
            if (type == "cAnimation")
            {
                std::string name;
                std::string startType;
                bool loop;
                lineStream >> name >> startType >> loop;
                m_reg.addComponent<cAnimation>(e, loop, &m_game->assets(), name, startType);
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
    auto tempFreeIDs = m_freeIDs;
    if (file.is_open())
    {
        file << "mapSize " << m_width << " " << m_height << std::endl;
        file << "nextID " << m_nextID << std::endl;
        file << "freeIDs" << std::endl;
        while (!tempFreeIDs.empty())
        {
            file << tempFreeIDs.front() << std::endl;
            tempFreeIDs.pop();
        }
    }
}

void World::saveTiles(std::string path)
{
    std::ofstream file;
    file.open(path);

    if (file.is_open())
    {
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                auto& tile = m_tileMap[x][y];
                file << "key " << x << " " << y << std::endl;
                file << "hovered " << tile.hovered() << std::endl;
                file << "entity " << tile.entity() << std::endl;
                file << "occupant " << tile.occupant() << std::endl;
                file << std::endl;
            }
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
                file << "cID " << id.id << std::endl;
            }
            if (m_reg.hasComponent<cPos>(e))
            {
                auto &pos = *m_reg.getComponent<cPos>(e);
                file << "cPos " << pos.x << " " << pos.y << std::endl;
            }
            if (m_reg.hasComponent<cVel>(e))
            {
                auto &vel = *m_reg.getComponent<cVel>(e);
                file << "cVel " << vel.x << " " << vel.y << std::endl;
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
    if (std::filesystem::create_directory(worldPath))
    {
        std::cout << "World is getting generated!..." << std::endl;
    }
    else 
    {
        std::cout << "Error: World generation failed!" << std::endl;
        return;
    }

    std::ofstream data(worldPath + "/data.txt", std::ios::trunc);
    if (!data) throw std::runtime_error("Error: Could not create data.txt");

    data << "mapSize " << m_width << ' ' << m_height << std::endl;
    data << "nextID "  << m_nextID << std::endl;
    data << "freeIDs"  << std::endl;

    std::ofstream tiles(worldPath + "/tiles.txt", std::ios::trunc);
    if (!tiles) throw std::runtime_error("Error: Could not create tiles.txt");    

    std::ofstream entities(worldPath + "/entities.txt", std::ios::trunc);
    if (!entities) throw std::runtime_error("Error: Could not create entities.txt");    
}

uint32_t World::getID()
{
    uint32_t id;

    if (!m_freeIDs.empty())
    {
        id = m_freeIDs.front();
        m_freeIDs.pop();
    }
    else
    {
        id = m_nextID;
        ++m_nextID;
    }
    return id;
}

void World::freeID(uint32_t id)
{
    m_freeIDs.push(id);
    m_IDToEntity.erase(id);
}

World::World(Game *game, std::string name) 
    : m_game(game), m_name(name), m_width(512), m_height(512)
{
    const std::string worldPath = "../data/worlds/" + m_name;

    if (std::filesystem::exists(worldPath))
    {
        if (std::filesystem::is_directory("../data/worlds/" + m_name)) load(worldPath);
    }
    else 
    {
        generate(worldPath);
        load(worldPath);
    }
    hoverAnim = *m_game->assets().getAnimation("hover_default");
}

World::World(Game *game, std::string name, uint16_t width, uint16_t height) 
    : m_game(game), m_name(name), m_width(width), m_height(height)
{
    const std::string worldPath = "../data/worlds/" + m_name;

    if (std::filesystem::exists(worldPath))
    {
        if (std::filesystem::is_directory(worldPath)) load(worldPath);
    }
    else 
    {
        generate(worldPath);
        load(worldPath);
    }
    hoverAnim = *m_game->assets().getAnimation("hover_default");
}

World::~World()
{
    const std::string worldPath = "../data/worlds/" + m_name;
    save(worldPath);
}

void World::update()
{
    float mousePosX;
    float mousePosY;

    int windowPosX;
    int windowPosY;

    SDL_GetWindowPosition(m_game->window(), &windowPosX, &windowPosY);
    SDL_GetGlobalMouseState(&mousePosX, &mousePosY);

    size_t mouseTileX = static_cast<size_t>(floor((mousePosX - windowPosX) / m_tileWidth));
    size_t mouseTileY = static_cast<size_t>(floor((mousePosY - windowPosY) / m_tileHeight));

    if (mouseTileX < m_width && mouseTileY < m_height)
    {
        m_tileMap[mouseTileX][mouseTileY].hovered() = true;
    }
}

void World::render()
{
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            auto &t = m_tileMap[x][y];
            if(t.hovered())
            {
                float posX = x * m_tileWidth;
                float posY = y * m_tileHeight;

                const SDL_FRect dRect = {posX, posY, (float)hoverAnim.getSprite()->getTexture()->w, (float)hoverAnim.getSprite()->getTexture()->h};
                SDL_RenderTexture(m_game->renderer(), hoverAnim.getSprite()->getTexture(), hoverAnim.getSprite()->getRect(), &dRect);
                t.hovered() = false;

                if (t.occupant() == ecs::NONE)
                {
                    uint32_t id = getID();

                    auto e = m_reg.create();
                    m_reg.addComponent<cPos>(e, (int)posX, (int)posY);
                    m_reg.addComponent<cAnimation>(e, true, &m_game->assets(), "hover", "default");
                    m_reg.addComponent<cID>(e, id);
                    t.occupant() = id;                    
                }
            }
        }
    }
    m_reg.each<cAnimation, cPos>([this](ecs::Entity e)
    {
        auto &pos = *m_reg.getComponent<cPos>(e);
        auto &anim = *m_reg.getComponent<cAnimation>(e);

        const SDL_FRect dRect = {(float)pos.x, (float)pos.y, (float)hoverAnim.getSprite()->getTexture()->w, (float)hoverAnim.getSprite()->getTexture()->h};
        SDL_RenderTexture(m_game->renderer(), hoverAnim.getSprite()->getTexture(), hoverAnim.getSprite()->getRect(), &dRect);
    });
}