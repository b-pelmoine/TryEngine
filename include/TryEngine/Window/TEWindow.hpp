#ifndef TERO_WINDOWHANDLER_HPP
#define TERO_WINDOWHANDLER_HPP

#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "TEInput.hpp"
#include <json/json.h>

struct TEWindowOptions
{
    sf::Vector2u size;
    sf::Vector2i position;
    sf::ContextSettings settings;
    uint32_t style;
    std::string name;
    sf::VideoMode fullScreenMode; // only applies if style is set to Fullscreen

    TEWindowOptions() :
    size(sf::Vector2u(800, 600)),
    position(sf::Vector2i(0, 0)),
    settings(sf::ContextSettings()),
    style(sf::Style::Default),
    name("TryEngine")
    {}

    void CenterPosition() { 
        position = sf::Vector2i(
            sf::VideoMode::getDesktopMode().width/2 - size.x/2,
            sf::VideoMode::getDesktopMode().height/2 - size.y/2
        );
    }

    void Load(Json::Value options)
    {
        if(options.get("fullscreen", false).asBool())
        {
            fullScreenMode = sf::VideoMode::getFullscreenModes()[0];
            style = sf::Style::Fullscreen;
        }
        else
        {
            size = sf::Vector2u(options.get("width", 1920).asInt(), options.get("height", 1080).asInt());
            CenterPosition();
        }
        settings = sf::ContextSettings();
    }
};

class TEWindow
{
    public:
    enum class Layer { BG, MID, FG, UI, _Count };
    TEWindow(TEWindowOptions&& options);
    TEWindow(std::shared_ptr<sf::RenderWindow> window);

    void HandleEvents();
    void PostUpdate();
    void Draw();
    void Display();

    void Config(const TEWindowOptions& options);
    const TEWindowOptions& GetConfig() const { return m_options; }
    const std::shared_ptr<sf::RenderWindow>& GetRender() const { return m_window; }
    bool Active() const { return m_active; }
    std::weak_ptr<TEInput> Inputs() const { return m_input; }

    void AddDrawable(std::shared_ptr<sf::Drawable> drawable, const Layer& layer = Layer::MID);
    void RemoveDrawable(std::shared_ptr<sf::Drawable> drawable, const Layer& layer = Layer::MID);

    private:
    void SaveCurrentConfig();
    void ApplyConfig();

    TEWindowOptions m_options;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::shared_ptr<TEInput> m_input;
    
    bool m_active;

    std::map<Layer, std::vector<std::weak_ptr<sf::Drawable>>> m_drawables;
};

#endif