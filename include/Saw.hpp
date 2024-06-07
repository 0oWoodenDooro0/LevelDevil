//
// Created by User on 2024/5/22.
//

#ifndef LEVELDEVIL_SAW_HPP
#define LEVELDEVIL_SAW_HPP

#define _USE_MATH_DEFINES

#include <string>
#include <glm/vec2.hpp>
#include <cmath>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"

class Saw : public GameObject {
public:
    explicit Saw(const std::string& image_path, AudioManager audio_manager);

    void SetImage(const std::string& image_path);

    void Update(const std::shared_ptr<Character>& character);

    void Enable() override;

    void Disable() override;

private:
    std::string image_path_;
    AudioManager audio_manager_;
    float angle_ = 0;
    float rotate_ = 2 * M_PI / 16;
    float rotate_timer_ = 100;
};

#endif //LEVELDEVIL_SAW_HPP