//
// Created by User on 2024/5/10.
//

#include "Portal.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"
#include "Movable.hpp"

Portal::Portal(std::vector<std::string> image_pahts, AudioManager audio_manager, glm::vec2 position)
        : audio_manager_(std::move(audio_manager)), goal_(position) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(image_pahts[0]);
    animator_.SetAnimationStates({{"s1", std::make_unique<Util::Image>(image_pahts[0])},
                                  {"s2", std::make_unique<Util::Image>(image_pahts[1])}});
}

void Portal::Update(const std::shared_ptr<Character> &character_) {
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
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider()) && GetEnabled() && character_->GetCurrentState() == Character::State::Alive) {
        audio_manager_.Play(AudioManager::SFX::Door);
        iscollide_ = true;
        character_->UpdateState(Character::State::Portal);
    }
    if (iscollide_ && character_->GetCurrentState() == Character::State::Warp) {
        Movable::Move(character_, goal_, speed_);
        speed_ += 100;
        if (character_->GetPosition() == goal_) {
            character_->UpdateState(Character::State::Appear);
            iscollide_ = false;
            speed_ = 50;
        }
    }
}

void Portal::Enable() {
    SetVisible(true);
    SetEnabled(true);
    iscollide_ = false;
    current_state_ = State::State1;
    draw_timer_ = 300;
}