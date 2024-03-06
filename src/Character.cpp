//
// Created by User on 2024/3/1.
//

#include "Character.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"

Character::Character() : Util::GameObject() {
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

void Character::Translate(const glm::vec2 &position) {
    m_Transform.translation += position;
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
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
            std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (!onGrounded) {
        acceleration_.y = gravity_;
        if (direction_right_) {
            animator_.UpdateAnimationState("JumpRight", set_drawable_function);
        } else {
            animator_.UpdateAnimationState("JumpLeft", set_drawable_function);
        }
    } else {
        velocity_ = {0, 0};
        if (input_velocity.x > 0) {
            animator_.UpdateAnimationState("RunRight", set_drawable_function);
        } else if (input_velocity.x < 0) {
            animator_.UpdateAnimationState("RunLeft", set_drawable_function);
        } else {
            animator_.UpdateAnimationState("Idle", set_drawable_function);
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
        if (max_time != 0) {
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
