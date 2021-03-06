#ifndef TERO_COMPONENT_HPP
#define TERO_COMPONENT_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

#include "TEEntity.hpp"
#include "TESerializable.hpp"
#include "json/json.h"

#define TECOMPONENT_REGISTER_DESTROYER(component) TEComponent::componentsDestroyer[component::TypeID] = [](std::weak_ptr<TEEntity> owner) { TEComponents<component>::RemoveFrom(owner, false); };
#define DESTROY_COMPONENT(componentID, entity) TEComponent::componentsDestroyer[componentID](entity);

class TEComponent : public TESerializable
{
    static std::unordered_map<TEComponentType, std::function<std::weak_ptr<TEComponent>(std::weak_ptr<TEEntity>)>> registeredComponents;
    static std::unordered_map<TEComponentType, std::function<void(std::weak_ptr<TEEntity>)>> componentsDestroyer;
protected:
    TEComponent();
    TEComponent(std::weak_ptr<TEEntity> e, bool loadedFromFile);
    virtual ~TEComponent() {};
    bool bLoadedFromFile;
public:
    std::weak_ptr<TEEntity> Owner() const;
    virtual void Initialize() {}
    virtual TEComponentType Type() const noexcept =0;
private:
    std::weak_ptr<TEEntity> m_entity;

    template<typename T>
    friend class TEComponents;
    friend class TEEntities;
    friend class TEEntity;
    friend class TEWorld;
    friend class UserDefinedTypes;
};

template<class C>
class TEComponents 
{
public:
    static std::weak_ptr<C> AddTo(std::weak_ptr<TEEntity>& entity, bool loading = false);
    static void RemoveFrom(std::weak_ptr<TEEntity> entity, bool deleteFromEntity = true);
private: 
    TEComponents(size_t size = 1024);
    static std::vector<std::shared_ptr<C>> m_components;
};

template<class C>
std::vector<std::shared_ptr<C>> TEComponents<C>::m_components;

template<class C>
TEComponents<C>::TEComponents(size_t size) {
    m_components.reserve(size);
}
 
template<class C>
std::weak_ptr<C> TEComponents<C>::AddTo(std::weak_ptr<TEEntity>& entity, bool loading) {
    assert(!entity.expired());
    
    auto it = std::find_if(m_components.begin(), m_components.end(), 
        [entity](const std::shared_ptr<TEComponent>& comp)-> bool { return entity.lock() == comp->Owner().lock(); });

    if(it != m_components.end())
    {
        return *it;
    }
    else
    {
        m_components.emplace_back(std::make_shared<C>(C(entity, loading)));
        auto comp = m_components.back();
        entity.lock()->AddComponent(comp);
        return comp;
    }
}

template<class C>
void TEComponents<C>::RemoveFrom(std::weak_ptr<TEEntity> entity, bool deleteFromEntity) {
    assert(!entity.expired());
    auto it = std::remove_if(begin(m_components), end(m_components), 
        [entity](const std::shared_ptr<TEComponent>& component){ return component->Owner().lock() == entity.lock(); });
    m_components.erase(it, m_components.end());
    if(deleteFromEntity)
        entity.lock()->RemoveComponent((*it));
}

#endif