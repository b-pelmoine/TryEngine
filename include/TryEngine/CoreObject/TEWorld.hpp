#ifndef TERO_WORLD_HPP
#define TERO_WORLD_HPP

#include "CoreMinimal.hpp"
#include "UserDefinedTypes.hpp"
#include <fstream>

class TEWorld 
{
public:
    TEWorld();

    TEEntities& Entities() const noexcept;
private:
    TEEntities m_entities;
    
    virtual void Load(TEEntities&& entities, Json::Value&& systems);
    virtual Json::Value Serialize() const;

    friend class TEWorldStream;
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