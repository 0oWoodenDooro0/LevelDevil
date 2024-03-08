//
// Created by User on 2024/3/1.
//

#ifndef LEVELDEVIL_CHARACTER_HPP
#define LEVELDEVIL_CHARACTER_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Wall.hpp"
#include "Animator.hpp"
#include "Rigidbody.hpp"
#include "Collider.hpp"

class Character : public Util::GameObject {
public:
    explicit Character();

    Character(const Character &) = delete;

    Character(Character &&) = delete;

    Character &operator=(const Character &) = delete;

    Character &operator=(Character &&) = delete;

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return {32, 64}; }

    [[nodiscard]] inline Collider GetCollider() const { return {GetPosition() - glm::vec2(2, 6), {36, 52}}; }

    void Update(const std::vector<std::shared_ptr<Wall>> &walls);

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<Wall>> &others) const;

    void Dead();

private:
    Animator animator_;
    Rigidbody rigidbody_;

    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -0.98;
    bool direction_right_ = true;
};

#endif //LEVELDEVIL_CHARACTER_HPP
