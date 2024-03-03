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

class Character : public Util::GameObject {
public:
    enum class AnimationState {
        Idle,
        JumpLeft,
        JumpRight,
        RunLeft,
        RunRight,
    };

    explicit Character();

    Character(const Character &) = delete;

    Character(Character &&) = delete;

    Character &operator=(const Character &) = delete;

    Character &operator=(Character &&) = delete;

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    void SetAnimation(AnimationState animation_state);

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return {24, 64}; }

    void Update(const std::vector<std::shared_ptr<Wall>> &walls);

    void Translate(const glm::vec2 &position);

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<Wall>> &others) const;

private:
    std::shared_ptr<Core::Drawable> idle_drawable_ = std::make_unique<Util::Image>(
            RESOURCE_DIR"/image/character/idle/man_idle.png");
    std::shared_ptr<Core::Drawable> jump_left_drawable_ = std::make_unique<Util::Image>(
            RESOURCE_DIR"/image/character/jump_left/man_jump_left.png");
    std::shared_ptr<Core::Drawable> jump_right_drawable_ = std::make_unique<Util::Image>(
            RESOURCE_DIR"/image/character/jump_right/man_jump_right.png");
    std::shared_ptr<Core::Drawable> run_left_drawable_ = std::make_unique<Util::Animation>(
            std::vector<std::string>{
                    (RESOURCE_DIR"/image/character/run_left/man_run_left1.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left2.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left3.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left4.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left5.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left6.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left7.png"),
                    (RESOURCE_DIR"/image/character/run_left/man_run_left8.png")
            }, true, 40, true, 0);
    std::shared_ptr<Core::Drawable> run_right_drawable_ = std::make_unique<Util::Animation>(
            std::vector<std::string>{
                    (RESOURCE_DIR"/image/character/run_right/man_run_right1.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right2.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right3.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right4.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right5.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right6.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right7.png"),
                    (RESOURCE_DIR"/image/character/run_right/man_run_right8.png")
            }, true, 40, true, 0);
    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -0.98;
    glm::vec2 acceleration_ = {0, 0};
    glm::vec2 velocity_ = {0, 0};
    glm::vec2 last_safe_position = {0, 0};
    bool direction_right_ = true;
    AnimationState animation_state_ = AnimationState::Idle;
};

#endif //LEVELDEVIL_CHARACTER_HPP
