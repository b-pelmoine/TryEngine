#ifndef TERO_WORLD_HPP
#define TERO_WORLD_HPP

#include "CoreMinimal.hpp"
#include "UserDefinedTypes.hpp"
#include <fstream>

class TEWorld 
{
public:
    TEWorld();

    std::weak_ptr<TEEntities> Entities() noexcept;
private:
    std::shared_ptr<TEEntities> m_entities;
    std::vector<std::weak_ptr<TESystem>> m_systems;
    
    virtual void Load(TEEntities&& entities, Json::Value&& systems);
    virtual void Update();
    virtual Json::Value Serialize() const;

    friend class TEWorldStream;
    friend class TryEngine;
};

class TEWorldStream
{
public:
    TEWorldStream(std::string&& file);

    void Load(const std::string& infile = "");
    void Save(const std::string& outfile = "");
    std::weak_ptr<TEWorld> GetWorld() const { return m_world; }
private:
    std::string m_file;
    std::shared_ptr<TEWorld> m_world;
};

#endif