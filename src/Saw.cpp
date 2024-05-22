//
// Created by User on 2024/5/22.
//

#include "Saw.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Saw::Saw(const std::string& image_path, Position position_inbox, AudioManager audio_manager)
    : position_inbox_(position_inbox), audio_manager_(std::move(audio_manager)) {
    SetImage(image_path);
    SetZIndex(0);
}

void Saw::SetImage(const std::string& image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

Collider Saw::GetCollider() const {
    return { GetPosition(), GetSize() };
}

void Saw::Update(const std::shared_ptr<Character>& character) {
    if (!GetEnabled())return;
    if (!character->GetEnabled())return;
    if (CollisionHandler::CheckCollision(character->GetCollider(), GetCollider())) {
        character->UpdateState(Character::State::Dead);
        audio_manager_.Play(AudioManager::SFX::Saw);
    }
}

void Saw::Enable() {
    SetVisible(true);
    SetEnabled(true);
    audio_manager_.Play(AudioManager::SFX::Trap);
}

void Saw::Disable() {
    SetVisible(false);
    SetEnabled(false);
    audio_manager_.Play(AudioManager::SFX::Trap);
}