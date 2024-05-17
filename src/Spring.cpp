//
// Created by User on 2024/3/21.
//

#include "Spring.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Animator.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Spring::Spring(std::vector<std::string> image_pahts, AudioManager audio_manager)
        : audio_manager_(std::move(audio_manager)) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(image_pahts[0]);
    animator_.SetAnimationStates(
            {{"Down", std::make_unique<Util::Image>(image_pahts[0])},
             {"Up",   std::make_unique<Util::Image>(image_pahts[1])}});
}

void Spring::Update(const std::shared_ptr<Character> &character_) {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [this](
            std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    bounce_timer_ -= Util::Time::GetDeltaTimeMs();
    if (current_state_ == State::Up) {
        draw_timer_ -= Util::Time::GetDeltaTimeMs();
        if (draw_timer_ <= 0) {
            animator_.UpdateAnimationState("Down", set_drawable_function);
            current_state_ = State::Down;
        }
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider()) && bounce_timer_ <= 0) {
        animator_.UpdateAnimationState("Up", set_drawable_function);
        audio_manager_.Play(AudioManager::SFX::Bounce);
        current_state_ = State::Up;
        character_->Bounce();
        bounce_timer_ = 200;
        draw_timer_ = 500;
    }
}

void Spring::Reset() {
    animator_.UpdateAnimationState("Down", [this](std::shared_ptr<Core::Drawable> drawable) {
        m_Drawable = std::move(drawable);
    });
}
