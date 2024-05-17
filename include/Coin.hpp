//
// Created by User on 2024/3/22.
//

#ifndef LEVELDEVIL_COIN_HPP
#define LEVELDEVIL_COIN_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"
#include "GameObject.hpp"

class Coin : public GameObject {
public:
    enum class State {
        Idle,
        Trigger
    };

    explicit Coin(AudioManager audio_manager, const std::string &image_path = RESOURCE_DIR"/image/component/coin.png");

    void SetImage(const std::string &image_path);

    [[nodiscard]] inline Collider GetCollider() const override {
        return {GetPosition(), GetSize() - glm::vec2(42, 42)};
    }

    [[nodiscard]] inline State GetState() const { return current_state_; }

    void Update(const std::shared_ptr<Character> &character_);

private:
    State current_state_ = State::Idle;
    AudioManager audio_manager_;
    std::string image_path_;
};

#endif //LEVELDEVIL_COIN_HPP