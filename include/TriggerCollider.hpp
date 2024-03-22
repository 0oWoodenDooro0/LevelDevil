//
// Created by User on 2024/3/22.
//

#ifndef LEVELDEVIL_TRIGGERCOLLIDER_HPP
#define LEVELDEVIL_TRIGGERCOLLIDER_HPP

#include "Util/GameObject.hpp"
#include "glm/vec2.hpp"

class TriggerCollider : public Util::GameObject {
public:
    enum class State {
        Idle,
        Trigger
    };

    TriggerCollider() = default;

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }
};


#endif //LEVELDEVIL_TRIGGERCOLLIDER_HPP
