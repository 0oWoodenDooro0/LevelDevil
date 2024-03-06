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

class Character : public Util::GameObject {
public:
    explicit Character();

    Character(const Character &) = delete;

    Character(Character &&) = delete;

    Character &operator=(const Character &) = delete;

    Character &operator=(Character &&) = delete;

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return {32, 64}; }

    void Update(const std::vector<std::shared_ptr<Wall>> &walls);

    void Translate(const glm::vec2 &position);

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<Wall>> &others) const;

private:
    Animator animator_;

    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -0.98;
    glm::vec2 acceleration_ = {0, 0};
    glm::vec2 velocity_ = {0, 0};
    bool direction_right_ = true;
};

#endif //LEVELDEVIL_CHARACTER_HPP
