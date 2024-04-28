//
// Created by User on 2024/4/27.
//

#include "Level6.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"

#include <utility>

Level6::Level6(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level6::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level6/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-448, -256});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/level6/door.png",
                                          RESOURCE_DIR"/image/level/level6/in_door1.png",
                                          RESOURCE_DIR"/image/level/level6/in_door2.png",
                                          RESOURCE_DIR"/image/level/level6/in_door3.png",
                                          RESOURCE_DIR"/image/level/level6/in_door4.png",
                                          RESOURCE_DIR"/image/level/level6/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({576, 256});
    renderer_.AddChild(door_);
    auto wall_images = std::vector<std::string>{RESOURCE_DIR"/image/level/level6/top.png",
                                                RESOURCE_DIR"/image/level/level6/left.png",
                                                RESOURCE_DIR"/image/level/level6/bottom.png",
                                                RESOURCE_DIR"/image/level/level6/right1.png",
                                                RESOURCE_DIR"/image/level/level6/right2.png"};
    for (const auto &wall_image: wall_images) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 448});
    walls_[1]->SetPosition({-768, 0});
    walls_[2]->SetPosition({-608, -384});
    walls_[3]->SetPosition({768, 0});
    walls_[4]->SetPosition({704, -128});
    auto movable_wall_images = std::vector<std::string>{RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png",
                                                        RESOURCE_DIR"/image/level/level6/platform.png"};
    for (const auto &movable_wall_image: movable_wall_images) {
        auto movable_wall = std::make_shared<MovableSprite>(std::make_shared<Util::Image>(movable_wall_image));
        movable_walls_.push_back(movable_wall);
        walls_.push_back(movable_wall);
        renderer_.AddChild(movable_wall);
    }
    movable_walls_[0]->SetPosition({-288, -256});
    movable_walls_[1]->SetPosition({-160, -192});
    movable_walls_[2]->SetPosition({-32, -128});
    movable_walls_[3]->SetPosition({96, -64});
    movable_walls_[4]->SetPosition({224, 0});
    movable_walls_[5]->SetPosition({352, 64});
    movable_walls_[6]->SetPosition({480, 128});

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-32, -128}, {128, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({352, 128}, {128, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({480, 256}, {128, 1000})));
}

void Level6::Update() {
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
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move1);
            }
            break;
        case State::Move1:
            Move1();
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move2);
            }
            break;
        case State::Move2:
            Move1();
            Move2();
            triggerColliders_[2]->Update(character_->GetPosition());
            if (triggerColliders_[2]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move3);
            }
            break;
        case State::Move3:
            Move1();
            Move3();
            break;
        case State::Outro:
            transition_.Outro([this]() { set_level_state_function_(level_); });
            break;
    }

    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -484) {
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

    renderer_.Update();
}

void Level6::ResetLevel() {
    movable_walls_[0]->SetPosition({-288, -256});
    movable_walls_[1]->SetPosition({-160, -192});
    movable_walls_[2]->SetPosition({-32, -128});
    movable_walls_[3]->SetPosition({96, -64});
    movable_walls_[4]->SetPosition({224, 0});
    movable_walls_[5]->SetPosition({352, 64});
    movable_walls_[6]->SetPosition({480, 128});
    number_of_platform_ = 0;
    current_state_ = State::Start;
}

void Level6::UpdateCurrentState(Level6::State state) {
    if (current_state_ == state)return;
    switch (current_state_) {
        case State::Intro:
            if (state == State::Start) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Start:
            if (state == State::Move1) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move1:
            if (state == State::Move2) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move2:
            if (state == State::Move3) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move3:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}

void Level6::Move1() {
    if (move1_timer_ <= 0) {
        move1_timer_ = 500;
        if (number_of_platform_ < 6) {
            number_of_platform_ += 1;
        }
    } else {
        move1_timer_ -= Util::Time::GetDeltaTimeMs();
    }
    for (int i = 0; i < number_of_platform_ - 1; ++i) {
        movable_walls_[i]->Move({movable_walls_[i]->GetPosition().x, -600}, platform_down_speed_);
    }
}

void Level6::Move2() {
    if (move2_timer_ <= 0) {
        movable_walls_[5]->Move({movable_walls_[5]->GetPosition().x, 480}, platform_up_speed_);
    } else {
        move2_timer_ -= Util::Time::GetDeltaTimeMs();
    }
}

void Level6::Move3() {
    movable_walls_[6]->Move({movable_walls_[6]->GetPosition().x, -600}, platform_down_speed_);
}
