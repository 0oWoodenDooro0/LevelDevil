//
// Created by User on 2024/3/1.
//

#include "Character.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"

Character::Character(AudioManager audio_manager) : Util::GameObject(), audio_manager_(std::move(audio_manager)) {
    SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/image/character/idle/man_idle.png"));
    SetZIndex(1);
    animator_.SetAnimationStates(
            {{"Idle",      std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/idle/man_idle.png")},
             {"JumpLeft",  std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/jump_left/man_jump_left.png")},
             {"JumpRight", std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/jump_right/man_jump_right.png")},
             {"RunLeft",   std::make_unique<Util::Animation>(
                     std::vector<std::string>{RESOURCE_DIR"/image/character/run_left/man_run_left1.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left2.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left3.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left4.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left5.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left6.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left7.png",
                                              RESOURCE_DIR"/image/character/run_left/man_run_left8.png"}, true, 40,
                     true, 0)},
             {"RunRight",  std::make_unique<Util::Animation>(
                     std::vector<std::string>{RESOURCE_DIR"/image/character/run_right/man_run_right1.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right2.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right3.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right4.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right5.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right6.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right7.png",
                                              RESOURCE_DIR"/image/character/run_right/man_run_right8.png"}, true, 40,
                     true, 0)}});
}

void Character::SetCheckPoint(glm::vec2 check_point) {
    m_Transform.translation = check_point;
    check_point_ = check_point;
}

void Character::Move(glm::vec2 input_velocity, const std::vector<std::shared_ptr<Sprite>> &walls) {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [this](
            const std::shared_ptr<Core::Drawable> &drawable) { this->SetDrawable(drawable); };
    auto isGrounded = GroundCheck(walls);
    if (!isGrounded) {
        rigidbody_.AddAcceleration({0, gravity_});
        if (is_direction_right_) {
            animator_.UpdateAnimationState("JumpRight", set_drawable_function);
        } else {
            animator_.UpdateAnimationState("JumpLeft", set_drawable_function);
        }
    } else {
        if (input_velocity.x > 0) {
            animator_.UpdateAnimationState("RunRight", set_drawable_function);
        } else if (input_velocity.x < 0) {
            animator_.UpdateAnimationState("RunLeft", set_drawable_function);
        } else {
            animator_.UpdateAnimationState("Idle", set_drawable_function);
        }
    }
    if (input_velocity.x > 0) {
        is_direction_right_ = true;
    } else if (input_velocity.x < 0) {
        is_direction_right_ = false;
    }
    is_run_ = abs(input_velocity.x) > 0;
    if (is_run_ && isGrounded) {
        audio_manager_.Play(AudioManager::SFX::Run);
    }
    if (input_velocity.y > 0 && isGrounded) {
        rigidbody_.AddAcceleration({0, jump_height_});
        audio_manager_.Play(AudioManager::SFX::Jump);
    }
    rigidbody_.SetVelocity(
            {move_speed_ * input_velocity.x * Util::Time::GetDeltaTimeMs() / 1000, rigidbody_.GetVelocity().y});

    std::function<void(glm::vec2)> translate = [this](glm::vec2 position) { m_Transform.translation += position; };
    rigidbody_.Update(GetCollider(), walls, translate);
}

void Character::Enable() {
    SetVisible(true);
    enabled_ = true;
}

void Character::Disable() {
    SetVisible(false);
    enabled_ = false;
}

bool Character::GroundCheck(const std::vector<std::shared_ptr<Sprite>> &others) const {
    return std::any_of(others.begin(), others.end(), [this](const std::shared_ptr<Sprite> &other) {
        return CollisionHandler::CheckCollision(
                Collider({GetCollider().center.x, GetCollider().bottom}, {GetCollider().size.x, 0.1}),
                other->GetCollider());
    });
}

void Character::Revive() {
    Enable();
    SetPosition(check_point_);
    animator_.UpdateAnimationState("Idle", [this](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
    UpdateState(State::Alive);
    rigidbody_.ResetVelocity();
    rigidbody_.ResetAcceleration();
    audio_manager_.Play(AudioManager::SFX::Revive);
    animator_.UpdateAnimationState("Idle", [this](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
}

void Character::Dead() {
    Disable();
    audio_manager_.Play(AudioManager::SFX::Dead);
}

void Character::LevelClear() {
    Disable();
}

void Character::Bounce() {
    rigidbody_.SetVelocity({rigidbody_.GetVelocity().x, 0});
    rigidbody_.SetAcceleration({rigidbody_.GetAcceleration().x, spring_height_});
}

void Character::Warp(glm::vec2 position) {
    rigidbody_.ResetVelocity();
    rigidbody_.ResetAcceleration();
    SetPosition(position);
}

void Character::UpdateState(Character::State state) {
    if (current_state_ == state) return;
    current_state_ = state;
    switch (current_state_) {
        case State::Alive:
            break;
        case State::Dead:
            Dead();
            break;
        case State::LevelClear:
            LevelClear();
            break;
    }
}
