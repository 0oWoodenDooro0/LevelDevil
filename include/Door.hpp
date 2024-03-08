//
// Created by User on 2024/3/6.
//

#ifndef LEVELDEVIL_DOOR_HPP
#define LEVELDEVIL_DOOR_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "Collider.hpp"

class Door : public Util::GameObject {
public:
    explicit Door();

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] inline Collider GetCollider() const { return { GetPosition(), GetSize()}; }

    void Update(std::shared_ptr<Character> character_);

private:
    Animator animator_;
};

#endif //LEVELDEVIL_DOOR_HPP