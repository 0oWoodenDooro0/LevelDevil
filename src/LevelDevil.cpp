//
// Created by User on 2024/5/24.
//

#include "LevelDevil.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "Movable.hpp"

#include <utility>

LevelDevil::LevelDevil(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
	: set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void LevelDevil::Start() {
	renderer_.AddChild(transition_.GetTop());
	renderer_.AddChild(transition_.GetBottom());
	background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level_devil/background.png");
	renderer_.AddChild(background_);
	button_ = std::make_shared<EscButton>(audio_manager_);
	button_->SetPosition({ -800, 416 });
	renderer_.AddChild(button_);
	character_ = std::make_shared<Character>(audio_manager_);
	character_->SetCheckPoint({ -704, 0 });
	renderer_.AddChild(character_);

	std::vector<std::string> img_paths = { RESOURCE_DIR"/image/level/level_devil/door.png",
										  RESOURCE_DIR"/image/level/level_devil/in_door1.png",
										  RESOURCE_DIR"/image/level/level_devil/in_door2.png",
										  RESOURCE_DIR"/image/level/level_devil/in_door3.png",
										  RESOURCE_DIR"/image/level/level_devil/in_door4.png",
										  RESOURCE_DIR"/image/level/level_devil/in_door5.png" };
	door_ = std::make_shared<Door>(audio_manager_, img_paths);
	door_->SetPosition({ 704, 0 });
	renderer_.AddChild(door_);
	auto wall_image = std::vector<std::string>{ RESOURCE_DIR"/image/level/level_devil/top1.png",
											   RESOURCE_DIR"/image/level/level_devil/top2.png",
											   RESOURCE_DIR"/image/level/level_devil/top3.png",
											   RESOURCE_DIR"/image/level/level_devil/top4.png",
											   RESOURCE_DIR"/image/level/level_devil/side.png",
											   RESOURCE_DIR"/image/level/level_devil/side.png",
											   RESOURCE_DIR"/image/level/level_devil/bottom.png",
											   RESOURCE_DIR"/image/level/level_devil/movebottom.png" };
	for (int i = 0; i < 8; ++i) {
		auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
		walls_.push_back(wall);
		renderer_.AddChild(wall);
	}
	walls_[0]->SetPosition({ 0, 352 });
	walls_[1]->SetPosition({ 0, 320 });
	walls_[2]->SetPosition({ 0, 288 });
	walls_[3]->SetPosition({ 0, 256 });
	walls_[4]->SetPosition({ -832, 0 });
	walls_[5]->SetPosition({ 832, 0 });
	walls_[6]->SetPosition({ 96, -256 });
	walls_[7]->SetPosition({ -768, -256 });

	auto portal_image = std::vector<std::string>{ RESOURCE_DIR"/image/level/level_devil/portal1.png",
											   RESOURCE_DIR"/image/level/level_devil/portal2.png" };

	auto portal = std::make_shared<Portal>(portal_image, audio_manager_);
	portals_.push_back(portal);
	renderer_.AddChild(portal);

	portals_[0]->SetPosition({ 576, -64 });
	portals_[0]->SetGoal({ -704, 0 });

	for (int i = 0; i < 3; i++)
	{
		auto saw = std::make_shared<Saw>(RESOURCE_DIR"/image/level/level_devil/saw.png", audio_manager_);
		saws_.push_back(saw);
		renderer_.AddChild(saw);
	}

	saws_[0]->SetPosition({ -832, -32 });
	saws_[1]->SetPosition({ 640, -96 });
	saws_[2]->SetPosition({ -832, 0 });

	for (int i = 0; i < 19; i++) {
		auto spike = std::make_shared<Spike>(RESOURCE_DIR"/image/level/level_devil/spike.png", Spike::Position::Top, audio_manager_);
		spikes_.push_back(spike);
		renderer_.AddChild(spike);
		spike->SetPosition({ -576 + i * 64, 0 });
		spike->Disable();
	}



	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ -448, 0 }, { 20, 100 })));
	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ 64, 0 }, { 20, 100 })));
	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ -480, 0 }, { 20, 100 })));
	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ 0, 0 }, { 20, 100 })));
	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ 544, 0 }, { 20, 100 })));
	triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ -576, 0 }, { 20, 100 })));
}

