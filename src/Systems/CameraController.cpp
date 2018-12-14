
#include "Systems/CameraController.hpp"
#include "TryEngine.hpp"

const TEComponentType SCameraController::TypeID = "SCameraController";

SCameraController::SCameraController(Json::LargestInt ID): TESystem(ID),
m_view(sf::Vector2f(0.0f,0.0f), sf::Vector2f(1920.0f, 1080.0f)), m_BaseSpeed(0.0f), m_FastSpeed(0.0f)
{
    bCanEverTick = true;
}

SCameraController::~SCameraController()
{}

Json::Value SCameraController::Serialize() const
{
    Json::Value sys;
    sys["base-speed"] = m_BaseSpeed;
    sys["fast-speed"] = m_FastSpeed;
    return sys;
}

void SCameraController::Load(Json::Value&& data, std::shared_ptr<TEEntities> entities)
{
    m_BaseSpeed = data["base-speed"].asFloat();
    m_FastSpeed = data["fast-speed"].asFloat();
}

void SCameraController::Initialize()
{
    m_window = TE.Window().lock()->GetRender();
    m_window->setView(m_view);
    m_inputs = TE.Window().lock()->Inputs().lock();
}

void SCameraController::OnDestroy()
{}

void SCameraController::Tick()
{
    float elapsed = TE.tickTime.Get();
    float speed = (m_inputs->IsInputTriggered("Shift")) ? m_FastSpeed : m_BaseSpeed;
    m_view.setCenter(m_view.getCenter() + sf::Vector2f(m_inputs->GetAxis("MoveRight")*elapsed*speed, m_inputs->GetAxis("MoveUp")*elapsed*speed));
    m_window->setView(m_view);
}