//
// Created by User on 2024/4/27.
//

#ifndef LEVELDEVIL_LEVEL3_HPP
#define LEVELDEVIL_LEVEL3_HPP

#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Door.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Util/Renderer.hpp"
#include "Button.hpp"
#include "EscButton.hpp"
#include "Spike.hpp"
#include "TriggerCollider.hpp"
#include "Transition.hpp"

class Level3 : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        Move,
        Spike,
        Outro
    };

    explicit Level3(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void ResetLevel();

    void UpdateCurrentState(State state);

    void SpikeDelay();

private:
    Util::Renderer renderer_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_3;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_manager_;
    Transition transition_;
    std::shared_ptr<Background> background_;
    std::shared_ptr<EscButton> button_;
    std::vector<std::shared_ptr<GameObject>> walls_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::vector<std::shared_ptr<TriggerCollider>> triggerColliders_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;

    float speed_ = 300;
    float timer_ = 2000;
    float revive_timer_ = 500;
};

#endif //LEVELDEVIL_LEVEL3_HPP
