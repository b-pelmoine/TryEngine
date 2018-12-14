#ifndef TERO_SYS_CAMERACONTROLLER_HPP
#define TERO_SYS_CAMERACONTROLLER_HPP

#include "CoreMinimal.hpp"
#include "SFML/Graphics/View.hpp"
#include <memory>
#include "Window/TEInput.hpp"

class SCameraController : public TESystem {
public: 
    SCameraController(Json::LargestInt ID);
    virtual ~SCameraController();
    static const TESystemType TypeID;
    virtual TESystemType Type() const noexcept override { return SCameraController::TypeID; }
    Json::Value Serialize() const override;
    void Load(Json::Value&& data, std::shared_ptr<TEEntities> entities) override;
    void Initialize() override;
    void Tick() override;
    void OnDestroy() override;

private: 
    SCameraController(): TESystem(0) {}
    sf::View m_view;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::shared_ptr<TEInput> m_inputs;
    float m_BaseSpeed, m_FastSpeed;
    friend class UserDefinedTypes;
};

#endif

