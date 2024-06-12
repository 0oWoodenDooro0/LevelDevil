//
// Created by User on 2024/3/6.
//

#include "Spike.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Spike::Spike(const std::string &image_path, Position position_inbox, AudioManager audio_manager)
        : position_inbox_(position_inbox), audio_manager_(std::move(audio_manager)) {
    SetImage(image_path);
    SetZIndex(0);
}

void Spike::SetImage(const std::string &image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

Collider Spike::GetCollider() const {
    switch (position_inbox_) {
        case Position::Bottom:
            return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
        case Position::Top:
            return {GetPosition() + glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
        case Position::Left:
            return {GetPosition() - glm::vec2(21, 0), GetSize() - glm::vec2(42, 0)};
        case Position::Right:
            return {GetPosition() + glm::vec2(21, 0), GetSize() - glm::vec2(42, 0)};
    }
    assert(false && "Wrong Spike Position");
}

void Spike::Update(const std::shared_ptr<Character> &character) {
    if (!GetEnabled())return;
    if (!character->GetEnabled())return;
    if (CollisionHandler::CheckCollision(character->GetCollider(), GetCollider()) && character->GetCurrentState() != Character::State::Vanish){
        character->UpdateState(Character::State::Dead);
    }
}

void Spike::Enable() {
    SetVisible(true);
    SetEnabled(true);
    audio_manager_.Play(AudioManager::SFX::Trap);
}