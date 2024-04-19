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
#include "Util/Renderer.hpp"
#include "AudioManager.hpp"
#include "MovableSprite.hpp"

class LevelSelect : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        Outro
    };

    explicit LevelSelect(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void End() override;

    void UpdateCurrentState(State state);

private:
    Util::Renderer root_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_SELECT;

    std::function<void(Level::State)> set_level_state_function_;

    std::vector<std::shared_ptr<MovableSprite>> transitions_;
    float transition_timer_ = 0;
    float transition_delta_time_multiple = 0.003f;
    float transition_end_timer = 0;
    float transition_end_delay_ = 300;

    AudioManager audio_manager_;
    std::shared_ptr<Background> background_;
    std::vector<std::shared_ptr<DoorButton>> door_buttons_;
    std::vector<std::shared_ptr<Sprite>> button_hovers_;
};


#endif //LEVELDEVIL_LEVELSELECT_HPP
