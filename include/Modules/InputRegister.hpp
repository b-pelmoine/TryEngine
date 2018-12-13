#ifndef TERO_INPUTREGISTER_HPP
#define TERO_INPUTREGISTER_HPP

#include <System/TEModule.hpp>
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Keyboard.hpp"

class TEInputDescriptor;
class TELiveInputDescriptor;
enum class LiveInputType : unsigned int;
class TEAxisDescriptor;

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
    TEInputDescriptor       DeserializeInput(Json::Value&& data) const;
    TELiveInputDescriptor   DeserializeLiveInput(Json::Value&& data) const;
    TEAxisDescriptor        DeserializeAxis(Json::Value&& data) const;

    LiveInputType           StringToLiveInputType(const std::string& base) const;
    sf::Event::EventType    StringToEventType(const std::string& base) const;
    sf::Keyboard::Key       StringToKey(const std::string& base) const;
    sf::Mouse::Button       StringToButton(const std::string& base) const;
};

#endif