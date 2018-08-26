#ifndef TERO_COMPONENT_HPP
#define TERO_COMPONENT_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <map>

#include "TEEntity.hpp"

class TEComponent
{
protected:
    TEComponent();
    TEComponent(std::weak_ptr<TEEntity> e);
    TEComponent(TEComponent&& component) = default;
    virtual ~TEComponent() {};
public:
    std::weak_ptr<TEEntity> Owner() const;
    virtual TEComponentType Type() const noexcept =0;
private:
    std::weak_ptr<TEEntity> m_entity;
    virtual void Destroy() =0;

    template<typename T>
    friend class TEComponents;
    friend class TEEntities;
};

template<class C>
class TEComponents 
{
public:
    static std::weak_ptr<C> AddTo(std::weak_ptr<TEEntity>& entity);
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
std::weak_ptr<C> TEComponents<C>::AddTo(std::weak_ptr<TEEntity>& entity) {
    assert(!entity.expired());
    
    auto it = std::find_if(m_components.begin(), m_components.end(), 
        [entity](const std::shared_ptr<TEComponent>& comp)-> bool { return entity.lock() == comp->Owner().lock(); });

    if(it != m_components.end())
    {
        return *it;
    }
    else
    {
        m_components.push_back(std::make_shared<C>(C(entity)));
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