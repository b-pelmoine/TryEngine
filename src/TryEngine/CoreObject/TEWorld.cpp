#include "CoreObject/TEWorld.hpp"

#include <iostream>
#include <json/json.h>

#include "TryEngine.hpp"

TEWorld::TEWorld()
{}

std::weak_ptr<TEEntities> TEWorld::Entities() noexcept
{
    return m_entities;
}

void TEWorld::Load(TEEntities&& entities, Json::Value&& systems)
{
    //entities
    m_entities = std::make_shared<TEEntities>(std::move(entities));
    {
        auto init = [](auto& c){ c.second.lock()->Initialize(); };
        for(auto& entity: entities.m_entities)
        {
            std::for_each(entity->m_components.begin(), entity->m_components.end(), init);
        }
    }
    //systems
    m_systems.clear();
    if(systems.size() > 0)
    {
        TESystemType systemType;
        TESystemID id;
        Json::Value system;
        const size_t typeCount = systems.size();
        m_systems.reserve(typeCount);
        for ( Json::Value::ArrayIndex index = 0; index < typeCount; ++index )
        {
            systemType = systems[index]["TypeId"].asString();
            const size_t systemCount = systems[index]["systems"].size();
            auto it = TESystem::registeredSystems.find(systemType);
            for ( Json::Value::ArrayIndex i = 0; i < systemCount; ++i )
            {
                system = systems[index]["systems"][i];
                id = system["system-id"].asLargestUInt();
                m_systems.push_back(it->second(id));
                std::prev(m_systems.end())->lock()->Load(std::move(system["data"]));
            }
        }
        auto init = [](auto& s){ s.lock()->Initialize(); };
        std::for_each(m_systems.begin(), m_systems.end(), init);
    }
}

void TEWorld::Update()
{
    for(auto& w_sys: m_systems)
    {
        if(const auto& sys = w_sys.lock())
        {
            sys->Update();
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

/* TEWorldStream */

TEWorldStream::TEWorldStream(std::string&& file): m_file(file), m_world(std::make_shared<TEWorld>())
{
    Load();
}

void TEWorldStream::Load(const std::string& file)
{
    std::string inFile = !file.empty() ? file : m_file;
    Json::Value root;
    std::ifstream stream(inFile, std::ifstream::binary);
    if(stream.is_open())
    {
        stream >> root;
        stream.close();
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