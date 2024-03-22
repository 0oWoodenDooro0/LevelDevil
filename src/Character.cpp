//
// Created by User on 2024/3/1.
//

#include "Character.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"

Character::Character(AudioManager audioManager) : Util::GameObject(), audioManager_(std::move(audioManager)) {
    SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/image/character/idle/man_idle.png"));
    SetZIndex(1);
    animator_.SetAnimationStates(
            {{"Idle",      std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/idle/man_idle.png")},
             {"JumpLeft",  std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/jump_left/man_jump_left.png")},
             {"JumpRight", std::make_unique<Util::Image>(RESOURCE_DIR"/image/character/jump_right/man_jump_right.png")},
             {"RunLeft",   std::make_unique<Util::Animation>(
                     std::vector<std::string>{(RESOURCE_DIR"/image/character/run_left/man_run_left1.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left2.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left3.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left4.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left5.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left6.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left7.png"),
                                              (RESOURCE_DIR"/image/character/run_left/man_run_left8.png")}, true, 40,
                     true, 0)},
             {"RunRight",  std::make_unique<Util::Animation>(
                     std::vector<std::string>{(RESOURCE_DIR"/image/character/run_right/man_run_right1.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right2.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right3.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right4.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right5.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right6.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right7.png"),
                                              (RESOURCE_DIR"/image/character/run_right/man_run_right8.png")}, true, 40,
                     true, 0)}});
}

void Character::SetCheckPoint(glm::vec2 check_point) {
    m_Transform.translation = check_point;
    check_point_ = check_point;
}

void Character::Update(const std::vector<std::shared_ptr<Sprite>> &walls) {
    if (!enabled_) {
        if (!level_clear_ && (isForwardPressed() || isBackwardPressed() || isJumpPressed())) {
            Revive();
        }
        return;
    }

    glm::vec2 input_velocity = {0, 0};
    if (isForwardPressed()) {
        input_velocity.x = 1;
    } else if (isBackwardPressed()) {
        input_velocity.x = -1;
    }
    if (isJumpPressed()) {
        input_velocity.y = 1;
    }
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
            const std::shared_ptr<Core::Drawable> &drawable) { this->SetDrawable(drawable); };
    auto isGrounded = GroundCheck(walls);
    if (!isGrounded) {
        rigidbody_.SetAcceleration({rigidbody_.GetAcceleration().x, gravity_});
        if (is_direction_right_) {
            animator_.UpdateAnimationState("JumpRight", set_drawable_function);
        } else {
            animator_.UpdateAnimationState("JumpLeft", set_drawable_function);
        }
    } else {
        rigidbody_.ResetVelocity();
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
        audioManager_.Play(AudioManager::SFX::Run);
    }
    if (input_velocity.y > 0 && isGrounded) {
        rigidbody_.SetAcceleration({rigidbody_.GetAcceleration().x, jump_height_});
        audioManager_.Play(AudioManager::SFX::Jump);
    }
    rigidbody_.SetVelocity(
            {float(move_speed_ * input_velocity.x * Util::Time::GetDeltaTime()), rigidbody_.GetVelocity().y});

    std::function<void(glm::vec2)> translate = [&](glm::vec2 position) { m_Transform.translation += position; };
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
    for (const auto &other: others) {
        if (CollisionHandler::CheckCollision(
                Collider({GetCollider().center.x, GetCollider().bottom}, {GetCollider().size.x, 0.1}),
                other->GetCollider())) {
            return true;
        }
    }
    return false;
}

bool Character::isForwardPressed() {
    return Util::Input::IsKeyPressed(Util::Keycode::D) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT);
}

bool Character::isBackwardPressed() {
    return Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::LEFT);
}

bool Character::isJumpPressed() {
    return Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
           Util::Input::IsKeyDown(Util::Keycode::UP);
}

void Character::Revive() {
    Enable();
}

void Character::Dead() {
    Disable();
    rigidbody_.ResetVelocity();
    rigidbody_.ResetAcceleration();
    animator_.UpdateAnimationState("Idle", [&](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
    SetPosition(check_point_);
}

void Character::LevelClear() {
    Disable();
    level_clear_ = true;
}

void Character::Bounce() {
    rigidbody_.SetAcceleration({rigidbody_.GetAcceleration().x, spring_height_});
}
