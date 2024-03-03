//
// Created by User on 2024/3/1.
//

#include "Character.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"

Character::Character() : Util::GameObject(
        std::make_unique<Util::Image>(RESOURCE_DIR"/Image/Character/Idle/man_idle.png"), 1) {}

void Character::Translate(const glm::vec2 &position) {
    m_Transform.translation += position;
}

void Character::SetAnimation(AnimationState animation_state) {
    if (animation_state_ == animation_state) return;
    animation_state_ = animation_state;
    switch (animation_state) {
        case AnimationState::Idle:
            m_Drawable = idle_drawable_;
            break;
        case AnimationState::JumpLeft:
            m_Drawable = jump_left_drawable_;
            break;
        case AnimationState::JumpRight:
            m_Drawable = jump_right_drawable_;
            break;
        case AnimationState::RunLeft:
            m_Drawable = run_left_drawable_;
            break;
        case AnimationState::RunRight:
            m_Drawable = run_right_drawable_;
            break;
    }
}

void Character::Update(const std::vector<std::shared_ptr<Wall>> &walls) {
    glm::vec2 input_velocity = {0, 0};
    if (Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        input_velocity.x = -1;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::D) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        input_velocity.x = 1;
    }
    if (Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
        Util::Input::IsKeyDown(Util::Keycode::UP)) {
        input_velocity.y = 1;
    }
    auto onGrounded = GroundCheck(walls);
    if (!onGrounded) {
        acceleration_.y = gravity_;
        if (direction_right_) {
            SetAnimation(AnimationState::JumpRight);
        } else {
            SetAnimation(AnimationState::JumpLeft);
        }
    } else {
        velocity_ = {0, 0};
        if (input_velocity.x > 0) {
            SetAnimation(AnimationState::RunRight);
        } else if (input_velocity.x < 0) {
            SetAnimation(AnimationState::RunLeft);
        } else {
            SetAnimation(AnimationState::Idle);
        }
    }
    if (input_velocity.x > 0) {
        direction_right_ = true;
    } else if (input_velocity.x < 0) {
        direction_right_ = false;
    }
    if (input_velocity.y > 0 && onGrounded) {
        acceleration_.y = jump_height_;
    }
    velocity_.x = float(move_speed_ * input_velocity.x * Util::Time::GetDeltaTime());
    velocity_ += acceleration_;
    acceleration_ = {0, 0};
    float nearest_time = 1;
    glm::vec2 nearest_delta_position = velocity_;
    float max_time = hypot(velocity_.x, velocity_.y);
    for (const auto &wall: walls) {
        glm::vec2 delta_position = CollisionHandler::SweepTest(GetPosition(), wall->GetPosition(), GetSize(),
                                                               wall->GetSize(), velocity_);
        float delta_time = hypot(delta_position.x, delta_position.y);
        if (max_time != 0){
            float time = delta_time / max_time;
            if (time < nearest_time) {
                nearest_time = time;
                nearest_delta_position = delta_position;
            }
        }
    }
    Translate(nearest_delta_position);
}

bool Character::GroundCheck(const std::vector<std::shared_ptr<Wall>> &others) const {
    for (const auto &other: others) {
        if (CollisionHandler::CheckCollision(GetPosition() - glm::vec2(0, GetSize().y / 2),
                                             glm::vec2(GetSize().x, 0.1), other->GetPosition(),
                                             other->GetSize())) {
            return true;
        }
    }
    return false;
}
