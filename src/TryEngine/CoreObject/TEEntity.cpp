#include "CoreObject/TEEntity.hpp" 
#include "CoreObject/TEComponent.hpp"

#include <assert.h>
#include <iostream>
/* TEEntity */

TEEntity::TEEntity(TEEntityID id) : m_id(id) 
{};

bool TEEntity::operator<(const TEEntity& other) const
{
    return m_id < other.ID();
}

bool TEEntity::operator==(const TEEntity& other) const
{
    return m_id == other.ID();
}
 
void TEEntity::AddComponent(std::weak_ptr<TEComponent> component)
{
    m_components[component.lock()->Type()] = component;
}

void TEEntity::RemoveComponent(std::weak_ptr<TEComponent> component)
{
    auto it = m_components.find(component.lock()->Type());
    assert(it != m_components.end());
    m_components.erase(component.lock()->Type());
}

std::optional<std::weak_ptr<TEComponent>> TEEntity::GetComponent(TEComponentType type) const
{
    auto it = m_components.find(type);
    return (it != m_components.end()) ? std::optional<std::weak_ptr<TEComponent>>{it->second} : std::nullopt;
}

Json::Value TEEntity::Serialize() const
{
    Json::Value entity;
    entity["entity-id"] = m_id;
    {
        Json::Value components(Json::arrayValue);
        Json::ArrayIndex compCounter = 0;
        for(const auto& component : m_components)
        {
            components[compCounter]["TypeId"] = component.second.lock()->Type();
            components[compCounter]["data"] = component.second.lock()->Serialize();
            ++compCounter;
        }
        entity["components"] = components;
    }
    return entity;
}

void TEEntity::Load(Json::Value&& data, std::weak_ptr<TEEntity> self)
{
    if(data.size() == 0) return;
    for ( int index = 0; index < data.size(); ++index )
    {
        Json::Value component = data[index];
        TEComponentType type = component["TypeId"].asString();
        auto it = TEComponent::registeredComponents.find(type);
        it->second(self).lock()->Load(std::move(component["data"]));
    }
}

/* TEEntities */

std::weak_ptr<TEEntity> TEEntities::Create(bool overrideID, Json::LargestUInt ID) {
    Json::LargestUInt newID;
    if(overrideID)
    {
        newID = ID;
    }
    else
    {
        const auto it = std::adjacent_find(m_entities.begin(), m_entities.end(), 
        [](std::shared_ptr<TEEntity> lhs,std::shared_ptr<TEEntity> rhs)
        { return ((lhs->ID())+1 != rhs->ID()); });
        newID = (it == m_entities.end()) ? m_entities.size() : (*it)->ID()+1;
    }
    auto result = m_entities.insert(
        std::make_shared<TEEntity> (TEEntity(newID))
        );
    return *(result.first);
}
 
void TEEntities::Remove(std::weak_ptr<TEEntity> entity) {
    assert(!entity.expired());
    auto& components = entity.lock()->m_components;
    std::for_each(components.begin(), components.end(),
    [&](auto& comp){ DESTROY_COMPONENT(comp.second.lock()->Type(), entity) });
    m_entities.erase(entity.lock());
}

void TEEntities::Load(Json::Value&& rawEntities)
{
    for(auto& rawEntity : rawEntities)
    {
        auto entity = TEEntities::Create(true, rawEntity["entity-id"].asLargestUInt());
        entity.lock()->Load(std::move(rawEntity["components"]), entity);
    }
}
