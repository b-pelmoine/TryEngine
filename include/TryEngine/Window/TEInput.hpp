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

enum class LiveInputType : unsigned int{
    Mouse,
    Keyboard,
    Joystick
};

struct TELiveInputDescriptor
{
    TELiveInputDescriptor(std::string _identifier, std::function<bool(void)> _predicate):
    identifier(_identifier), predicate(_predicate)
    {}
    private:
    std::string identifier;
    std::function<bool(void)> predicate;

    bool Call() { return predicate(); }

    friend class TEInput;
};

struct TEAxisDescriptor
{
    TEAxisDescriptor(std::string _identifier, std::function<float(void)> _getAxis):
    identifier(_identifier), getAxis(_getAxis)
    {}
    private:
    std::string identifier;
    std::function<float(void)> getAxis;

    float Call() { return getAxis(); }

    friend class TEInput;
};

class TEInput
{
    public:
    TEInput();

    void Handle(sf::Event& event);
    void PostUpdate();
    
    bool ReceivedCloseEvent() const { return bQueryingClose; }

    void RegisterInputDescriptor(TEInputDescriptor&&);
    void RegisterLiveInput(TELiveInputDescriptor&&);
    void RegisterAxis(TEAxisDescriptor&&);
    size_t AddListener(const std::string& identifier, const TEInputListener& listener);
    void RemoveListener(const std::string& identifier, const size_t& ID);
    bool IsInputTriggered(const std::string& identifier);
    float GetAxis(const std::string& identifier);
    sf::Vector2i GetMouseDelta() const { return m_mouseDelta; };
    sf::Vector2f GetNormMouseDelta() const;

    private:
    bool bQueryingClose;
    std::unordered_map<std::string, TEInputDescriptor> m_registeredInputs;
    std::unordered_map<std::string, TELiveInputDescriptor> m_liveInputs;
    std::unordered_map<std::string, TEAxisDescriptor> m_Axis;

    sf::Vector2i m_mouseDelta, m_pMousePosition;
    size_t MouseMovedID;
    void MouseMoved(const sf::Event& event);

    friend class TEWindow;
};

#endif