void LevelDevil::Update() {
	if (character_->GetEnabled()) {
		if (character_->GetPosition().y < -480) {
			character_->UpdateState(Character::State::Dead);
		}
		if (InputHandler::isGodPressed()) {
			character_->ChangeGod();
		}
		glm::vec2 input_velocity = { 0, 0 };
		if (character_->GetGod()) {
			input_velocity = InputHandler::GetGodMoveVelocity();
		}
		else {
			input_velocity = InputHandler::GetCharacterMoveVelocity();
		}
		character_->Update(input_velocity, walls_);
	}
	else {
		if (revive_timer_ > 0) {
			revive_timer_ -= Util::Time::GetDeltaTimeMs();
		}
		else if (character_->GetCurrentState() != Character::State::LevelClear && InputHandler::isRevivePressed()) {
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
	portals_[0]->Update(character_);

	for (int i = 0; i < 3; i++)
	{
		saws_[i]->Update(character_);
	}

	if (character_->GetCurrentState() == Character::State::Alive) {
		for (int i = 0; i < 19; i++)
		{
			spikes_[i]->Update(character_);
		}
	}

	switch (current_state_) {
	case State::Intro:
		transition_.Intro([this]() { UpdateCurrentState(State::Start); });
		break;
	case State::Start:
		if(character_->GetCurrentState() == Character::State::Alive)triggerColliders_[0]->Update(character_->GetPosition());
		if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move1);
		}
		break;
	case State::Move1:
		Movable::Move(saws_[0], {128,-32}, trap_speed_);
		triggerColliders_[1]->Update(saws_[0]->GetPosition());
		if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move2);
			trap_speed_ = 330;
		}
		break;
	case State::Move2:
		Movable::Move(saws_[0], { 128,-32 }, trap_speed_);
		Movable::Move(saws_[1], { 640,-32 }, trap_speed_);
		if (saws_[1]->GetPosition() == glm::vec2({ 640, -32 })) {
			UpdateCurrentState(State::Move3);
			delay_timer_ = 500;
			audio_trig_ = false;
		}
		break;
	case State::Move3:
		delay_timer_ -= Util::Time::GetDeltaTimeMs();
		if (delay_timer_ <= 0) {
			if (!audio_trig_) {
				audio_manager_.Play(AudioManager::SFX::Trap);
				audio_trig_ = true;
			}
			Movable::Move(saws_[0], { 128,-96 }, trap_speed_);
			Movable::Move(saws_[1], { -512,-32 }, trap_speed_);
		}
		triggerColliders_[2]->Update(saws_[1]->GetPosition());
		if (triggerColliders_[2]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move4);
		}
		break;
	case State::Move4:
		Movable::Move(saws_[1], { -512,-32 }, trap_speed_);
		Movable::Move(saws_[2], { -800, 0 }, trap_speed_);
		if (saws_[2]->GetPosition() == glm::vec2({ -800, 0 })) {
			UpdateCurrentState(State::Move5);
			delay_timer_ = 500;
			audio_trig_ = false;
		}
		break;
	case State::Move5:
		delay_timer_ -= Util::Time::GetDeltaTimeMs();
		if (delay_timer_ <= 0) {
			if (!audio_trig_) {
				audio_manager_.Play(AudioManager::SFX::Trap);
				audio_trig_ = true;
			}
			Movable::Move(saws_[1], { -512,-96 }, trap_speed_);
			Movable::Move(saws_[2], { -864,-0 }, trap_speed_);
		}
		if (character_->GetCurrentState() == Character::State::Alive)triggerColliders_[3]->Update(character_->GetPosition());
		if (triggerColliders_[3]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move6);
			delay_timer_ = 500;
			spike_timer_ = 0;
			spike_num_ = 0;
		}
		break;
	case State::Move6:
		if (spike_num_ < 19)SpikeAct();
		if (spikes_[9]->GetEnabled())Movable::Move(portals_[0], { 576, 0 }, trap_speed_);
		if (portals_[0]->IsCollide()) {
			UpdateCurrentState(State::Move7);
			delay_timer_ = 1000;
		}
		break;
	case State::Move7:
		delay_timer_ -= Util::Time::GetDeltaTimeMs();
		if (delay_timer_ > 0) {
			if (spike_num_ < 19)SpikeAct();
		}
		if (delay_timer_ <= 0) {
			for (int i = 0; i < 19; i++)spikes_[i]->Disable();
			Movable::Move(portals_[0], { 576, -96 }, trap_speed_);
		}
		if (character_->GetCurrentState() == Character::State::Alive)triggerColliders_[4]->Update(character_->GetPosition());
		if (triggerColliders_[4]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move8);
			trap_speed_ = 400;
		}
		break;
	case State::Move8:
		Movable::Move(door_, { 864, 0 }, trap_speed_);
		if (door_->GetPosition() == glm::vec2({864, 0})) {
			UpdateCurrentState(State::Move9);
			door_->SetPosition({ -864, 0 });
		}
		break;
	case State::Move9:
		Movable::Move(door_, { -704, 0 }, trap_speed_);
		if (character_->GetCurrentState() == Character::State::Alive)triggerColliders_[5]->Update(character_->GetPosition());
		if (triggerColliders_[5]->GetState() == TriggerCollider::State::Trigger) {
			UpdateCurrentState(State::Move10);
		}
		break;
	case State::Move10:
		Movable::Move(door_, { -768, 0 }, trap_speed_);
		Movable::Move(walls_[7], { -832, -256 }, trap_speed_);
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

