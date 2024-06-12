//
// Created by User on 2024/3/21.
//

#include "Level8.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "Movable.hpp"

#include <utility>

Level8::Level8(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level8::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level8/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-768, 320});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/level8/door.png",
                                          RESOURCE_DIR"/image/level/level8/in_door1.png",
                                          RESOURCE_DIR"/image/level/level8/in_door2.png",
                                          RESOURCE_DIR"/image/level/level8/in_door3.png",
                                          RESOURCE_DIR"/image/level/level8/in_door4.png",
                                          RESOURCE_DIR"/image/level/level8/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({768, 192});
    renderer_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level8/top.png",
                                               RESOURCE_DIR"/image/level/level8/Ltop.png",
                                               RESOURCE_DIR"/image/level/level8/Lbottom.png",
                                               RESOURCE_DIR"/image/level/level8/Rbottom.png",
                                               RESOURCE_DIR"/image/level/level8/Lside.png",
                                               RESOURCE_DIR"/image/level/level8/Rside.png",
                                               RESOURCE_DIR"/image/level/level8/platform.png",
                                               RESOURCE_DIR"/image/level/level8/platform.png"};
    for (int i = 0; i < 8; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 448});
    walls_[1]->SetPosition({-800, 416});
    walls_[2]->SetPosition({-800, 32});
    walls_[3]->SetPosition({800, -160});
    walls_[4]->SetPosition({-832, 128});
    walls_[5]->SetPosition({832, 0});
    walls_[6]->SetPosition({-128, 320});
    walls_[7]->SetPosition({512, 320});

    auto portal_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level8/portal1.png",
                                                 RESOURCE_DIR"/image/level/level8/portal2.png"};

    for (int i = 0; i < 5; ++i) {
        auto portal = std::make_shared<Portal>(portal_image, audio_manager_);
        portals_.push_back(portal);
        renderer_.AddChild(portal);
    }

    portals_[0]->SetPosition({640, -192});
    portals_[1]->SetPosition({384, -192});
    portals_[2]->SetPosition({0, -192});
    portals_[3]->SetPosition({-256, -192});
    portals_[4]->SetPosition({-512, -192});
    portals_[0]->SetGoal({768, 352});
    portals_[1]->SetGoal({640, 160});
    portals_[2]->SetGoal({-128, 384});
    portals_[3]->SetGoal({512, 384});
    portals_[4]->SetGoal({-128, 384});

    glm::vec2 goal = {-768, -264};
    auto movableportal = std::make_shared<Portal>(portal_image, audio_manager_, goal);
    portals_.push_back(movableportal);
    renderer_.AddChild(movableportal);
    movableportal->SetPosition({-640, -64});

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-128, 288}, {320, 32})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({512, 288}, {320, 32})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({384, -128}, {128, 64})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({0, -128}, {128, 64})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-512, -128}, {128, 64})));
}

void Level8::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480 && character_->GetCurrentState() != Character::State::Vanish) {
            character_->UpdateState(Character::State::Dead);
        }
        if (InputHandler::isGodPressed()) {
            character_->ChangeGod();
        }
        glm::vec2 input_velocity = {0, 0};
        if (character_->GetGod()) {
            input_velocity = InputHandler::GetGodMoveVelocity();
        } else {
            input_velocity = InputHandler::GetCharacterMoveVelocity();
        }
        character_->Update(input_velocity, walls_);
    } else {
        if (revive_timer_ > 0) {
            revive_timer_ -= Util::Time::GetDeltaTimeMs();
        } else if (character_->GetCurrentState() != Character::State::LevelClear && InputHandler::isRevivePressed()) {
            ResetLevel();
        }
    }

    if (InputHandler::isResetLevelPressed() && door_->GetState() == Door::State::Idle) {
        ResetLevel();
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

    for (int i = 0; i < 6; i++) {
        portals_[i]->Update(character_);
    }

    for (int i = 0; i < 5; i++) {
        if (portals_[i]->IsCollide())
            portals_[i]->Disable();
    }

    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            if (character_->GetCurrentState() == Character::State::Alive)
                triggerColliders_[4]->Update(character_->GetPosition());
            if (triggerColliders_[4]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move1);
            }
            break;
        case State::Move1:
            Movable::Move(portals_[5], {0, -64}, 2000);
            if (character_->GetCurrentState() == Character::State::Alive)
                triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move2);
            }
            break;
        case State::Move2:
            Movable::Move(portals_[5], {-256, -64}, 2000);
            if (character_->GetCurrentState() == Character::State::Alive)
                triggerColliders_[3]->Update(character_->GetPosition());
            if (triggerColliders_[3]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move3);
            }
            break;
        case State::Move3:
            Movable::Move(portals_[5], {384, -64}, 2000);
            if (character_->GetCurrentState() == Character::State::Alive)
                triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move4);
            }
            break;
        case State::Move4:
            Movable::Move(portals_[5], {640, -64}, 2000);
            if (character_->GetCurrentState() == Character::State::Alive)
                triggerColliders_[2]->Update(character_->GetPosition());
            if (triggerColliders_[2]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move5);
            }
            break;
        case State::Move5:
            Movable::Move(portals_[5], {-1000, -64}, 2000);
            break;
        case State::Outro:
            transition_.Outro([this]() { set_level_state_function_(level_); });
            break;
    }

    renderer_.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        UpdateCurrentState(State::Outro);
        level_ = Level::State::LEVEL_SELECT;
    }
}

void Level8::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
    portals_[5]->SetPosition({-640, -64});
    for (int i = 0; i < 6; i++) {
        portals_[i]->Enable();
    }
    current_state_ = State::Start;
    for (auto collider: triggerColliders_) {
        collider->UpdateState(TriggerCollider::State::Idle);
    }
}

void Level8::UpdateCurrentState(State state) {
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
            if (state == State::Move4) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move4:
            if (state == State::Move5) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move5:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}