//
// Created by User on 2024/3/10.
//

#include "SoundEffect.hpp"
#include "Util/Time.hpp"

SoundEffect::SoundEffect(const std::string &path, float cooldown) : sfx_(Util::SFX(path)), initial_cooldown_(cooldown),
                                                                    cooldown_(cooldown) {}

void SoundEffect::Play() {
    if (cooldown_ < 0) {
        sfx_.Play();
        cooldown_ = initial_cooldown_;
    }
}

void SoundEffect::Update() {
    cooldown_ -= Util::Time::GetDeltaTimeMs();
}
