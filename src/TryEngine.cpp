#include "TryEngine.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <json/json.h>
#include <fstream>

#include <iostream>


TryEngine::TryEngine(int argc, char** argv) :
m_windowHandler(nullptr), m_moduleHandler(std::make_shared<TEModuleHandler>())
{
    ParseArgs(argc, argv);

    //Register all modules
    m_moduleHandler->RegisterModule(DiscordHandler::ID(), DiscordHandler::Create());

    //Load configuration file
    LoadConfigFromDisk();

    //Init modules
    m_moduleHandler->InitializeModules();
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

void TryEngine::ParseArgs(int argc, char** argv)
{
    for(auto i=1; i< argc; ++i)
    {
        std::cout << argv[i] << "\n";
    }

    std::cout << std::flush;
}

void TryEngine::LoadConfigFromDisk()
{
    Json::Value root;
    std::ifstream config_doc("Resources/config.json", std::ifstream::binary);
    if(config_doc.is_open())
    {
        config_doc >> root;
        Json::Value windowSettings = root["video-settings"]["window"];
        if(windowSettings)
        {
            TEWindowOptions options;
            if(windowSettings.get("fullscreen", false).asBool())
            {
                options.fullScreenMode = sf::VideoMode::getFullscreenModes()[0];
                options.style = sf::Style::Fullscreen;
            }
            else
            {
                options.size = sf::Vector2u(windowSettings.get("width", 1080).asInt(), windowSettings.get("height", 1080).asInt());
                options.CenterPosition();
            }
            options.settings = sf::ContextSettings();
            m_windowHandler = std::make_shared<TEWindow>(std::move(options));
        }
        Json::Value moduleSettings = root["module-settings"];
        if(moduleSettings)
        {
            m_moduleHandler->LoadModules(std::move(moduleSettings));
        }
        config_doc.close();
    }
    else
    {
        std::cout << "No config file found\n";

        m_windowHandler = std::make_shared<TEWindow>(TEWindowOptions());
    }
}