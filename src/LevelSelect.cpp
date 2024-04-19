//
// Created by User on 2024/3/21.
//

#include "LevelSelect.hpp"

#include <utility>
#include "Util/Animation.hpp"
#include "Util/Time.hpp"
#include "EasingFunction.hpp"

LevelSelect::LevelSelect(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void LevelSelect::Start() {
    auto top = std::make_shared<MovableSprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_top.png"), 20);
    auto bottom = std::make_shared<MovableSprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_bottom.png"), 20);
    top->SetPosition({0, 208});
    bottom->SetPosition({0, -210});
    transitions_.push_back(top);
    transitions_.push_back(bottom);
    root_.AddChild(top);
    root_.AddChild(bottom);
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/background/level_select.png");
    root_.AddChild(background_);
    std::array<glm::vec2, 15> door_positions = {glm::vec2(-728, 4), glm::vec2(-580, -72), glm::vec2(-404, -8),
                                                glm::vec2(-236, -44), glm::vec2(-332, -184), glm::vec2(-284, -344),
                                                glm::vec2(-84, -324), glm::vec2(24, -192), glm::vec2(0, -28),
                                                glm::vec2(128, 32), glm::vec2(304, -80), glm::vec2(316, -244),
                                                glm::vec2(404, -364), glm::vec2(620, -372), glm::vec2(728, -260)};
    for (int i = 0; i < 15; ++i) {
        auto door_button = std::make_shared<DoorButton>(
                std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/unlock_door.png"), audio_manager_);
        door_button->SetPosition(door_positions[i]);
        auto button_hover = std::make_shared<Sprite>(std::make_shared<Util::Animation>(
                std::vector<std::string>{(RESOURCE_DIR"/image/ui/door_hover1.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover2.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover3.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover4.png")}, true, 200, true, 0), 11);
        button_hover->SetPosition(door_positions[i] + glm::vec2(0, 32));
        button_hover->SetVisible(false);
        door_buttons_.push_back(door_button);
        button_hovers_.push_back(button_hover);
        door_button->AddChild(button_hover);
        root_.AddChild(door_button);
    }
}

void LevelSelect::Update() {
    switch (current_state_) {
        case State::Intro:
            if (transition_timer_ < 1) {
                transition_timer_ += Util::Time::GetDeltaTimeMs() * transition_delta_time_multiple;
            } else {
                transition_timer_ = 1;
                UpdateCurrentState(State::Start);
            }
            transitions_[0]->SetPosition({0, int(208 + EasingFunction::EaseInCubic(transition_timer_) * 544)});
            transitions_[1]->SetPosition({0, int(-210 - EasingFunction::EaseInCubic(transition_timer_) * 540)});
            break;
        case State::Start:
            for (int i = 0; i < int(door_buttons_.size()); ++i) {
                door_buttons_[i]->Update();
                if (door_buttons_[i]->GetState() == Button::State::Click) {
                    switch (i) {
                        case 0:
                            level_ = Level::State::LEVEL_1;
                            break;
                        case 1:
                            level_ = Level::State::LEVEL_2;
                            break;
                        default:
                            level_ = Level::State::LEVEL_2;
                            break;
                    }

                    UpdateCurrentState(State::Outro);
                }
            }
            break;
        case State::Outro:
            if (transition_timer_ < 1) {
                transition_timer_ += Util::Time::GetDeltaTimeMs() * transition_delta_time_multiple;
            } else {
                transition_timer_ = 1;
                transition_end_timer += Util::Time::GetDeltaTimeMs();
                if (transition_end_timer >= transition_end_delay_) {
                    set_level_state_function_(level_);
                }
            }
            transitions_[0]->SetPosition({0, int(752 - EasingFunction::EaseOutCubic(transition_timer_) * 544)});
            transitions_[1]->SetPosition({0, int(-750 + EasingFunction::EaseOutCubic(transition_timer_) * 540)});
            break;
    }

    root_.Update();
}

void LevelSelect::End() {

}

void LevelSelect::UpdateCurrentState(LevelSelect::State state) {
    if (current_state_ == state) return;
    switch (current_state_) {
        case State::Intro:
            if (state == State::Start)current_state_ = state;
            break;
        case State::Start:
            if (state == State::Outro) {
                current_state_ = state;
                transition_timer_ = 0;
            }
            break;
        case State::Outro:
            break;
    }
}
