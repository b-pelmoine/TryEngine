#include "Window/TEInput.hpp"
#include <SFML/Window/Event.hpp>
#include <assert.h>

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

void TEInput::RegisterInputDescriptor(TEInputDescriptor&& descriptor)
{
    m_registeredInputs.insert(std::make_pair(descriptor.identifier, descriptor));
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


