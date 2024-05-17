//
// Created by User on 2024/3/22.
//

#include "Coin.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Coin::Coin(AudioManager audio_manager, const std::string &image_path) : audio_manager_(std::move(audio_manager)) {
    SetImage(image_path);
    SetZIndex(0);
}

void Coin::SetImage(const std::string &image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

void Coin::Update(const std::shared_ptr<Character> &character_) {
    if (!GetEnabled() || !character_->GetEnabled()) {
        current_state_ = State::Idle;
        return;
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        Disable();
        current_state_ = State::Trigger;
    }
}
