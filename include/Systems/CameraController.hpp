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
    void OnDestroy() override;

private: 
    SCameraController(): TESystem(0) {}

    void ZoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);

    sf::View m_view;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::shared_ptr<TEInput> m_inputs;
    sf::Vector2f m_lastPanPos;
    float m_zoomOffset;
    float m_currentZoom;
    size_t zoomID;
    size_t panDownID, panUpID, movingID;
    friend class UserDefinedTypes;
};

#endif

