#ifndef TERO_MODULE_HPP
#define TERO_MODULE_HPP

#include <vector>
#include <map>
#include <json/json.h>
#include <memory>
#include <string>

class TEModule
{
    public:
    TEModule(): bUpdatable(false) {};
    virtual ~TEModule() {};
    virtual void Initialize() =0;
    virtual void Load(Json::Value&& data) =0;
    virtual void Update() {};
    protected:
    bool bUpdatable;

    friend class TEModuleHandler;
};

class TEModuleHandler
{
    public:
    TEModuleHandler();

    void RegisterModule(const std::string& ID, TEModule* module);
    void LoadModules(Json::Value&& SerializedModules);

    void UpdateModules();
    void InitializeModules();

    std::shared_ptr<TEModule> GetModule(std::string ID) const;

    private:
    std::vector<std::shared_ptr<TEModule>> m_activeModules;
    std::vector<std::shared_ptr<TEModule>> m_updatableModules;
    std::map<std::string, std::shared_ptr<TEModule>> m_registeredModules;
};

#endif