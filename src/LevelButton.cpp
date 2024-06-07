//
// Created by User on 2024/3/27.
//


#include "LevelButton.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Character.hpp"
#include "CollisionHandler.hpp"

LevelButton::LevelButton(AudioManager audiomanager) : GameObject(), audiomanager_(std::move(audiomanager)) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonUp.png");
    animator_.SetAnimationStates(
            {{"Up",   std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonUp.png")},
             {"Down", std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonDown.png")}});
}

void LevelButton::Update(const std::shared_ptr<Character> &character) {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [this](
        std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (!GetEnabled())return;
    if (!character->GetEnabled())return;
    if(current_state_ == State::Up)animator_.UpdateAnimationState("Up", set_drawable_function);
    if (CollisionHandler::CheckCollision(character->GetCollider(), GetCollider())) {
        current_state_ = State::Down;
        animator_.UpdateAnimationState("Down", set_drawable_function);
    }
}
