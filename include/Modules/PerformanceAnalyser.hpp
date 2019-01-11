#ifndef TERO_PERFORMANCE_ANALYSER_HPP
#define TERO_PERFORMANCE_ANALYSER_HPP

#include <System/TEModule.hpp>
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include <stack>

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
    sf::Font m_font;
    sf::Text m_fps;
    float m_averageFPS;
    float m_lastFPSUpdate;
    float m_FPSRefreshRate;
    unsigned int m_stackSize;
    std::stack<float> m_previousFPS;
};

#endif