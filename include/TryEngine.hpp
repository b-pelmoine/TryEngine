#ifndef TERO_TRYENGINE_HPP
#define TERO_TRYENGINE_HPP

#define APP_NAME "TryEngine"

#include <Utilities/DiscordHandler.hpp>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Window/TEWindow.hpp"
#include "System/TEModule.hpp"
#include "System/TEPhysics.hpp"

class TryEngine
{
    std::shared_ptr<TEWindow>           m_windowHandler;
    std::shared_ptr<TEModuleHandler>    m_moduleHandler;
    std::shared_ptr<TEPhysics>          m_physics;

    public:
    TryEngine(int argc, char** argv);

    void Execute();

    std::weak_ptr<TEWindow>           Window() const            { return m_windowHandler; };
    std::weak_ptr<TEModuleHandler>    ModuleHandler() const     { return m_moduleHandler; };
    std::weak_ptr<TEPhysics>          Physics() const           { return m_physics; };

    private:
    void ParseArgs(int argc, char** argv);
    void LoadConfigFromDisk();
};

#endif