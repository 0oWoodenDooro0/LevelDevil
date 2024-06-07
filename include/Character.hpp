//
// Created by User on 2024/3/1.
//

#ifndef LEVELDEVIL_CHARACTER_HPP
#define LEVELDEVIL_CHARACTER_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/SFX.hpp"
#include "Animator.hpp"
#include "Rigidbody.hpp"
#include "Collider.hpp"
#include "SoundEffect.hpp"
#include "AudioManager.hpp"

class Character : public GameObject {
public:
    enum class State {
        Alive,
        Dead,
        LevelClear,
        Vanish,
        Appear,
        Portal,
        Warp
    };

    explicit Character(AudioManager audio_manager);

    void SetCheckPoint(glm::vec2 check_point);

    inline void SetMoveSpeed(float speed) { move_speed_ = speed; }

    [[nodiscard]] inline Collider GetCollider() const override { return {GetPosition() - glm::vec2(2, 6), {32, 52}}; }

    [[nodiscard]] inline State GetCurrentState() const { return current_state_; }

    [[nodiscard]] inline bool GetGod() const { return god_; }

    inline void ChangeGod() { god_ = !GetGod(); }

    void CharacterMove(glm::vec2 input_velocity, const std::vector<std::shared_ptr<GameObject>> &walls);

    void GodMove(glm::vec2 input_velocity);

    void Update(glm::vec2 input_velocity, const std::vector<std::shared_ptr<GameObject>> &walls);

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<GameObject>> &others) const;

    void Revive();

    void Dead();

    void Vanish();

    void Appear();

    void Bounce();

    void Warp();

    void UpdateState(State state);

private:
    State current_state_ = State::Alive;
    Animator animator_;
    Rigidbody rigidbody_;
    AudioManager audio_manager_;

    glm::vec2 check_point_ = { 0, 0 };
    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -1;
    float spring_height_ = 24;
    bool is_direction_right_ = true;
    bool is_run_ = false;
    bool god_ = false;
};

#endif //LEVELDEVIL_CHARACTER_HPP
