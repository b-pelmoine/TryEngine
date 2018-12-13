
#include "Systems/CameraController.hpp"
#include "TryEngine.hpp"

const TEComponentType SCameraController::TypeID = "SCameraController";

SCameraController::SCameraController(Json::LargestInt ID): TESystem(ID),
m_view(sf::Vector2f(0.0f,0.0f), sf::Vector2f(1920.0f, 1080.0f))
{
    bCanEverTick = true;
}

SCameraController::~SCameraController()
{}

Json::Value SCameraController::Serialize() const
{
    return Json::nullValue;
}

void SCameraController::Load(Json::Value&& data, std::shared_ptr<TEEntities> entities)
{}

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
    if(m_inputs->IsInputTriggered("Right"))
    {
        m_view.setCenter(m_view.getCenter() + sf::Vector2f(0.1f, 0));
    }
    if(m_inputs->IsInputTriggered("Left"))
    {
        m_view.setCenter(m_view.getCenter() + sf::Vector2f(-0.1f, 0));
    }
    m_view.setCenter(m_view.getCenter() + sf::Vector2f(m_inputs->GetAxis("MoveRight"), m_inputs->GetAxis("MoveUp")));
    m_window->setView(m_view);
}