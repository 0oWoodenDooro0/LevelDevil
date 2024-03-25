//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_LEVELSELECT_HPP
#define LEVELDEVIL_LEVELSELECT_HPP

#include <utility>

#include "ILevel.hpp"
#include "DoorButton.hpp"
#include "Sprite.hpp"
#include "Background.hpp"
#include "Level.hpp"
#include "Util/Root.hpp"
#include "AudioManager.hpp"

class LevelSelect : public ILevel {
public:
    explicit LevelSelect(std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void End() override;

private:
    Util::Root root_;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_manager_;
    std::shared_ptr<Background> background_;
    std::vector<std::shared_ptr<DoorButton>> door_buttons_;
    std::vector<std::shared_ptr<Sprite>> button_hovers_;
};


#endif //LEVELDEVIL_LEVELSELECT_HPP
