#include "Modules/PerformanceAnalyser.hpp"

#include <iostream>
#include <TryEngine.hpp>
#include <sstream>

PerformanceAnalyser::PerformanceAnalyser() { bUpdatable = true; }
PerformanceAnalyser::~PerformanceAnalyser() {}

std::string PerformanceAnalyser::ID() { return "PERF_ANALYSER"; }

void PerformanceAnalyser::Load(Json::Value&& data)
{
    m_font.loadFromFile(data["font"].asString());
    m_stackSize = data["fps-memory"].asUInt();
    m_FPSRefreshRate = data["refresh-rate"].asFloat();
    m_graphLength = data["graph-length"].asFloat();
    m_fps.setFont(m_font);
    m_fps.setPosition(5.0f,0.0f);
    m_fps.setCharacterSize(32);
    m_fps.setStyle(sf::Text::Regular);
    
    m_fpsmin.setFont(m_font);
    m_fpsmin.setPosition(50.f,900.f);
    m_fpsmin.setCharacterSize(50);

    m_fpsmax.setFont(m_font);
    m_fpsmax.setPosition(50.f,50.f);
    m_fpsmax.setCharacterSize(50);
}

void PerformanceAnalyser::Initialize()
{
    m_FPSvertices.resize(m_graphLength / m_FPSRefreshRate);
    const size_t _size = m_FPSvertices.size();
    if(_size > 1)
    {
        for(size_t i=0; i<_size; ++i)
        {
            m_FPSvertices[i].position.x = (float)i/(_size-1);
            m_FPSvertices[i].position.y = 0.5f;
            m_FPSvertices[i].color.a = 0.0f;
        }
    }
    {
        sf::VertexArray fps_ui(sf::LineStrip, 5);
        fps_ui[0].position = sf::Vector2f(0.f, 0.f);
        fps_ui[1].position = sf::Vector2f(1.f, 0.f);
        fps_ui[2].position = sf::Vector2f(1.f, 1.f);
        fps_ui[3].position = sf::Vector2f(0.f, 1.f);
        fps_ui[4].position = sf::Vector2f(0.f, 0.f);
        m_FPS_UI = fps_ui;
    }
    m_maxFPS = 90.0f;
    m_minFPS = 0.0f;
    m_lastFPSUpdate = 0.0f;
    if(auto res = TE.Resources().lock())
    {
        res->Register("post", TEResource::Type::SHADER, "Resources/shader/postprocess.frag");
        std::shared_ptr<TEShader> test;
        res->GetShader("post", test);
        test->Configure("Resources/shader/postprocess.frag", sf::Shader::Type::Fragment);
        test->Load();
        test->Get().setUniform("texture", sf::Shader::CurrentTexture);
        TE.Window().lock()->ConfigurePostProcess("post");
    }
}

void PerformanceAnalyser::Update() 
{
    if(m_previousFPS.size() > m_stackSize)
    {
        m_previousFPS.pop();
    }
    m_previousFPS.push(1/TE.tickTime());
    m_averageFPS = m_averageFPS + ((m_previousFPS.top() - m_averageFPS)/m_previousFPS.size());

    if(TE.globalTime.Elapsed() - m_lastFPSUpdate > m_FPSRefreshRate)
    {
        m_lastFPSUpdate = TE.globalTime.Elapsed();
        {
            std::stringstream ss;
            ss << static_cast<unsigned int>(m_averageFPS) << " FPS";
            m_fps.setString(ss.str());
        }
        {
            std::stringstream ss;
            ss << static_cast<unsigned int>(m_minFPS);
            m_fpsmin.setString(ss.str());
        }
        {
            std::stringstream ss;
            ss << static_cast<unsigned int>(m_maxFPS);
            m_fpsmax.setString(ss.str());
        }
        UpdateFPSgraph();
    }
    Draw();
}

void PerformanceAnalyser::Draw()
{
    auto render = TE.Window().lock()->GetRenderWindow();
    sf::View t_view = render->getView();
    render->setView(render->getDefaultView());
    render->draw(m_fps);
    sf::View fpsview({0.5f, 0.5f}, {1.01f, 1.01f});
    fpsview.setViewport(sf::FloatRect(0.70f, 0.05f, 0.25f, 0.25f));
    render->setView(fpsview);
    render->draw(&m_FPSvertices[0], m_currentVertex, sf::LineStrip);
    const auto count = m_FPSvertices.size() - (m_currentVertex);
    render->draw(&m_FPSvertices[m_currentVertex], count, sf::LineStrip);
    render->draw(m_FPS_UI);
    fpsview.setSize({1000.f,1000.f});
    fpsview.setCenter({500.f, 500.f});
    render->setView(fpsview);
    render->draw(m_fpsmin);
    render->draw(m_fpsmax);
    render->setView(t_view);
}

void PerformanceAnalyser::UpdateFPSgraph()
{
    if(m_FPSvertices.empty()) return;

    UpdateMinMaxFPS();
    ++m_currentVertex;
    if(m_currentVertex >= m_FPSvertices.size()) m_currentVertex = 0;
}

void PerformanceAnalyser::UpdateMinMaxFPS()
{
    float _max = m_maxFPS;
    if(m_averageFPS > m_maxFPS) m_maxFPS = m_averageFPS*1.1f;
    for(auto& vert: m_FPSvertices)
    {
        vert.position.y = m_minFPS + ((1-vert.position.y) * (_max - m_minFPS));
        vert.position.y = 1 - (vert.position.y - m_minFPS)/(m_maxFPS - m_minFPS);
        vert.color.a *= 1.0f - (2.0f/m_FPSvertices.size());
    }
    m_FPSvertices[m_currentVertex].position.y = 1 - (m_averageFPS - m_minFPS)/(m_maxFPS - m_minFPS);
    m_FPSvertices[m_currentVertex].color = sf::Color::Green;
}