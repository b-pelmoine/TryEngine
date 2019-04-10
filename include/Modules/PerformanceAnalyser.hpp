#ifndef TERO_PERFORMANCE_ANALYSER_HPP
#define TERO_PERFORMANCE_ANALYSER_HPP

#include <System/TEModule.hpp>
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include <stack>
#include <vector>

class PerformanceAnalyser : public TEModule
{
    public:
    void Load(Json::Value&& data) override;
    void Initialize() override;
    void Update() override;

    PerformanceAnalyser();
    ~PerformanceAnalyser();

    static std::string ID();
    static PerformanceAnalyser* Create() { return new PerformanceAnalyser; }

    private:
    void UpdateFPSgraph();
    void Draw();

    void UpdateMinMaxFPS();

    sf::Font m_font;
    sf::Text m_fps, m_fpsmin, m_fpsmax;
    float m_averageFPS;
    float m_lastFPSUpdate;
    float m_FPSRefreshRate;
    float m_graphLength;

    unsigned int m_stackSize;
    float m_maxFPS, m_minFPS;
    std::stack<float> m_previousFPS;
    std::vector<sf::Vertex> m_FPSvertices;
    sf::VertexArray m_FPS_UI;
    size_t m_currentVertex;
};

#endif