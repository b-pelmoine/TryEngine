#ifndef TERO_SYSTEM_HPP
#define TERO_SYSTEM_HPP

#include "TESerializable.hpp"
#include "TEEntity.hpp"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

using TESystemType = std::string;
using TESystemID = Json::LargestUInt;

class TESerializableSystem : public TESerializable
{
    public:
    virtual void Load(Json::Value&& data __attribute__((unused))) {};
    virtual void Load(Json::Value&& data, std::shared_ptr<TEEntities> entities) = 0;
};

#define DESTROY_SYS(system, w_pointer) if(auto sys = std::dynamic_pointer_cast<system> (w_pointer.lock()) ){TESystems<system>::Remove(std::weak_ptr<system>(sys));}

class TESystem : public TESerializableSystem
{
    static std::unordered_map<TESystemType, std::function<std::weak_ptr<TESystem>(TESystemID)>> registeredSystems;
    static std::unordered_map<TESystemType, std::function<void(std::weak_ptr<TESystem>)>> systemsDestroyer;
    public:
    TESystem(Json::LargestUInt ID): bCanEverTick(false), m_id(ID) {};
    virtual ~TESystem() {};
    virtual TESystemType Type() const =0;
    virtual TESystemID ID() const { return m_id; };
    virtual bool CanTick() const { return bCanEverTick; }
    virtual void Tick() {};
    virtual void OnDestroy(std::weak_ptr<TESystem> self) =0;

    protected:
    bool bCanEverTick;

    private:
    TESystemID m_id;

    friend class UserDefinedTypes;
    template<class S>
    friend class TESystems;
    friend class TEWorld;
};

template<class S>
class TESystems 
{
public:
    static std::weak_ptr<S> Create(bool overrideID = false, Json::LargestUInt ID = 0);
    static void Remove(std::weak_ptr<S> system);
private: 
    TESystems(size_t size = 1024);
    static std::vector<std::shared_ptr<S>> m_systems;

    friend class UserDefinedTypes;
};

template<class S>
std::vector<std::shared_ptr<S>> TESystems<S>::m_systems;

template<class S>
TESystems<S>::TESystems(size_t size) {
    m_systems.reserve(size);
}
template<class S>
std::weak_ptr<S> TESystems<S>::Create(bool overrideID, Json::LargestUInt ID) {
    Json::LargestUInt newID;
    typename std::vector<std::shared_ptr<S>>::iterator Iterator;
    if(overrideID)
    {
        newID = ID;
        m_systems.push_back(
            std::make_shared<S>(S(newID))
            );
        Iterator = std::prev(m_systems.end());
    }
    else
    {
        const auto it = std::adjacent_find(m_systems.begin(), m_systems.end(), 
        [](std::shared_ptr<S> lhs,std::shared_ptr<S> rhs)
        { return ((lhs->ID())+1 != rhs->ID()); });
        newID = (it == m_systems.end()) ? m_systems.size() : (*it)->ID()+1;
        Iterator = m_systems.insert(
            it,
            std::make_shared<S> (S(newID))
            );
    }
    return *(Iterator);
}

template<class S>
void TESystems<S>::Remove(std::weak_ptr<S> system) {
    assert(!system.expired());
    m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system.lock()));
}

#endif