#include "Systems/TestSystem.hpp"
#include "TryEngine.hpp"

const TEComponentType STest::TypeID = "STest";

STest::~STest()
{
    
}

Json::Value STest::Serialize() const
{
    Json::Value drawables(Json::arrayValue);
    Json::ArrayIndex compCounter = 0;
    for(const auto& drawable : m_drawables)
    {
        drawables[compCounter] = drawable.lock()->Owner().lock()->ID();
        ++compCounter;
    }
    return drawables;
}
void STest::Load(Json::Value&& data, std::shared_ptr<TEEntities> entities)
{
    for(auto& id: data)
    {
        auto entity = entities->Get(id.asUInt64());
        if(auto component = entity.lock()->GetComponent(CTest::TypeID))
        {
            if(auto pos = std::dynamic_pointer_cast<CTest> (component->lock()) )
            {
                TE.Window().lock()->AddDrawable(pos, TEWindow::Layer::UI);
                m_drawables.push_back(pos);
            }
        }
    }
}
void STest::Initialize()
{
    if(const auto& window = TE.Window().lock())
    {
        if(const auto& inputs = window->Inputs().lock())
        {
            saveID = inputs->AddListener("Save", [](const sf::Event& event __attribute__((unused))){
                TE.WorldStream().lock()->Save(); 
                std::cout << TE.WorldStream().lock()->Filename() << " => Saved " << std::endl;
            });
            switchID = inputs->AddListener("Switch", [](const sf::Event& event __attribute__((unused))){
                std::string fileToLoad = (TE.WorldStream().lock()->Filename() != "Resources/worlds/default1.world.json") ?
                "Resources/worlds/default1.world.json" : "Resources/worlds/default.world.json";
                TE.WorldStream().lock()->Load(fileToLoad);
                std::cout << TE.WorldStream().lock()->Filename() << " => Loaded "<< std::endl;
            });
            spawnID = inputs->AddListener("Spawn", [this](const sf::Event& event){ 
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

void STest::OnDestroy(std::weak_ptr<TESystem> self) 
{
    if(const auto& window = TE.Window().lock())
    {
        if(const auto& inputs = window->Inputs().lock())
        {
            inputs->RemoveListener("Save", saveID);
            inputs->RemoveListener("Switch", switchID);
            inputs->RemoveListener("Spawn", spawnID);
        }
    }
    if(const auto& window = TE.Window().lock())
    {
        for(const auto& drawable : m_drawables)
        {
            window->RemoveDrawable(drawable.lock(), TEWindow::Layer::UI);
        }
    }
    DESTROY_SYS(STest, self)
}