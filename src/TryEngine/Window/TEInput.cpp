#include "Window/TEInput.hpp"
#include <SFML/Window/Event.hpp>
#include <assert.h>
#include <math.h>

TEInput::TEInput() : 
bQueryingClose(false),
m_registeredInputs(8),
m_liveInputs(8),
m_mouseDelta(0, 0),
m_pMousePosition(0, 0),
MouseMovedID(0)
{
    auto alwaysTrue = [this](const sf::Event& e __attribute__((unused))) { return true; };
    RegisterInputDescriptor(TEInputDescriptor("MouseMoved", sf::Event::EventType::MouseMoved, alwaysTrue));
    RegisterInputDescriptor(TEInputDescriptor("MouseEntered", sf::Event::EventType::MouseEntered, alwaysTrue));
    RegisterInputDescriptor(TEInputDescriptor("MouseLeft", sf::Event::EventType::MouseLeft, alwaysTrue));

    AddListener("MouseEntered", [this](const sf::Event& e __attribute__((unused))){ 
        m_pMousePosition = sf::Mouse::getPosition();
        MouseMovedID = AddListener("MouseMoved", [this](const sf::Event& e_){ MouseMoved(e_); });
    });
    AddListener("MouseLeft", [this](const sf::Event& e __attribute__((unused))){ 
        m_mouseDelta = sf::Vector2i(0, 0);
        RemoveListener("MouseMoved", MouseMovedID);
    });
}

void TEInput::Handle(sf::Event& event)
{
    for(auto entry : m_registeredInputs)
    {
        TEInputDescriptor& t_descriptor = entry.second;
        if(event.type == t_descriptor.type && t_descriptor.predicate(event))
            t_descriptor.Call(event);
    }
    if(event.type == sf::Event::EventType::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        bQueryingClose = true;
}

void TEInput::PostUpdate()
{
    m_mouseDelta = sf::Vector2i(0, 0);
}

void TEInput::RegisterInputDescriptor(TEInputDescriptor&& descriptor)
{
    m_registeredInputs.insert(std::make_pair(descriptor.identifier, descriptor));
}

void TEInput::RegisterLiveInput(TELiveInputDescriptor&& descriptor)
{
    m_liveInputs.insert(std::make_pair(descriptor.identifier, descriptor));
}

void TEInput::RegisterAxis(TEAxisDescriptor&& descriptor)
{
    m_Axis.insert(std::make_pair(descriptor.identifier, descriptor));
}

size_t TEInput::AddListener(const std::string& identifier, const TEInputListener& listener)
{
    auto it = m_registeredInputs.find(identifier);
    assert(it != m_registeredInputs.end());
    auto& listeners = it->second.listeners;
    size_t ID = listeners.empty() ? 0 : listeners.back().second + 1;
    TEInputListenerEntry entry = std::make_pair(listener , ID);
    listeners.push_back(entry);
    return ID;
}

void TEInput::RemoveListener(const std::string& identifier, const size_t& ID)
{
    auto it = m_registeredInputs.find(identifier);
    assert(it != m_registeredInputs.end());
    auto& listeners = it->second.listeners;
    auto element = std :: remove_if (listeners.begin(), listeners.end() , [ID](const TEInputListenerEntry& listener){ return listener.second == ID; });
    listeners.erase(element, listeners.end());
}

bool TEInput::IsInputTriggered(const std::string& identifier)
{
    auto it = m_liveInputs.find(identifier);
    if(it == m_liveInputs.end()) return false;
    return it->second.Call();
}

float TEInput::GetAxis(const std::string& identifier)
{
    auto it = m_Axis.find(identifier);
    if(it == m_Axis.end()) return 0.0f;
    return it->second.Call();
}

sf::Vector2f TEInput::GetNormMouseDelta() const
{
    const float mag = sqrt( (m_mouseDelta.x * m_mouseDelta.x) + (m_mouseDelta.y * m_mouseDelta.y) );
    return (mag != 0) ? sf::Vector2f(static_cast<float>(m_mouseDelta.x)/mag, static_cast<float>(m_mouseDelta.y)/mag) : sf::Vector2f(0.0f, 0.0f);
}

void TEInput::MouseMoved(const sf::Event& event)
{
    sf::Vector2i newPosition(event.mouseMove.x, event.mouseMove.y);
    m_mouseDelta = newPosition - m_pMousePosition;
    m_pMousePosition = newPosition;
}