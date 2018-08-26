#ifndef TERO_SYSTEM_HPP
#define TERO_SYSTEM_HPP

#include "TEEntity.hpp"
#include <memory>

class TESystem {
    public:
    virtual void Update() = 0;
};

template<class System>
class TESystems 
{
    public: 
    TESystems(size_t size = 1000);
    System& Create(TEEntity& entity);
    void Remove(TEEntity entity);
protected: // can add algos in derived classes
    std::vector<System> m_components{};   // not sorted array
};

template<class System>
TESystems<System>::TESystems(size_t size) {
    m_components.reserve(size);
}

template<class System>
System& TESystems<System>::Create(TEEntity& entity) {
    m_components.push_back(System(entity));
    return m_components.back();
}

template<class System>
void TESystems<System>::Remove(TEEntity entity) {
    auto it = std::remove_if(begin(m_components), end(m_components), 
        [entity](const System& component){ return (component.Entity() == entity); });
    m_components.erase(it, m_components.end());
}

#endif