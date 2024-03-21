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

class Character : public Util::GameObject, public IBehaviour{
public:
    explicit Character(AudioManager audioManager);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline Collider GetCollider() const { return {GetPosition() - glm::vec2(2, 6), {36, 52}}; }

    void Update(const std::vector<std::shared_ptr<Sprite>> &walls);

    void Enable() override;

    void Disable() override;

    [[nodiscard]] bool GroundCheck(const std::vector<std::shared_ptr<Sprite>> &others) const;

    void Dead();

    void Bouns();

private:
    Animator animator_;
    Rigidbody rigidbody_;
    AudioManager audioManager_;

    float move_speed_ = 350;
    float jump_height_ = 12;
    float gravity_ = -0.98;
    bool dead_or_clear_ = false;
    bool is_direction_right_ = true;
    bool is_run_ = false;
};

#endif //LEVELDEVIL_CHARACTER_HPP
