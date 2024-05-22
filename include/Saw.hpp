//
// Created by User on 2024/5/22.
//

#ifndef LEVELDEVIL_SAW_HPP
#define LEVELDEVIL_SAW_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"

class Saw : public GameObject {
public:
    explicit Saw(const std::string& image_path, Position position_inbox, AudioManager audio_manager);

    void SetImage(const std::string& image_path);

    [[nodiscard]] Collider GetCollider() const override;

    void Update(const std::shared_ptr<Character>& character);

    void Enable() override;

    void Disable() override;

private:
    Position position_inbox_ = Position::Bottom;
    std::string image_path_;
    AudioManager audio_manager_;
};

#endif //LEVELDEVIL_SAW_HPP