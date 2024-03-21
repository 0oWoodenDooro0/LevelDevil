//
// Created by User on 2024/3/21.
//

#include "Bounce.hpp"

#include <utility>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Animator.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"
#include "Character.hpp"

Bounce::Bounce() {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/bounceDown.png");
    animator_.SetAnimationStates(
        { {"Down",       std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/bounceDown.png")},
         {"Up",      std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/bounceUp.png")} 
        });
}

void Bounce::Update(const std::shared_ptr<Character>& character_) {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
        std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (current_state_ == State::Up) {
        timer_ -= Util::Time::GetDeltaTime();
        if (timer_ <= 0) {
            animator_.UpdateAnimationState("Down", set_drawable_function);
            current_state_ = State::Down;
        }
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        animator_.UpdateAnimationState("Up", set_drawable_function);
        current_state_ = State::Up;
        character_->Bouns();
        timer_ = 0.5;
    }
        
        
}

void Bounce::Enable() {
    SetVisible(true);
}

void Bounce::Disable() {
    SetVisible(false);
}
