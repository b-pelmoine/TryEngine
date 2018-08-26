#include "CoreObject/TEComponent.hpp"

TEComponent::TEComponent() {}

TEComponent::TEComponent(std::weak_ptr<TEEntity> e) : m_entity(e) {
}
 
std::weak_ptr<TEEntity> TEComponent::Owner() const { return m_entity; }