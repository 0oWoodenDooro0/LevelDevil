//
// Created by User on 2024/3/21.
//
#ifndef LEVELDEVIL_LEVEL2_HPP
#define LEVELDEVIL_LEVEL2_HPP

#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Door.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Util/Renderer.hpp"
#include "MovableSprite.hpp"
#include "Spike.hpp"
#include "Button.hpp"
#include "EscButton.hpp"
#include "TriggerCollider.hpp"

class Level2 : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        Spike,
        Outro
    };

    explicit Level2(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void End() override;

    void ResetLevel();

    void UpdateCurrentState(State state);

private:
    Util::Renderer root_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_2;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_maganer_;
    std::vector<std::shared_ptr<MovableSprite>> transitions_;
    std::shared_ptr<Background> background_;
    std::shared_ptr<EscButton> button_;
    std::vector<std::shared_ptr<Sprite>> walls_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::vector<std::shared_ptr<TriggerCollider>> triggerColliders_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;
};

#endif //LEVELDEVIL_LEVEL2_HPP