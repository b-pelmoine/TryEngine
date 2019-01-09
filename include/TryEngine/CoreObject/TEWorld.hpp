#ifndef TERO_WORLD_HPP
#define TERO_WORLD_HPP

#include "CoreMinimal.hpp"
#include "UserDefinedTypes.hpp"
#include <fstream>

class TEWorld 
{
public:
    TEWorld();
    
    void Add(std::weak_ptr<TESystem>); 
    
    std::weak_ptr<TEEntities> Entities() noexcept;

private:
    bool bLoaded;
    std::shared_ptr<TEEntities> m_entities;
    std::vector<std::weak_ptr<TESystem>> m_systems;
    std::vector<std::weak_ptr<TESystem>> m_tickableSystems;
    
    virtual void Load(TEEntities&& entities, Json::Value&& systems);
    void UnLoad();
    virtual void Update();
    virtual Json::Value Serialize() const;
    bool Loaded() const { return bLoaded; }

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
    std::string Filename() const { return m_file; }
private:
    std::string m_file;
    std::shared_ptr<TEWorld> m_world;
};

#endif