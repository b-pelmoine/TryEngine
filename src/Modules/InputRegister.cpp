#include "Modules/InputRegister.hpp"

#include <iostream>
#include <TryEngine.hpp>

void TEST(const sf::Event& event)
{
    std::cout << "Hello" << std::endl;
}

InputRegister::InputRegister() {}
InputRegister::~InputRegister() {}

std::string InputRegister::ID() { return "INPUT_REGISTER"; }

void InputRegister::Load(Json::Value&& data)
{
    Json::Value root;
    std::ifstream input_doc(data["inputs-file"].asString(), std::ifstream::binary);
    if(input_doc.is_open())
    {
        input_doc >> root;
        if(auto inputs = TE.Window().lock()->Inputs().lock())
        {
            const size_t inputCount = root.size();
            for ( Json::Value::ArrayIndex index = 0; index < inputCount; ++index )
            {
                inputs->RegisterInputDescriptor(
                    Deserialize(std::move(root[index]))
                );
            }
        }
    }
}

TEInputDescriptor InputRegister::Deserialize(Json::Value&& data) const
{
    std::vector<std::function<bool()>> modifiers;

    if(!data.get("modifiers", Json::nullValue).isNull())
    {
        const auto isShift = [](){ return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);  };
        const auto isCtrl = [](){ return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl); };
        const auto isAlt = [](){ return sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt); };

        bool shift = data["modifiers"]["shift"].asBool();
        bool ctrl = data["modifiers"]["ctrl"].asBool();
        bool alt = data["modifiers"]["alt"].asBool();

        if(shift) modifiers.push_back(isShift);
        if(ctrl) modifiers.push_back(isCtrl);
        if(alt) modifiers.push_back(isAlt);
    }
    
    sf::Event::EventType type = StringToEventType(data["event-type"].asString());
    std::function<bool(const sf::Event&)> test;
    Json::Value key = data["key-tested"];
    if(key != Json::nullValue)
    {
        using T_ = sf::Event::EventType;

        switch(type)
        {
            case T_::MouseButtonPressed: 
            case T_::MouseButtonReleased: 
            {
                sf::Mouse::Button mouseButton = StringToButton(key.asString());
                test = [mouseButton](const sf::Event& event){ return event.mouseButton.button == mouseButton; };
            }break;
            case T_::KeyPressed:
            case T_::KeyReleased:
            {
                sf::Keyboard::Key keyID = StringToKey(key.asString());
                test = [keyID](const sf::Event& event){ return event.key.code == keyID; };
            }break;
            case T_::JoystickButtonPressed:
            case T_::JoystickButtonReleased:
            {
                unsigned int buttonID = key.asUInt();
                test = [buttonID](const sf::Event& event){ return event.joystickButton.button == buttonID;};
            }break;
            default: break;
        }
    }

    auto predicate = [test, modifiers](const sf::Event& event){ 
        for(const auto& modifier : modifiers){
                if(!modifier()) return false;
            }
        if(test) return test(event);
        else 
            return true;
    };
    
    return TEInputDescriptor(data["ID"].asString(), type, predicate);
}

void InputRegister::Initialize()
{   
    if(auto inputs = TE.Window().lock()->Inputs().lock())
    {
        inputs->AddListener("ShiftMouseLeftDown", TEST);
    }
}

void InputRegister::Update()
{
}

sf::Event::EventType InputRegister::StringToEventType(const std::string& base) const
{
    const std::map<std::string,sf::Event::EventType> EventTypes {
        { "Closed", sf::Event::EventType::Closed },
        { "Resized", sf::Event::EventType::Resized },
        { "LostFocus", sf::Event::EventType::LostFocus },
        { "GainedFocus", sf::Event::EventType::GainedFocus },
        { "TextEntered", sf::Event::EventType::TextEntered },
        { "KeyPressed", sf::Event::EventType::KeyPressed },
        { "KeyReleased", sf::Event::EventType::KeyReleased },
        { "MouseWheelMoved", sf::Event::EventType::MouseWheelMoved },
        { "MouseWheelScrolled", sf::Event::EventType::MouseWheelScrolled },
        { "MouseButtonPressed", sf::Event::EventType::MouseButtonPressed },
        { "MouseButtonReleased", sf::Event::EventType::MouseButtonReleased },
        { "MouseMoved", sf::Event::EventType::MouseMoved },
        { "MouseEntered", sf::Event::EventType::MouseEntered },
        { "MouseLeft", sf::Event::EventType::MouseLeft },
        { "JoystickButtonPressed", sf::Event::EventType::JoystickButtonPressed },
        { "JoystickButtonReleased", sf::Event::EventType::JoystickButtonReleased },
        { "JoystickMoved", sf::Event::EventType::JoystickMoved },
        { "JoystickConnected", sf::Event::EventType::JoystickConnected },
        { "JoystickDisconnected", sf::Event::EventType::JoystickDisconnected },
        { "TouchBegan", sf::Event::EventType::TouchBegan },
        { "TouchMoved", sf::Event::EventType::TouchMoved },
        { "TouchEnded", sf::Event::EventType::TouchEnded },
        { "SensorChanged", sf::Event::EventType::SensorChanged }
    };
    auto it = EventTypes.find(base);
    return it == EventTypes.end() ? sf::Event::Count : it->second;
}

