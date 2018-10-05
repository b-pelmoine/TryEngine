#ifndef TERO_INPUTREGISTER_HPP
#define TERO_INPUTREGISTER_HPP

#include <System/TEModule.hpp>

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
};

#endif