void LevelDevil::ResetLevel() {
	character_->Revive();
	revive_timer_ = 500;
	door_->SetPosition({ 704, 0 });
	walls_[7]->SetPosition({ -768, -256 });
	portals_[0]->SetPosition({ 576, 64 });
	saws_[0]->SetPosition({ -832, -32 });
	saws_[1]->SetPosition({ 640, -96 });
	saws_[2]->SetPosition({ -832, 0 });
	for (auto spike : spikes_){
		spike->Disable();
	}
	spike_timer_ = 0;
	spike_num_ = 0;
	delay_timer_ = 500;
	revive_timer_ = 500;
	trap_speed_ = 330;
	current_state_ = State::Start;
	for (auto collider : triggerColliders_) {
		collider->UpdateState(TriggerCollider::State::Idle);
	}
}

void LevelDevil::UpdateCurrentState(State state) {
	if (current_state_ == state)return;
	switch (current_state_) {
	case State::Intro:
		if (state == State::Start) {
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Start:
		if (state == State::Move1) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move1:
		if (state == State::Move2) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move2:
		if (state == State::Move3) {
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move3:
		if (state == State::Move4) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move4:
		if (state == State::Move5) {
			current_state_ = state;
		}
		else if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move5:
		if (state == State::Move6) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move6:
		if (state == State::Move7) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move7:
		if (state == State::Move8) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move8:
		if (state == State::Move9) {
			audio_manager_.Play(AudioManager::SFX::Trap);
			current_state_ = state;
		}
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move9:
		if (state == State::Move10) {
			audio_manager_.Play(AudioManager::SFX::WallTrap);
			current_state_ = state;
		}
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Move10:
		if (state == State::Outro) {
			current_state_ = state;
			transition_.ResetTimer();
		}
		break;
	case State::Outro:
		break;
	}
}

void LevelDevil::SpikeAct() {
	spike_timer_ -= float(Util::Time::GetDeltaTimeMs());
	if (spike_timer_ <= 0) {
		if (spike_num_ < 19) {
			spikes_[spike_num_]->Enable();
		}
		spike_timer_ = 110;
		spike_num_++;
	}
}