//
// Created by User on 2024/4/27.
//

#include "Level3.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "Movable.hpp"

#include <utility>

Level3::Level3(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level3::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level3/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({640, -128});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/level3/door.png",
                                          RESOURCE_DIR"/image/level/level3/in_door1.png",
                                          RESOURCE_DIR"/image/level/level3/in_door2.png",
                                          RESOURCE_DIR"/image/level/level3/in_door3.png",
                                          RESOURCE_DIR"/image/level/level3/in_door4.png",
                                          RESOURCE_DIR"/image/level/level3/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({-704, -128});
    renderer_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level3/top.png",
                                               RESOURCE_DIR"/image/level/level3/Lbottom.png",
                                               RESOURCE_DIR"/image/level/level3/Rbottom.png",
                                               RESOURCE_DIR"/image/level/level3/Lside.png"};
    for (int i = 0; i < 4; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 320});
    walls_[1]->SetPosition({-672, -320});
    walls_[2]->SetPosition({256, -320});
    walls_[3]->SetPosition({-832, 0});
    auto movable_wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level3/Rside.png"};
    for (int i = 0; i < 1; ++i) {
        auto movable_wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(movable_wall_image[i]));
        walls_.push_back(movable_wall);
        renderer_.AddChild(movable_wall);
    }
    walls_[4]->SetPosition({1312, 0});

    auto spike_image = RESOURCE_DIR"/image/level/Level3/spike.png";
    for (int i = 0; i < 3; i++) {
        auto spike = std::make_shared<Spike>(spike_image, Spike::Position::Bottom, audio_manager_);
        spikes_.push_back(spike);
        renderer_.AddChild(spike);
        spike->SetPosition({-640 + i * 64, -128});
        spike->Disable();
    }

    auto movable_spike_image = RESOURCE_DIR"/image/level/Level3/Rspike.png";
    for (int i = 0; i < 5; i++) {
        auto movable_spike = std::make_shared<Spike>(movable_spike_image, Spike::Position::Right,
                                                            audio_manager_);
        spikes_.push_back(movable_spike);
        renderer_.AddChild(movable_spike);
        movable_spike->SetPosition({704, -128 + i * 64});
        movable_spike->Disable();
    }

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({384, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-334, 0}, {20, 1000})));
}

void Level3::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480 && character_->GetCurrentState() != Character::State::Vanish) {
            character_->UpdateState(Character::State::Dead);
        }
        if (InputHandler::isGodPressed()){
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

    for (const auto &spike: spikes_) {
        spike->Update(character_);
    }

    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move);
                speed_ = 0;
            }
            break;
        case State::Move:
            for (int i = 0; i < 5; i++) {
                if (!spikes_[i + 3]->GetEnabled()) spikes_[i + 3]->Enable();
            }
            Movable::Move(walls_[4], {320, 0}, speed_);
            for (int i = 0; i < 5; i++) {
                Movable::Move(spikes_[i + 3], {-288, -128 + i * 64}, speed_);
            }
            speed_ += 5;
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Spike);
                timer_ = 1000;
            }
            break;
        case State::Spike:
            Movable::Move(walls_[4], {320, 0}, speed_);
            for (int i = 0; i < 5; i++) {
                Movable::Move(spikes_[i + 3], {-288, -128 + i * 64}, speed_);
            }
            speed_ += 10;
            SpikeDelay();
            timer_ -= Util::Time::GetDeltaTimeMs();
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

void Level3::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
    walls_[4]->SetPosition({1312, 0});
    for (int i = 0; i < 3; i++) {
        spikes_[i]->Disable();
    }
    for (int i = 0; i < 5; i++) {
        spikes_[i + 3]->SetPosition({704, -128 + i * 64});
        spikes_[i + 3]->Disable();
    }
    current_state_ = State::Start;
    speed_ = 0;
    timer_ = 1000;

}

void Level3::UpdateCurrentState(State state) {
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
            if (state == State::Move) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move:
            if (state == State::Spike) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Spike:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}

void Level3::SpikeDelay() {
    if (timer_ > 0) {
        for (int i = 0; i < 3; i++) {
            if (!spikes_[i]->GetEnabled()) spikes_[i]->Enable();
        }
    } else {
        if (spikes_[0]->GetEnabled())audio_manager_.Play(AudioManager::SFX::WallTrap);
        for (int i = 0; i < 3; i++) {
            if (spikes_[i]->GetEnabled()) spikes_[i]->Disable();
        }
    }
}