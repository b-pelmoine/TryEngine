#ifndef TERO_ENTITY_HPP
#define TERO_ENTITY_HPP

#include <cassert>
#include <set>
#include <unordered_map>
#include <memory>
#include <algorithm>

#include "json/json.h"

class TEComponent;
using TEComponentType = std::string;

using TEEntityID = Json::LargestUInt;

class TEEntity
{
    public:
    TEEntityID ID() const noexcept { return m_id; };
    bool operator<(const TEEntity& other) const;
    bool operator==(const TEEntity& other) const;

    std::optional<std::weak_ptr<TEComponent>> GetComponent(TEComponentType type) const;

    private: 
    TEEntity(TEEntityID id);
    TEEntityID m_id;

    void AddComponent(std::weak_ptr<TEComponent> component);
    void RemoveComponent(std::weak_ptr<TEComponent> component);

    Json::Value Serialize() const;
    void Load(Json::Value&& data, std::weak_ptr<TEEntity> self);

    std::unordered_map<TEComponentType, std::weak_ptr<TEComponent>> m_components;

    friend class TEEntities;
    template<typename T>
    friend class TEComponents;
    friend class TEWorld;
};

class TEEntities
{
public:
    TEEntities();
    TEEntities(TEEntities&& other);
    std::weak_ptr<TEEntity> Create(bool overrideID = false, Json::LargestUInt ID = 0);
    void Remove(std::weak_ptr<TEEntity> entity);
    std::weak_ptr<TEEntity> Get(TEEntityID id);
    size_t Count() { return m_entities.size(); }
private:
    struct Less {
        bool operator()(const std::shared_ptr<TEEntity> &lhs, 
                        const std::shared_ptr<TEEntity> &rhs) const {
            return (*lhs) < (*rhs);
        }
    };
    std::set<std::shared_ptr<TEEntity>, Less> m_entities;

    void Load(Json::Value&& rawEntities);

    friend class TEWorldStream;
    friend class TEWorld;
};

#endif