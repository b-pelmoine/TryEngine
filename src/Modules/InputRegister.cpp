#include "Modules/InputRegister.hpp"

#include <iostream>
#include <TryEngine.hpp>

InputRegister::InputRegister() {}
InputRegister::~InputRegister() {}

std::string InputRegister::ID() { return "INPUT_REGISTER"; }

void InputRegister::Load(Json::Value&& data)
{
    //load config from data
}
void InputRegister::Initialize()
{
    if(auto inputs = TE.Window().lock()->Inputs().lock())
    {
        inputs->RegisterInputDescriptor(
            TEInputDescriptor("MouseLeftDown", sf::Event::EventType::MouseButtonPressed, [](const sf::Event& event){ 
                return event.mouseButton.button == sf::Mouse::Button::Left; 
                })
            );
        inputs->RegisterInputDescriptor(
            TEInputDescriptor("MouseLeftUp", sf::Event::EventType::MouseButtonReleased, [](const sf::Event& event){ 
                return event.mouseButton.button == sf::Mouse::Button::Left; 
                })
            );
        inputs->RegisterInputDescriptor(
            TEInputDescriptor("MajA", sf::Event::EventType::KeyPressed, [](const sf::Event& event){ 
                return event.key.shift && event.key.code == sf::Keyboard::A; 
                })
            );
    }
}
void InputRegister::Update()
{
}