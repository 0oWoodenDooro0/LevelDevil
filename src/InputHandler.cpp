//
// Created by User on 2024/3/29.
//

#include "InputHandler.hpp"
#include "Util/Input.hpp"

glm::vec2 InputHandler::GetCharacterMoveVelocity() {
    glm::vec2 input_velocity = {0, 0};
    if (isForwardPressed()) {
        input_velocity.x = 1;
    } else if (isBackwardPressed()) {
        input_velocity.x = -1;
    }
    if (isJumpPressed()) {
        input_velocity.y = 1;
    }
    return input_velocity;
}

bool InputHandler::isForwardPressed() {
    return Util::Input::IsKeyPressed(Util::Keycode::D) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT);
}

bool InputHandler::isBackwardPressed() {
    return Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::LEFT);
}

bool InputHandler::isJumpPressed() {
    return Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
           Util::Input::IsKeyDown(Util::Keycode::UP);
}
