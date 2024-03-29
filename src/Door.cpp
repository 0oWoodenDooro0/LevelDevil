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

Door::Door(AudioManager audio_manager) : Util::GameObject(), audio_manager_(std::move(audio_manager)) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(RESOURCE_DIR"/image/door/door.png");
    animator_.SetAnimationStates(
            {{"Idle",       std::make_unique<Util::Image>(RESOURCE_DIR"/image/door/door.png")},
             {"Delay",      std::make_unique<Util::Image>(RESOURCE_DIR"/image/door/in_door1.png")},
             {"StageClear", std::make_unique<Util::Animation>(
                     std::vector<std::string>{(RESOURCE_DIR"/image/door/in_door2.png"),
                                              (RESOURCE_DIR"/image/door/in_door3.png"),
                                              (RESOURCE_DIR"/image/door/in_door4.png"),
                                              (RESOURCE_DIR"/image/door/in_door5.png")}, true, 50,
                     false, 0)}});
}

void Door::Update(const std::shared_ptr<Character> &character_) {
    if (!enabled_) {
        return;
    }
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
            std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (current_state_ == State::StageClear) {
        if (timer_ <= 0) {
            audio_manager_.Play(AudioManager::SFX::StageClear);
            Disable();
        } else {
            timer_ -= float(Util::Time::GetDeltaTime());
        }
        return;
    }
    if (current_state_ == State::Delay) {
        if (timer_ <= 0) {
            animator_.UpdateAnimationState("StageClear", set_drawable_function);
            current_state_ = State::StageClear;
            timer_ = 0.25;
            audio_manager_.Play(AudioManager::SFX::Door);
        } else {
            timer_ -= float(Util::Time::GetDeltaTime());
        }
        return;
    }
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        character_->LevelClear();
        animator_.UpdateAnimationState("Delay", set_drawable_function);
        current_state_ = State::Delay;
        timer_ = 0.5;
    } else {
        animator_.UpdateAnimationState("Idle", set_drawable_function);
        current_state_ = State::Idle;
    }
}

void Door::Enable() {
    enabled_ = true;
    SetVisible(true);
}

void Door::Disable() {
    enabled_ = false;
    SetVisible(false);
}
