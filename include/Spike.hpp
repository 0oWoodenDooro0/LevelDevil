//
// Created by User on 2024/3/6.
//

#ifndef LEVELDEVIL_SPIKE_HPP
#define LEVELDEVIL_SPIKE_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"

class Spike : public GameObject {
public:
    enum class Position {
        Top,
        Bottom,
        Left,
        Right
    };

    explicit Spike(const std::string &image_path, Position position_inbox, AudioManager audio_manager);

    void SetImage(const std::string &image_path);

    [[nodiscard]] Collider GetCollider() const override;

    void Update(const std::shared_ptr<Character> &character);

    void Enable() override;

private:
    Position position_inbox_ = Position::Bottom;
    std::string image_path_;
    AudioManager audio_manager_;
};

#endif //LEVELDEVIL_SPIKE_HPP