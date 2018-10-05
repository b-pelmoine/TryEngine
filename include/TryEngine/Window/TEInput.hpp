#ifndef TERO_INPUTHANDLER_HPP
#define TERO_INPUTHANDLER_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include <unordered_map>
#include <vector>

using TEInputListener = std::function<void(const sf::Event&)>;
using TEInputListenerEntry = std::pair<TEInputListener, size_t>;

struct TEInputDescriptor
{
    TEInputDescriptor(std::string _identifier, sf::Event::EventType _type, std::function<bool(const sf::Event&)> _predicate):
    identifier(_identifier), type(_type), predicate(_predicate)
    {}
    private:
    std::string identifier;
    sf::Event::EventType type;
    std::function<bool(const sf::Event&)> predicate;
    std::vector<TEInputListenerEntry> listeners;

    void Call(const sf::Event& event) { for(const auto& listener : listeners) listener.first(event); }

    friend class TEInput;
};

class TEInput
{
    public:
    TEInput() : bQueryingClose(false) {}

    void Handle(sf::Event& event);
    
    bool ReceivedCloseEvent() const { return bQueryingClose; }

    void RegisterInputDescriptor(TEInputDescriptor&&);
    size_t AddListener(const std::string& identifier, const TEInputListener& listener);
    void RemoveListener(const std::string& identifier, const size_t& ID);

    private:
    bool bQueryingClose;
    std::unordered_map<std::string, TEInputDescriptor> m_registeredInputs;
};

#endif