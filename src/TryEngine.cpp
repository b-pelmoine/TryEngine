#include "TryEngine.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <json/json.h>
#include <fstream>

#include <iostream>

TryEngine TE;

TryEngine::TryEngine():
m_windowHandler(nullptr), m_moduleHandler(std::make_shared<TEModuleHandler>())
{}

void TryEngine::Launch(int argc, char** argv)
{
    //Register all modules
    m_moduleHandler->RegisterModule(DiscordHandler::ID(), DiscordHandler::Create());

    //Register all user defined types
    m_definedTypes.RegisterAllComponents();
    m_definedTypes.RegisterAllSystems();

    //Load configuration file
    LoadConfigFromDisk();

    //Init modules
    m_moduleHandler->InitializeModules();

    m_commands["world"] = [this](std::string arg) {
        Load(TEWorldStream(std::move(arg)));
    };
    ParseArgs(argc, argv);

    Execute();
}


void TryEngine::Execute()
{
    while (m_windowHandler && m_windowHandler->Active()) 
    {
        m_windowHandler->HandleEvents();
        m_windowHandler->Draw();
        //Update modules
        m_moduleHandler->UpdateModules();
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

void TryEngine::LoadConfigFromDisk()
{
    Json::Value root;
    std::ifstream config_doc("Resources/config.json", std::ifstream::binary);
    if(config_doc.is_open())
    {
        config_doc >> root;
        config_doc.close();
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

        m_windowHandler = std::make_shared<TEWindow>(TEWindowOptions());
    }
}