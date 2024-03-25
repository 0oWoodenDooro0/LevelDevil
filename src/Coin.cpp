//
// Created by User on 2024/3/22.
//

#include "Coin.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Coin::Coin(AudioManager audiomanager) : audiomanager_(std::move(audiomanager)) {
    SetImage(RESOURCE_DIR"/image/component/coin.png");
    SetZIndex(0);
}

void Coin::SetImage(const std::string &image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

void Coin::Update(const std::shared_ptr<Character> &character_) {
    if (!enabled_)return;
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        audiomanager_.Play(AudioManager::SFX::Coin);
        Disable();
    }
}

void Coin::Enable() {
    SetVisible(true);
    enabled_ = true;
}

void Coin::Disable() {
    SetVisible(false);
    enabled_ = false;
}
