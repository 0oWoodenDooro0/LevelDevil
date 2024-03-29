//
// Created by User on 2024/3/29.
//

#ifndef LEVELDEVIL_INPUTHANDLER_HPP
#define LEVELDEVIL_INPUTHANDLER_HPP

#include "glm/vec2.hpp"

class InputHandler {
public:
    static glm::vec2 GetCharacterMoveVelocity();
    static bool isForwardPressed();
    static bool isBackwardPressed();
    static bool isJumpPressed();

};


#endif //LEVELDEVIL_INPUTHANDLER_HPP
