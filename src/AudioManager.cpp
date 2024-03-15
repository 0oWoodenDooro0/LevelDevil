//
// Created by User on 2024/3/10.
//

#include "AudioManager.hpp"

void AudioManager::Update() {
    for (auto sfx : all_sfxs_){
        sfxs_[sfx]->Update();
    }
}

void AudioManager::Play(AudioManager::SFX sfx) {
    sfxs_[sfx]->Play();
}
