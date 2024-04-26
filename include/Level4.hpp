//
// Created by User on 2024/4/26.
//
#ifndef LEVELDEVIL_LEVEL4_HPP
#define LEVELDEVIL_LEVEL4_HPP

#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Door.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Util/Renderer.hpp"
#include "MovableSprite.hpp"
#include "Coin.hpp"
#include "Button.hpp"
#include "EscButton.hpp"
#include "TriggerCollider.hpp"
#include "Util/Time.hpp"
#include "Transition.hpp"

class Level4 : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        CoinMove,
        Outro
    };

    explicit Level4(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void ResetLevel();

    void UpdateCurrentState(State state);


private:
    Util::Renderer root_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_4;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_manager_;
    Transition transition_;
    std::shared_ptr<Background> background_;
    std::shared_ptr<EscButton> button_;
    std::vector<std::shared_ptr<Sprite>> walls_;
    std::vector<std::shared_ptr<Coin>> coins_;
    std::vector<std::shared_ptr<TriggerCollider>> triggerColliders_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;
    float timer_ = 0;
};

#endif //LEVELDEVIL_LEVEL4_HPP