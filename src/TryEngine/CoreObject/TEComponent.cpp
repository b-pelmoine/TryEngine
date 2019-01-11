#include "CoreObject/TEComponent.hpp"

std::unordered_map<TEComponentType, std::function<std::weak_ptr<TEComponent>(std::weak_ptr<TEEntity>)>> TEComponent::registeredComponents;
std::unordered_map<TEComponentType, std::function<void(std::weak_ptr<TEEntity>)>> TEComponent::componentsDestroyer;

TEComponent::TEComponent() {}
TEComponent::TEComponent(std::weak_ptr<TEEntity> e, bool loadedFromFile) : m_entity(e) {
    bLoadedFromFile = loadedFromFile;
}
 
std::weak_ptr<TEEntity> TEComponent::Owner() const { return m_entity; }