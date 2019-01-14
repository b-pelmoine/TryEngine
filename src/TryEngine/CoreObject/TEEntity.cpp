#include "CoreObject/TEEntity.hpp" 
#include "CoreObject/TEComponent.hpp"

#include <assert.h>
#include <iostream>
/* TEEntity */

TEEntity::TEEntity(TEEntityID id) : m_id(id) 
{}

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
    m_components.erase(component.lock()->Type());
}

Json::Value TEEntity::Serialize() const
{
    Json::Value entity(Json::arrayValue);
    Json::ArrayIndex compCounter = 0;
    for(const auto& component : m_components)
    {
        entity[compCounter]["TypeId"] = component.second.lock()->Type();
        entity[compCounter]["data"] = component.second.lock()->Serialize();
        ++compCounter;
    }
    return entity;
}

void TEEntity::Load(Json::Value&& data, std::weak_ptr<TEEntity> self)
{
    if(data.size() == 0) return;
    for ( Json::Value::ArrayIndex index = 0; index < data.size(); ++index )
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
    for ( Json::Value::ArrayIndex i = 0; i < rawEntities.size(); ++i )
    {
        auto entity = TEEntities::Create(true, i);
        entity.lock()->Load(std::move(rawEntities[i]), entity);
    }
}

TEEntities::TEEntities() {}

TEEntities::TEEntities(TEEntities&& other)
{
    m_entities = other.m_entities;
}

std::weak_ptr<TEEntity> TEEntities::Get(TEEntityID id)
{
    auto isID = [id](std::shared_ptr<TEEntity> e) { return e->ID() == id; };
    auto it = std::find_if (m_entities.begin(), m_entities.end(), isID);
    return (it != m_entities.end()) ? *it : nullptr;
}