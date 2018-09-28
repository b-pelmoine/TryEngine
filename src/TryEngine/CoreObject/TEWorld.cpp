#include "CoreObject/TEWorld.hpp"

#include <iostream>
#include <json/json.h>

#include "TryEngine.hpp"

TEWorld::TEWorld()
{}

void TEWorld::Load(TEEntities&& entities, Json::Value&& systems)
{
    //entities
    m_entities = entities;
    {
        auto init = [](auto& c){ c.second.lock()->Initialize(); };
        for(auto& entity: entities.m_entities)
        {
            std::for_each(entity->m_components.begin(), entity->m_components.end(), init);
        }
    }
    //systems
    if(systems.size() > 0)
    {
        TESystemType systemType;
        TESystemID id;
        Json::Value system;
        std::vector<std::weak_ptr<TESystem>> loadedSystems;
        const size_t typeCount = systems.size();
        loadedSystems.reserve(typeCount);
        for ( int index = 0; index < typeCount; ++index )
        {
            systemType = systems[index]["TypeId"].asString();
            const size_t systemCount = systems[index]["systems"].size();
            auto it = TESystem::registeredSystems.find(systemType);
            for ( int i = 0; i < systemCount; ++i )
            {
                system = systems[index]["systems"][i];
                id = system["system-id"].asLargestUInt();
                loadedSystems.push_back(it->second(id));
                std::prev(loadedSystems.end())->lock()->Load(std::move(system["data"]));
            }
        }
        auto init = [](auto& s){ s.lock()->Initialize(); };
        std::for_each(loadedSystems.begin(), loadedSystems.end(), init);
    }
}

Json::Value TEWorld::Serialize() const
{
    Json::Value world;
    {
        Json::Value entities;
        Json::ArrayIndex entCounter = 0;
        for(const auto& entity : m_entities.m_entities)
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
    Save("Resources/worlds/default1.world.json");
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