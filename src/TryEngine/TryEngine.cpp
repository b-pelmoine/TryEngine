#include "TryEngine.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <json/json.h>
#include <fstream>

#include <iostream>

TryEngine TE;

TryEngine::TryEngine():
m_windowHandler(nullptr), 
m_moduleHandler(std::make_shared<TEModuleHandler>()), 
m_physics(nullptr), 
m_resources(std::make_shared<TEResourceManager>()), 
m_worldStream(nullptr), 
bUseCustomConfig(false),
m_GC_waitTime(10.0f),
m_GC_lastOccurence(0.0f)
{}

void TryEngine::Launch(int argc, char** argv)
{
    RegisterDefaultCommands();

    //Register all user defined types
    m_definedTypes.RegisterAllComponents();
    m_definedTypes.RegisterAllSystems();
    m_definedTypes.RegisterAllModules(m_moduleHandler);

    ParseArgs(argc, argv);

    //Init modules
    m_moduleHandler->InitializeModules();

    LoadDefaultConfig();

    Execute();
}

void TryEngine::Execute()
{
    while (m_windowHandler->Active()) 
    {
        tickTime.Tick();
        m_windowHandler->HandleEvents();
        m_moduleHandler->UpdateModules();
        if(m_worldStream)
        {
            if(const auto& world = m_worldStream->GetWorld().lock())
            {
                world->Update();
            }
        }
        m_windowHandler->PostUpdate();
        m_windowHandler->Draw();
        
        if(globalTime.Elapsed() - m_GC_lastOccurence > m_GC_waitTime)
        {
            m_GC_lastOccurence = globalTime.Elapsed();
            m_resources->GC();
        }
    }
}

void TryEngine::Load(TEWorldStream&& stream)
{
    m_worldStream = std::make_shared<TEWorldStream>(stream);
}

void TryEngine::ParseArgs(int argc, char** argv)
{
    for(auto i=1; i< argc; ++i)
    {
        std::string rawArg = argv[i];
        const auto endDelimiter = rawArg.find('=');
        std::string commandName = rawArg.substr (1,endDelimiter-1);
        std::string argument = rawArg.substr (endDelimiter + 1, rawArg.length() - endDelimiter);
        auto it = m_commands.find(commandName);
        if(it != m_commands.end()) it->second(argument);
    }
}

void TryEngine::LoadConfigFromFile(const std::string& path)
{
    Json::Value root;
    std::ifstream config_doc(path, std::ifstream::binary);
    if(config_doc.is_open())
    {
        config_doc >> root;
        config_doc.close();
        bUseCustomConfig = true;
        {
            Json::Value windowSettings = root["video-settings"]["window"];
            TEWindowOptions options;
            if(windowSettings)
            {
                options.Load(windowSettings);
            }
            m_windowHandler = std::make_shared<TEWindow>(windowSettings ? std::move(options) : TEWindowOptions());
        }
        {
            Json::Value moduleSettings = root["module-settings"];
            if(moduleSettings)
            {
                m_moduleHandler->LoadModules(std::move(moduleSettings));
            }
        }
    }
    else
    {
        std::cout << "No config file found\n";
        LoadDefaultConfig();
    }
}

void TryEngine::LoadDefaultConfig()
{
    if(!bUseCustomConfig)
    {
        m_windowHandler = std::make_shared<TEWindow>(TEWindowOptions());
    }
}

void TryEngine::RegisterCommand(std::string identifier, TECommand&& command)
{
    m_commands[identifier] = std::move(command);
}

void TryEngine::UnregisterCommand(std::string identifier)
{
    auto it = m_commands.find(identifier);
    if(it != m_commands.end()) m_commands.erase (it);
}

void TryEngine::ExecCommand(std::string identifier, std::string arg)
{
    auto it = m_commands.find(identifier);
    if(it != m_commands.end()) it->second(arg);
}

void TryEngine::RegisterDefaultCommands()
{
    m_commands["world"] = [this](std::string arg) {
        Load(TEWorldStream(std::move(arg)));
    };
    m_commands["config"] = [this](std::string arg) {
        LoadConfigFromFile(arg);
    };
}