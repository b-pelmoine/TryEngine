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
    m_fps.setFont(m_font);
    m_fps.setPosition(5.0f,0.0f);
    m_fps.setCharacterSize(32);
    m_fps.setStyle(sf::Text::Regular);
}

void PerformanceAnalyser::Initialize()
{
    m_lastFPSUpdate = 0.0f;
}

void PerformanceAnalyser::Update() 
{
    if(m_previousFPS.size() > m_stackSize)
        m_previousFPS.pop();
    m_previousFPS.push(1/TE.tickTime());
    m_averageFPS = m_averageFPS + ((m_previousFPS.top() - m_averageFPS)/m_previousFPS.size());

    if(TE.globalTime.Elapsed() - m_lastFPSUpdate > m_FPSRefreshRate)
    {
        m_lastFPSUpdate = TE.globalTime.Elapsed();
        std::stringstream ss;
        ss << static_cast<unsigned int>(m_averageFPS) << " FPS";
        m_fps.setString(ss.str());
    }
    auto render = TE.Window().lock()->GetRender();
    sf::View t_view = render->getView();
    sf::View view = render->getDefaultView();
    render->setView(view);
    render->draw(m_fps);
    render->setView(t_view);
}