sf::Keyboard::Key InputRegister::StringToKey(const std::string& base) const
{
    using K_ = sf::Keyboard::Key;
    const std::map<std::string, K_ > KeyTypes {
        { "A", K_::A},
        { "B", K_::B},
        { "C", K_::C},
        { "D", K_::D},
        { "E", K_::E},
        { "F", K_::F},
        { "G", K_::G},
        { "H", K_::H},
        { "I", K_::I},
        { "J", K_::J},
        { "K", K_::K},
        { "L", K_::L},
        { "M", K_::M},
        { "N", K_::N},
        { "O", K_::O},
        { "P", K_::P},
        { "Q", K_::Q},
        { "R", K_::R},
        { "S", K_::S},
        { "T", K_::T},
        { "U", K_::U},
        { "V", K_::V},
        { "W", K_::W},
        { "X", K_::X},
        { "Y", K_::Y},
        { "Z", K_::Z},
        { "Num0", K_::Num0},
        { "Num1", K_::Num1},
        { "Num2", K_::Num2},
        { "Num3", K_::Num3},
        { "Num4", K_::Num4},
        { "Num5", K_::Num5},
        { "Num6", K_::Num6},
        { "Num7", K_::Num7},
        { "Num8", K_::Num8},
        { "Num9", K_::Num9},
        { "Escape", K_::Escape},
        { "LControl", K_::LControl},
        { "LShift", K_::LShift},
        { "LAlt", K_::LAlt},
        { "LSystem", K_::LSystem},
        { "RControl", K_::RControl},
        { "RShift", K_::RShift},
        { "RAlt", K_::RAlt},
        { "RSystem", K_::RSystem},
        { "Menu", K_::Menu},
        { "LBracket", K_::LBracket},
        { "RBracket", K_::RBracket},
        { "Semicolon", K_::Semicolon},
        { "Comma", K_::Comma},
        { "Period", K_::Period},
        { "Quote", K_::Quote},
        { "Slash", K_::Slash},
        { "Backslash", K_::Backslash},
        { "Tilde", K_::Tilde},
        { "Equal", K_::Equal},
        { "Hyphen", K_::Hyphen},
        { "Space", K_::Space},
        { "Enter", K_::Enter},
        { "Backspace", K_::Backspace},
        { "Tab", K_::Tab},
        { "PageUp", K_::PageUp},
        { "PageDown", K_::PageDown},
        { "End", K_::End},
        { "Home", K_::Home},
        { "Insert", K_::Insert},
        { "Delete", K_::Delete},
        { "Add", K_::Add},
        { "Subtract", K_::Subtract},
        { "Multiply", K_::Multiply},
        { "Divide", K_::Divide},
        { "Left", K_::Left},
        { "Right", K_::Right},
        { "Up", K_::Up},
        { "Down", K_::Down},
        { "Numpad0", K_::Numpad0},
        { "Numpad1", K_::Numpad1},
        { "Numpad2", K_::Numpad2},
        { "Numpad3", K_::Numpad3},
        { "Numpad4", K_::Numpad4},
        { "Numpad5", K_::Numpad5},
        { "Numpad6", K_::Numpad6},
        { "Numpad7", K_::Numpad7},
        { "Numpad8", K_::Numpad8},
        { "Numpad9", K_::Numpad9},
        { "F1", K_::F1},
        { "F2", K_::F2},
        { "F3", K_::F3},
        { "F4", K_::F4},
        { "F5", K_::F5},
        { "F6", K_::F6},
        { "F7", K_::F7},
        { "F8", K_::F8},
        { "F9", K_::F9},
        { "F10", K_::F10},
        { "F11", K_::F11},
        { "F12", K_::F12},
        { "F13", K_::F13},
        { "F14", K_::F14},
        { "F15", K_::F15},
        { "Pause", K_::Pause}
    };
    auto it = KeyTypes.find(base);
    return it == KeyTypes.end() ? sf::Keyboard::Unknown : it->second;
}

sf::Mouse::Button InputRegister::StringToButton(const std::string& base) const
{
    using B_ = sf::Mouse::Button;
    const std::map<std::string, B_> ButtonTypes {
        { "Left", B_::Left },
        { "Middle", B_::Middle },
        { "Right", B_::Right },
        { "XButton1", B_::XButton1 },
        { "XButton2", B_::XButton2 }
    };
    auto it = ButtonTypes.find(base);
    return it == ButtonTypes.end() ? sf::Mouse::Button::ButtonCount : it->second;
}