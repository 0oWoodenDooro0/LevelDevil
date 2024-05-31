//
// Created by User on 2024/5/20.
//

#ifndef LEVELDEVIL_LEVELDEVIL_HPP
#define LEVELDEVIL_LEVELDEVIL_HPP

#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Door.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Util/Renderer.hpp"
#include "Portal.hpp"
#include "Saw.hpp"
#include "Spike.hpp"
#include "Button.hpp"
#include "EscButton.hpp"
#include "TriggerCollider.hpp"
#include "Transition.hpp"

class LevelDevil : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        Move1,
        Move2,
        Move3,
        Move4,
        Move5,
        Move6,
        Move7,
        Move8,
        Move9,
        Move10,
        Outro
    };

    explicit LevelDevil(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void ResetLevel();

    void UpdateCurrentState(State state);

    void SpikeAct();

private:
    Util::Renderer renderer_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_7;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_manager_;
    Transition transition_;
    std::shared_ptr<Background> background_;
    std::shared_ptr<EscButton> button_;
    std::vector<std::shared_ptr<GameObject>> walls_;
    std::vector<std::shared_ptr<Portal>> portals_;
    std::vector<std::shared_ptr<Saw>> saws_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::vector<std::shared_ptr<TriggerCollider>> triggerColliders_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;

    float spike_timer_ = 0;
    int spike_num_ = 0;
    float delay_timer_ = 500;
    float revive_timer_ = 500;
    float trap_speed_ = 370;
    bool audio_trig_ = false;
};

#endif //LEVELDEVIL_LEVELDEVIL_HPP
