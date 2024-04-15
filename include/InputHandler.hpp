//
// Created by User on 2024/3/29.
//

#ifndef LEVELDEVIL_INPUTHANDLER_HPP
#define LEVELDEVIL_INPUTHANDLER_HPP

#include "glm/vec2.hpp"
#include "Util/Input.hpp"

class InputHandler {
public:
    static glm::vec2 GetCharacterMoveVelocity();

    static bool isForwardPressed();

    static bool isBackwardPressed();

    static bool isJumpPressed();

    static bool isRevivePressed();

private:
    constexpr static const std::array<Util::Keycode, 2> forward_keycodes_ = {Util::Keycode::D, Util::Keycode::RIGHT};
    constexpr static const std::array<Util::Keycode, 2> backward_keycodes_ = {Util::Keycode::A, Util::Keycode::LEFT};
    constexpr static const std::array<Util::Keycode, 3> jump_keycodes_ = {Util::Keycode::W, Util::Keycode::SPACE,
                                                                           Util::Keycode::UP};
    constexpr static const std::array<Util::Keycode, 7> revive_keycodes_ = {Util::Keycode::D, Util::Keycode::RIGHT,
                                                                            Util::Keycode::A, Util::Keycode::LEFT,
                                                                            Util::Keycode::W, Util::Keycode::SPACE,
                                                                            Util::Keycode::UP};
};


#endif //LEVELDEVIL_INPUTHANDLER_HPP
