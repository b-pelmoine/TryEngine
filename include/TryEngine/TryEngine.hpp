#ifndef TERO_TRYENGINE_HPP
#define TERO_TRYENGINE_HPP

#define APP_NAME "TryEngine"

#include <memory>
#include <map>
#include <functional>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Window/TEWindow.hpp"
#include "System/TEModule.hpp"
#include "System/TEPhysics.hpp"
#include "System/TEResourceManager.hpp"
#include "CoreObject/TEWorld.hpp"
#include "CoreObject/TETime.hpp"

#include "UserDefinedTypes.hpp"

class TryEngine
{
    using TECommand = std::function<void(std::string)>;

    std::shared_ptr<TEWindow>           m_windowHandler;
    std::shared_ptr<TEModuleHandler>    m_moduleHandler;
    std::shared_ptr<TEPhysics>          m_physics;
    std::shared_ptr<TEResourceManager>  m_resources;
    std::shared_ptr<TEWorldStream>      m_worldStream;

    UserDefinedTypes                    m_definedTypes;

    std::map<std::string, TECommand>    m_commands;
    bool bUseCustomConfig;
    float m_GC_waitTime;
    float m_GC_lastOccurence;

    public:
    TryEngine();

    void Load(TEWorldStream&& stream);
    void Launch(int argc, char** argv);
    void RegisterCommand(std::string identifier, TECommand&& command);
    void UnregisterCommand(std::string identifier);
    void ExecCommand(std::string identifier, std::string = "");

    std::weak_ptr<TEWindow>           Window() const            { return m_windowHandler; };
    std::weak_ptr<TEModuleHandler>    ModuleHandler() const     { return m_moduleHandler; };
    std::weak_ptr<TEPhysics>          Physics() const           { return m_physics; };
    std::weak_ptr<TEResourceManager>  Resources() const         { return m_resources; };
    std::weak_ptr<TEWorldStream>      WorldStream() const       { return m_worldStream; };
    const UserDefinedTypes&           UDT() const               { return m_definedTypes; };

    TETime globalTime;
    TETime tickTime;

    private:
    void Execute();
    void ParseArgs(int argc, char** argv);
    void LoadConfigFromFile(const std::string& path);
    void LoadDefaultConfig();
    void RegisterDefaultCommands();
};

extern TryEngine TE;

#endif