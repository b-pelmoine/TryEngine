#include "Systems/CameraController.hpp"
#include "TryEngine.hpp"

const TEComponentType SCameraController::TypeID = "SCameraController";

SCameraController::SCameraController(Json::LargestInt ID): TESystem(ID),
m_view(sf::Vector2f(0.0f,0.0f), sf::Vector2f(1920.0f, 1080.0f)),
m_zoomOffset(0.1f),
m_currentZoom(1.0f),
zoomID(0), panDownID(0), panUpID(0), movingID(0)
{
}

SCameraController::~SCameraController()
{}

Json::Value SCameraController::Serialize() const
{
    Json::Value sys;
    sys["zoom-offset"] = m_zoomOffset;
    sys["position"]["x"] = m_view.getCenter().x;
    sys["position"]["y"] = m_view.getCenter().y;
    sys["zoom"] = m_currentZoom;
    return sys;
}

void SCameraController::Load(Json::Value&& data, std::shared_ptr<TEEntities> entities __attribute__((unused)))
{
    m_zoomOffset = data["zoom-offset"].asFloat();
    m_currentZoom = data.get("zoom", 1.0f).asFloat();
    m_view.zoom(m_currentZoom);
    auto pos = data["position"];
    if(pos)
    {
        m_view.setCenter(pos.get("x", 0).asFloat(), pos.get("y", 0).asFloat());
    }
}

void SCameraController::Initialize()
{
    m_tex = TE.Window().lock()->GetRenderTexture();
    m_tex->setView(m_view);
    m_inputs = TE.Window().lock()->Inputs().lock();

    if(const auto& window = TE.Window().lock())
    {
        if(const auto& inputs = window->Inputs().lock())
        {
            zoomID = inputs->AddListener("Zoom", [this](const sf::Event& event){
                if(event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
                {
                    float zoomFactor = event.mouseWheelScroll.delta * m_zoomOffset;
                    m_currentZoom *= 1 + zoomFactor;
                    ZoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *m_tex, 1 + zoomFactor);
                }
            });
            TEInputListener panDown = [this, inputs, window](const sf::Event& e_ __attribute__((unused))){ 
                inputs->RemoveListener("PanDown", panDownID);
                m_lastPanPos = m_tex->mapPixelToCoords(sf::Mouse::getPosition());
                movingID = inputs->AddListener("MouseMoved", [this, inputs, window](const sf::Event& e_ __attribute__((unused))){ 
                    auto newPos =  m_tex->mapPixelToCoords(sf::Mouse::getPosition());
                    auto delta = m_lastPanPos - newPos;
                    m_lastPanPos = newPos + delta;
                    m_view.move(delta);
                    m_tex->setView(m_view);
                });
            };
            panDownID = inputs->AddListener("PanDown", panDown);
            panUpID = inputs->AddListener("PanUp", [this, inputs, panDown](const sf::Event& e_ __attribute__((unused))){ 
                inputs->RemoveListener("MouseMoved", movingID);
                panDownID = inputs->AddListener("PanDown", panDown);
            });
        }
    }
}

void SCameraController::ZoomViewAt(sf::Vector2i pixel, sf::RenderTexture& tex, float zoom)
{
	const sf::Vector2f beforeCoord{ tex.mapPixelToCoords(pixel) };
	sf::View view{ tex.getView() };
	view.zoom(zoom);
	tex.setView(view);
	const sf::Vector2f afterCoord{ tex.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	tex.setView(view);
    m_view = tex.getView();
}

void SCameraController::OnDestroy()
{
    if(const auto& window = TE.Window().lock())
    {
        if(const auto& inputs = window->Inputs().lock())
        {
            inputs->RemoveListener("Zoom", zoomID);
            inputs->RemoveListener("PanDown", zoomID);
            inputs->RemoveListener("PanUp", zoomID);
            inputs->RemoveListener("MouseMoved", zoomID);
        }
    }
}