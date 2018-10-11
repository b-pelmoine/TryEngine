#include "Systems/TestSystem.hpp"
#include "TryEngine.hpp"

const TEComponentType STest::TypeID = "STest";

STest::~STest()
{
    
}

Json::Value STest::Serialize() const
{
    return Json::Value(Json::objectValue);
}
void STest::Load(Json::Value&& data)
{
    std::cout << "Loading... " << data.size() << ":" << TypeID << "\n";
}
void STest::Initialize()
{
    if(const auto& window = TE.Window().lock())
    {
        if(const auto& inputs = window->Inputs().lock())
        {
            inputs->AddListener("Save", [](const sf::Event& event){ 
                TE.WorldStream().lock()->Save("Resources/worlds/default1.world.json"); 
            });
            inputs->AddListener("Spawn", [this](const sf::Event& event){ 
                auto entities = TE.WorldStream().lock()->GetWorld().lock()->Entities().lock();
                auto entity = entities->Create();
                auto pos = TEComponents<CTest>::AddTo(entity);
                pos.lock()->position = TE.Window().lock()->GetRender()->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                pos.lock()->Initialize();
                TE.Window().lock()->AddDrawable(pos.lock(), TEWindow::Layer::UI);
                m_drawables.push_back(pos.lock());
            });
        }
    }
}

void STest::Update()
{
    
}

void STest::OnDestroy() 
{
    if(const auto& window = TE.Window().lock())
    {
        for(const auto& drawable : m_drawables)
        {
            window->RemoveDrawable(drawable.lock(), TEWindow::Layer::UI);
        }
    }
}