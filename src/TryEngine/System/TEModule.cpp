#include "TryEngine/System/TEModule.hpp"

#include <iostream>

TEModuleHandler::TEModuleHandler() {}

void TEModuleHandler::RegisterModule(const std::string& ID, TEModule* module)
{
    std::shared_ptr<TEModule> mod;
    mod.reset(module);
    m_registeredModules[ID] = mod;
}

void TEModuleHandler::LoadModules(Json::Value&& serializedModules)
{
    for ( int index = 0; index < serializedModules.size(); ++index )
    {
        const Json::Value module = serializedModules[index];
        if(module)
        {
            std::shared_ptr<TEModule> moduleInstance = GetModule(module["module-id"].asString());
            if(moduleInstance != nullptr)
            {
                if(module.get("module-active", true).asBool())
                    m_activeModules.push_back(moduleInstance);
                Json::Value data = module["module-data"];
                if(data)
                {
                    moduleInstance->Load(std::move(data));
                }
            }
        }
    }
}

void TEModuleHandler::UpdateModules()
{
    for(std::shared_ptr<TEModule> module : m_activeModules)
        module->Update();
}

void TEModuleHandler::InitializeModules()
{
    for(std::shared_ptr<TEModule> module : m_activeModules)
        module->Initialize();
}

std::shared_ptr<TEModule> TEModuleHandler::GetModule(std::string ID) const
{
    const auto& it = m_registeredModules.find(ID);
    return (it != m_registeredModules.end()) ? it->second : nullptr;
}