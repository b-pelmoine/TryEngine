#include "CoreObject/TEWorld.hpp"

#include <iostream>
#include <json/json.h>

#include "TryEngine.hpp"

TEWorld::TEWorld(): 
bLoaded(false), 
m_entities(nullptr), 
m_systems(std::vector<std::weak_ptr<TESystem>>())
{}

std::weak_ptr<TEEntities> TEWorld::Entities() noexcept
{
    return m_entities;
}

void TEWorld::Add(std::weak_ptr<TESystem> sys)
{
    sys.lock()->Initialize();
    m_systems.push_back(sys);
}

void TEWorld::Load(TEEntities&& entities, Json::Value&& systems)
{
    //entities
    m_entities = std::make_shared<TEEntities>(std::move(entities));
    //systems
    m_systems.clear();
    m_tickableSystems.clear();
    if(systems.size() > 0)
    {
        TESystemType systemType;
        const size_t typeCount = systems.size();
        m_systems.reserve(typeCount);
        for ( Json::Value::ArrayIndex index = 0; index < typeCount; ++index )
        {
            systemType = systems[index]["TypeId"].asString();
            const size_t systemCount = systems[index]["systems"].size();
            auto it = TESystem::registeredSystems.find(systemType);
            for ( Json::Value::ArrayIndex i = 0; i < systemCount; ++i )
            {
                m_systems.push_back(it->second(i));
                auto sys = std::prev(m_systems.end())->lock();
                sys->Load(std::move(systems[index]["systems"][i]), m_entities);
                m_tickableSystems.push_back(sys);
            }
        }
        auto init = [](auto& s){ s.lock()->Initialize(); };
        std::for_each(m_systems.begin(), m_systems.end(), init);
    }
    //mark as loaded
    bLoaded = true;
}

void TEWorld::Update()
{
    for(auto& w_sys: m_tickableSystems)
    {
        if(const auto& sys = w_sys.lock())
        {
            sys->Tick();
        }
    }
    auto element = std :: remove_if (m_systems.begin(), m_systems.end() , [](const std::weak_ptr<TESystem>& w_sys){ return w_sys.expired(); });
    m_systems.erase(element, m_systems.end());
}

Json::Value TEWorld::Serialize() const
{
    Json::Value world;
    {
        std::cout << m_entities->m_entities.size() << " entities" << std::endl;
        Json::Value entities;
        Json::ArrayIndex entCounter = 0;
        for(const auto& entity : m_entities->m_entities)
        {
            entities[entCounter++] = entity->Serialize();
        }
        world["entities"] = entities;
    }
    world["systems"] = TE.UDT().SerializeAllSystems();

    return world;
}

void TEWorld::UnLoad()
{
    if(!bLoaded) return;
    for(auto& w_sys: m_systems)
    {
        DESTROY_SYS(w_sys)
    }
    while(!m_entities->m_entities.empty())
    {
        m_entities->Remove(*m_entities->m_entities.begin());
    }
}

/* TEWorldStream */

TEWorldStream::TEWorldStream(std::string&& file): m_file(file), m_world(std::make_shared<TEWorld>())
{
    Load();
}

void TEWorldStream::Load(const std::string& file)
{
    std::string inFile = !file.empty() ? file : m_file;
    m_file = inFile;
    Json::Value root;
    std::ifstream stream(inFile, std::ifstream::binary);
    if(stream.is_open())
    {
        stream >> root;
        stream.close();

        m_world->UnLoad();

        TEEntities entities;
        entities.Load(std::move(root["entities"]));
        m_world->Load(std::move(entities), std::move(root["systems"]));
    }
}


void TEWorldStream::Save(const std::string& file)
{
    const std::string& outFilePath = !file.empty() ? file : m_file;

    Json::Value value = m_world->Serialize();

    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(
        builder.newStreamWriter());
    std::ofstream outfile (outFilePath);
    writer->write(value, &outfile);
    outfile.close();
}