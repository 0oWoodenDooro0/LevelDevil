//
// Created by User on 2024/3/21.
//

#include "Level1.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "EasingFunction.hpp"

#include <utility>

Level1::Level1(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level1::Start() {
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
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level1/background.png");
    root_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    root_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-576, -128});
    root_.AddChild(character_);
    door_ = std::make_shared<Door>(audio_manager_);
    door_->SetPosition({576, -128});
    root_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/top.png",
                                               RESOURCE_DIR"/image/level/level1/bottom.png",
                                               RESOURCE_DIR"/image/level/level1/side.png",
                                               RESOURCE_DIR"/image/level/level1/side.png"};
    for (int i = 0; i < 4; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        root_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 320});
    walls_[1]->SetPosition({640, -320});
    walls_[2]->SetPosition({768, 0});
    walls_[3]->SetPosition({-768, 0});
    auto movable_wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/move1.png",
                                                       RESOURCE_DIR"/image/level/level1/move2.png"};
    for (int i = 0; i < 2; ++i) {
        auto movable_wall = std::make_shared<MovableSprite>(std::make_shared<Util::Image>(movable_wall_image[i]));
        movable_walls_.push_back(movable_wall);
        walls_.push_back(movable_wall);
        root_.AddChild(movable_wall);
    }
    walls_[4]->SetPosition({-288, -320});
    walls_[5]->SetPosition({256, -320});

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-200, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({275, 0}, {20, 1000})));
}

void Level1::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480) {
            character_->UpdateState(Character::State::Dead);
        }
        auto input_vector = InputHandler::GetCharacterMoveVelocity();
        character_->Move(input_vector, walls_);
    } else {
        if (character_->GetCurrentState() != Character::State::LevelClear && InputHandler::isRevivePressed()) {
            character_->Revive();
            ResetLevel();
        }
    }

    button_->Update();
    if (button_->GetState() == Button::State::Click) {
        level_ = Level::State::LEVEL_SELECT;
        UpdateCurrentState(State::Outro);
    }

    door_->Update(character_);
    if (!door_->GetEnabled()) {
        level_ = Level::State::LEVEL_SELECT;
        UpdateCurrentState(State::Outro);
    }

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
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move1);
            }
            break;
        case State::Move1:
            movable_walls_[0]->Move({480, -320}, 750);
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move2);
            }
            break;
        case State::Move2:
            movable_walls_[0]->SetPosition({1000, 1000});
            movable_walls_[0]->Disable();
            movable_walls_[1]->Move({192, -320}, 500);
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

void Level1::End() {

}

void Level1::ResetLevel() {
    movable_walls_[0]->SetPosition({-288, -320});
    movable_walls_[1]->SetPosition({256, -320});
    current_state_ = State::Start;
    movable_walls_[0]->Enable();
}

void Level1::UpdateCurrentState(State state) {
    if (current_state_ == state)return;
    switch (current_state_) {
        case State::Intro:
            if (state == State::Start) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_timer_ = 0;
            }
            break;
        case State::Start:
            if (state == State::Move1) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_timer_ = 0;
            }
            break;
        case State::Move1:
            if (state == State::Move2) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_timer_ = 0;
            }
            break;
        case State::Move2:
            if (state == State::Outro) {
                current_state_ = state;
                transition_timer_ = 0;
            }
            break;
        case State::Outro:
            break;
    }
}
