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
#include "Util/SFX.hpp"
#include "Animator.hpp"
#include "Rigidbody.hpp"
#include "Collider.hpp"
#include "SoundEffect.hpp"
#include "AudioManager.hpp"
#include "IBehaviour.hpp"

class Character : public Util::GameObject, public IBehaviour {
public:
    explicit Character(AudioManager audioManager);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    void SetCheckPoint(glm::vec2 check_point);

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline Collider GetCollider() const { return {GetPosition() - glm::vec2(2, 6), {36, 52}}; }

    void Update(const std::vector<std::shared_ptr<Sprite>> &walls);

    void Enable() override;

    void Disable() override;

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<Sprite>> &others) const;

    static bool isForwardPressed();

    static bool isBackwardPressed();

    static bool isJumpPressed();

    void Revive();

    void Dead(AudioManager::SFX sfx);

    void LevelClear();

    void Bounce();

private:
    Animator animator_;
    Rigidbody rigidbody_;
    AudioManager audioManager_;

    glm::vec2 check_point_ = {0, 0};
    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -0.98;
    float spring_height_ = 24;
    bool enabled_ = true;
    bool is_direction_right_ = true;
    bool is_run_ = false;
    bool level_clear_ = false;
};

#endif //LEVELDEVIL_CHARACTER_HPP
