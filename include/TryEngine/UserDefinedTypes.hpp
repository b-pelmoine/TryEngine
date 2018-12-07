#ifndef TERO_USERDEFINEDTYPES_HPP
#define TERO_USERDEFINEDTYPES_HPP

#include "json/json.h"

/*include all classes defining components here*/
#include "Components/TestComponent.hpp"

/*include all classes defining systems here*/
#include "Systems/TestSystem.hpp"

/*include all classes defining modules here*/
#include "Modules/DiscordHandler.hpp"
#include "Modules/InputRegister.hpp"

#define SERIALIZE(system) std::for_each(TESystems<system>::m_systems.begin(), TESystems<system>::m_systems.end(), serialize); systems[typeCounter]["TypeId"] = system::TypeID; ++typeCounter;
#define REGISTER_COMPONENT(component)   TEComponent::registeredComponents[component::TypeID] = [](std::weak_ptr<TEEntity> entity) { return TEComponents<component>::AddTo(entity); }; \
                                        TECOMPONENT_REGISTER_DESTROYER(component)
#define REGISTER_SYSTEM(system) TESystem::registeredSystems[system::TypeID] = [](TESystemID id) { return TESystems<system>::Create(id); }; \
                                        TESYSTEM_REGISTER_DESTROYER(system)
#define REGISTER_MODULE(module) handler->RegisterModule(module::ID(), module::Create())

struct UserDefinedTypes
{
    void RegisterAllComponents() const
    {
        REGISTER_COMPONENT(CTest);
    }

    void RegisterAllSystems() const
    {
        REGISTER_SYSTEM(STest);
    }

    void RegisterAllModules(std::shared_ptr<TEModuleHandler> handler) const
    {
        REGISTER_MODULE(DiscordHandler);
        REGISTER_MODULE(InputRegister);
    }

    Json::Value SerializeAllSystems() const
    {
        Json::Value systems(Json::arrayValue);
        Json::ArrayIndex sysCounter = 0;
        Json::ArrayIndex typeCounter = 0;
        auto serialize = [&](auto& s){ 
            systems[typeCounter]["systems"][sysCounter]["data"] = s->Serialize();
            systems[typeCounter]["systems"][sysCounter]["system-id"] = s->ID();
            ++sysCounter;
        };
        {
            SERIALIZE(STest)
        }
        return systems;
    }
};

#endif