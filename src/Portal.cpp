//
// Created by User on 2024/5/10.
//

#include "Portal.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Animator.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"

Portal::Portal(std::vector<std::string> image_pahts, glm::vec2 position, AudioManager audio_manager)
        : audio_manager_(std::move(audio_manager)), goal_(position) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(image_pahts[0]);
    animator_.SetAnimationStates({{"s1", std::make_unique<Util::Image>(image_pahts[0])},
                                  {"s2", std::make_unique<Util::Image>(image_pahts[1])}});
}

void Portal::Update(const std::shared_ptr<Character> &character_) {
    if (!enable_)return;
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [this](
            std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    draw_timer_ -= Util::Time::GetDeltaTimeMs();
    if (current_state_ == State::State1 && draw_timer_ <= 0) {
        animator_.UpdateAnimationState("s2", set_drawable_function);
        current_state_ = State::State2;
        draw_timer_ = 100;
    } else if (current_state_ == State::State2 && draw_timer_ <= 0) {
        animator_.UpdateAnimationState("s1", set_drawable_function);
        current_state_ = State::State1;
        draw_timer_ = 300;
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        animator_.UpdateAnimationState("Up", set_drawable_function);
        audio_manager_.Play(AudioManager::SFX::Door);
        character_->Warp(goal_);
    }
}

void Portal::Enable() {
    SetVisible(true);
    enable_ = true;
}

void Portal::Disable() {
    SetVisible(false);
    enable_ = false;
}
