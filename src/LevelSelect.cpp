//
// Created by User on 2024/3/21.
//

#include "LevelSelect.hpp"

#include <utility>
#include "Util/Animation.hpp"
#include "Util/Time.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

LevelSelect::LevelSelect(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void LevelSelect::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/background/level_select.png");
    renderer_.AddChild(background_);
    std::array<glm::vec2, 15> door_positions = {glm::vec2(-728, 4), glm::vec2(-580, -72), glm::vec2(-404, -8),
                                                glm::vec2(-236, -44), glm::vec2(-332, -184), glm::vec2(-284, -344),
                                                glm::vec2(-84, -324), glm::vec2(24, -192), glm::vec2(0, -28),
                                                glm::vec2(128, 32), glm::vec2(304, -80), glm::vec2(316, -244),
                                                glm::vec2(404, -364), glm::vec2(620, -372), glm::vec2(728, -260)};
    for (int i = 0; i < 15; ++i) {
        std::string image_path;
        if (i < 8 && i != 5) {
            image_path.append(RESOURCE_DIR"/image/ui/unlock_door");
            image_path.append(std::to_string(i+1));
            image_path.append(".png");
        }
        else {
            image_path.append(RESOURCE_DIR"/image/ui/unlock_door.png");
        }
        auto door_button = std::make_shared<DoorButton>(
            std::make_shared<Util::Image>(image_path), audio_manager_);
        door_button->SetPosition(door_positions[i]);
        auto button_hover = std::make_shared<Sprite>(std::make_shared<Util::Animation>(
                std::vector<std::string>{RESOURCE_DIR"/image/ui/door_hover1.png",
                                         RESOURCE_DIR"/image/ui/door_hover2.png",
                                         RESOURCE_DIR"/image/ui/door_hover3.png",
                                         RESOURCE_DIR"/image/ui/door_hover4.png"}, true, 200, true, 0), 11);
        button_hover->SetPosition(door_positions[i] + glm::vec2(0, 32));
        button_hover->SetVisible(false);
        door_buttons_.push_back(door_button);
        button_hovers_.push_back(button_hover);
        door_button->AddChild(button_hover);
        renderer_.AddChild(door_button);
    }
    auto door_button = std::make_shared<DoorButton>(
        std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/unlock_boss_door.png"), audio_manager_);
    door_button->SetPosition({ 635, -20 });
    auto button_hover = std::make_shared<Sprite>(std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR"/image/ui/boos_door_hover1.png",
        RESOURCE_DIR"/image/ui/boos_door_hover2.png",
        RESOURCE_DIR"/image/ui/boos_door_hover3.png",
        RESOURCE_DIR"/image/ui/boos_door_hover4.png"}, true, 200, true, 0), 11);
    button_hover->SetPosition(glm::vec2({ 635, -20 }) + glm::vec2({ -4, 26 }));
    button_hover->SetVisible(false);
    door_buttons_.push_back(door_button);
    button_hovers_.push_back(button_hover);
    door_button->AddChild(button_hover);
    renderer_.AddChild(door_button);
}

void LevelSelect::Update() {
    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
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
                        case 2:
                            level_ = Level::State::LEVEL_3;
                            break;
                        case 3:
                            level_ = Level::State::LEVEL_4;
                            break;
                        case 4:
                            level_ = Level::State::LEVEL_5;
                            break;
                        case 6:
                            level_ = Level::State::LEVEL_7;
                            break;
                        case 7:
                            level_ = Level::State::LEVEL_8;
                            break;
                        case 15:
                            level_ = Level::State::LEVEL_DEVIL;
                            break;
                        default:
                            renderer_.Update();
                            return;
                    }

                    UpdateCurrentState(State::Outro);
                }
            }
            break;
        case State::Outro:
            transition_.Outro([this]() { set_level_state_function_(level_); });
            break;
    }

    renderer_.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        UpdateCurrentState(State::Outro);
        level_ = Level::State::END;
    }
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
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}
