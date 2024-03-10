//
// Created by User on 2024/3/10.
//

#ifndef LEVELDEVIL_AUDIOMANAGER_HPP
#define LEVELDEVIL_AUDIOMANAGER_HPP


#include <map>
#include "SoundEffect.hpp"

class AudioManager {
public:
    enum class SFX {
        Run,
        Jump
    };

    AudioManager() = default;

    void Play(SFX sfx);

    void Update();

private:
    std::array<SFX, 2> all_sfxs_ = {SFX::Run, SFX::Jump};
    std::map<SFX, std::shared_ptr<SoundEffect>> sfxs_ = {
            {SFX::Run,  std::make_shared<SoundEffect>(RESOURCE_DIR"/sound/run.mp3", 0.25)},
            {SFX::Jump, std::make_shared<SoundEffect>(RESOURCE_DIR"/sound/jump.mp3", 0.13)}
    };
};


#endif //LEVELDEVIL_AUDIOMANAGER_HPP
