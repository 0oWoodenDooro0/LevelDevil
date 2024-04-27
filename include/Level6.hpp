//
// Created by User on 2024/4/27.
//

#ifndef LEVELDEVIL_LEVEL6_HPP
#define LEVELDEVIL_LEVEL6_HPP


#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Level.hpp"
#include "Util/Renderer.hpp"
#include "Transition.hpp"
#include "Background.hpp"
#include "EscButton.hpp"
#include "TriggerCollider.hpp"
#include "Door.hpp"
#include "Character.hpp"

class Level6 : public ILevel {
public:
    enum class State {
        Intro,
        Start,
        Move1,
        Move2,
        Move3,
        Outro
    };

    explicit Level6(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void ResetLevel();

    void UpdateCurrentState(State state);

    void Move1();

    void Move2();

    void Move3();

private:
    Util::Renderer renderer_;
    State current_state_ = State::Intro;
    Level::State level_ = Level::State::LEVEL_1;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_manager_;
    Transition transition_;
    std::shared_ptr<Background> background_;
    std::shared_ptr<EscButton> button_;
    std::vector<std::shared_ptr<Sprite>> walls_;
    std::vector<std::shared_ptr<MovableSprite>> movable_walls_;
    std::vector<std::shared_ptr<TriggerCollider>> triggerColliders_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;

    float move1_timer_ = 200;
    int number_of_platform_ = 0;
    float move2_timer_ = 200;
    float platform_down_speed_ = 500;
    float platform_up_speed_ = 300;
};



#endif //LEVELDEVIL_LEVEL6_HPP
