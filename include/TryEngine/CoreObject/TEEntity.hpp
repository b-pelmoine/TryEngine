#ifndef TERO_ENTITY_HPP
#define TERO_ENTITY_HPP

#include <cassert>
#include <set>
#include <unordered_map>
#include <memory>
#include <algorithm>

class TEComponent;
using TEComponentType = std::string;

using TEEntityID = size_t;

class TEEntity
{
    public:
    TEEntityID ID() const noexcept { return m_id; };
    bool operator<(const TEEntity& other) const;
    bool operator==(const TEEntity& other) const;

    std::optional<std::weak_ptr<TEComponent>> GetComponent(TEComponentType type) const;

    private: 
    TEEntity(TEEntityID id) : m_id(id) {};
    TEEntityID m_id;

    void AddComponent(std::weak_ptr<TEComponent> component);
    void RemoveComponent(std::weak_ptr<TEComponent> component);

    std::unordered_map<TEComponentType, std::weak_ptr<TEComponent>> m_components;

    friend class TEEntities;
    template<typename T>
    friend class TEComponents;
};

class TEEntities
{
public:
    std::weak_ptr<TEEntity> Create();
    void Remove(std::weak_ptr<TEEntity> entity);
private:
    struct Less {
        bool operator()(const std::shared_ptr<TEEntity> &lhs, 
                        const std::shared_ptr<TEEntity> &rhs) const {
            return (*lhs) < (*rhs);
        }
    };
    std::set<std::shared_ptr<TEEntity>, Less> m_entities;
};

#endif