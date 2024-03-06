//
// Created by User on 2024/3/6.
//

#include "Spike.hpp"

#include <utility>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"
#include "Character.hpp"

Spike::Spike(const std::string& image_path){
    SetImage(image_path);
    SetZIndex(10);
}

void Spike::SetImage(const std::string& image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}

void Spike::Translate(const glm::vec2& position) {
    m_Transform.translation += position;
}

void Spike::Update(std::shared_ptr<Character> character_) {
    if (CollisionHandler::CheckCollision(character_->GetPosition(), character_->GetSize(), GetPosition() - glm::vec2(0, 21), GetSize()-glm::vec2(0,42))) {
        character_->dead();
    }
}