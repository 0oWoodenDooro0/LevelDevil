//
// Created by User on 2024/3/8.
//

#include "Door.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"
#include "Animator.hpp"

Door::Door(AudioManager audio_manager, std::vector<std::string> img_paths)
        : GameObject(), audio_manager_(std::move(audio_manager)) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(img_paths[0]);
    animator_.SetAnimationStates(
            {{"Idle",      std::make_unique<Util::Image>(img_paths[0])},
             {"Delay",     std::make_unique<Util::Image>(img_paths[1])},
             {"CloseDoor", std::make_unique<Util::Animation>(
                     std::vector<std::string>{(img_paths[2]),
                                              (img_paths[3]),
                                              (img_paths[4]),
                                              (img_paths[5])}, true, 50,
                     false, 0)}});
}

void Door::Update(const std::shared_ptr<Character> &character_) {
    if (!GetEnabled()) {
        return;
    }
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [this](
            std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (current_state_ == State::CloseDoor) {
        if (timer_ <= 0) {
            audio_manager_.Play(AudioManager::SFX::StageClear);
            Disable();
        } else {
            timer_ -= Util::Time::GetDeltaTimeMs();
        }
        return;
    }
    if (current_state_ == State::Delay) {
        if (timer_ <= 0) {
            animator_.UpdateAnimationState("CloseDoor", set_drawable_function);
            current_state_ = State::CloseDoor;
            timer_ = 250;
        } else {
            timer_ -= Util::Time::GetDeltaTimeMs();
        }
        return;
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider()) && character_->GetCurrentState() == Character::State::Alive) {
        character_->UpdateState(Character::State::LevelClear);
        animator_.UpdateAnimationState("Delay", set_drawable_function);
        current_state_ = State::Delay;
        timer_ = 400;
        audio_manager_.Play(AudioManager::SFX::Door);
    } else {
        animator_.UpdateAnimationState("Idle", set_drawable_function);
        current_state_ = State::Idle;
    }
}
