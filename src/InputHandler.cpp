//
// Created by User on 2024/3/29.
//

#include "InputHandler.hpp"

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

glm::vec2 InputHandler::GetGodMoveVelocity() {
    glm::vec2 input_velocity = {0, 0};
    if (isForwardPressed()) {
        input_velocity.x = 1;
    } else if (isBackwardPressed()) {
        input_velocity.x = -1;
    }
    if (isUpPressed()) {
        input_velocity.y = 1;
    } else if (isDownPressed()) {
        input_velocity.y = -1;
    }
    return input_velocity;
}

bool InputHandler::isForwardPressed() {
    return std::any_of(forward_keycodes_.begin(), forward_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyPressed(keycode); });
}

bool InputHandler::isBackwardPressed() {
    return std::any_of(backward_keycodes_.begin(), backward_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyPressed(keycode); });
}

bool InputHandler::isJumpPressed() {
    return std::any_of(jump_keycodes_.begin(), jump_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyDown(keycode); });
}

bool InputHandler::isUpPressed() {
    return std::any_of(up_keycodes_.begin(), up_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyPressed(keycode); });
}

bool InputHandler::isDownPressed() {
    return std::any_of(down_keycodes_.begin(), down_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyPressed(keycode); });
}

bool InputHandler::isRevivePressed() {
    return std::any_of(revive_keycodes_.begin(), revive_keycodes_.end(),
                       [](Util::Keycode keycode) { return Util::Input::IsKeyDown(keycode); });
}

bool InputHandler::isResetLevelPressed() {
    return Util::Input::IsKeyDown(reset_level_keycode);
}

bool InputHandler::isGodPressed() {
    return Util::Input::IsKeyDown(god_keycode);
}
