//
// Created by User on 2024/3/10.
//

#ifndef LEVELDEVIL_SOUNDEFFECT_HPP
#define LEVELDEVIL_SOUNDEFFECT_HPP


#include <string>
#include "Util/SFX.hpp"

class SoundEffect {
public:
    explicit SoundEffect(const std::string_view path, float cooldown = 0);

    void Play();

    void Update();

private:
    Util::SFX sfx_;
    float initial_cooldown_;
    float cooldown_;
};


#endif //LEVELDEVIL_SOUNDEFFECT_HPP
