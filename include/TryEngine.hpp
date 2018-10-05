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
#include "CoreObject/TEWorld.hpp"

#include "UserDefinedTypes.hpp"

class TryEngine
{
    std::shared_ptr<TEWindow>           m_windowHandler;
    std::shared_ptr<TEModuleHandler>    m_moduleHandler;
    std::shared_ptr<TEPhysics>          m_physics;
    std::shared_ptr<TEWorldStream>      m_worldStream;

    UserDefinedTypes                    m_definedTypes;

    std::map<std::string, std::function<void(std::string)>> m_commands;

    public:
    TryEngine();

    void Load(TEWorldStream&& stream);
    void Launch(int argc, char** argv);

    std::weak_ptr<TEWindow>           Window() const            { return m_windowHandler; };
    std::weak_ptr<TEModuleHandler>    ModuleHandler() const     { return m_moduleHandler; };
    std::weak_ptr<TEPhysics>          Physics() const           { return m_physics; };
    std::weak_ptr<TEWorld>            World() const             { return m_worldStream->GetWorld(); };
    const UserDefinedTypes&           UDT() const               { return m_definedTypes; };

    private:
    void Execute();
    void ParseArgs(int argc, char** argv);
    void LoadConfigFromDisk();
};

extern TryEngine TE;

#endif