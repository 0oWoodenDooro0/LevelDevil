//
// Created by User on 2024/3/22.
//

#include "Coin.hpp"

#include <utility>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"
#include "Character.hpp"

Coin::Coin() {
    SetImage(std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/coin.png"));
    SetZIndex(0);
}

void Coin::SetImage(const std::string& image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

void Coin::Update(const std::shared_ptr<Character>& character_) const {
    if (enabled_==false)return;
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider()))
        Disable();
}

void Coin::Enable() {
    enabled_ = true;
    SetVisible(true);
}

void Coin::Disable() {
    enabled_ = false;
    SetVisible(false);
}
