#include "CoreObject/TEEntity.hpp" 
#include "CoreObject/TEComponent.hpp"

#include <assert.h>
/* TEEntity */

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

/* TEEntities */

std::weak_ptr<TEEntity> TEEntities::Create() {
    const auto it = std::adjacent_find(m_entities.begin(), m_entities.end(), 
        [](std::shared_ptr<TEEntity> lhs,std::shared_ptr<TEEntity> rhs)
        { return ((lhs->ID())+1 != rhs->ID()); });
    
    auto result = m_entities.insert(
        std::make_shared<TEEntity> ( (it == m_entities.end()) ? TEEntity(m_entities.size()) : TEEntity((*it)->ID()+1) )
        );
    return *(result.first);
}
 
void TEEntities::Remove(std::weak_ptr<TEEntity> entity) {
    assert(!entity.expired());
    auto& components = entity.lock()->m_components;
    std::for_each(components.begin(), components.end(),
    [&](auto& comp){ comp.second.lock()->Destroy(); });
    m_entities.erase(entity.lock());
}
