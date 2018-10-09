#ifndef TERO_INPUTREGISTER_HPP
#define TERO_INPUTREGISTER_HPP

#include <System/TEModule.hpp>
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Keyboard.hpp"

class TEInputDescriptor;

class InputRegister : public TEModule
{
    public:
    void Load(Json::Value&& data) override;
    void Initialize() override;
    void Update() override;

    InputRegister();
    ~InputRegister();

    static std::string ID();
    static InputRegister* Create() { return new InputRegister; }
    
    private:
    TEInputDescriptor Deserialize(Json::Value&& data) const;
    sf::Event::EventType    StringToEventType(const std::string& base) const;
    sf::Keyboard::Key       StringToKey(const std::string& base) const;
    sf::Mouse::Button       StringToButton(const std::string& base) const;
};

#endif