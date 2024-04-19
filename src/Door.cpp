//
// Created by User on 2024/3/8.
//

#include "Door.hpp"

#include <utility>
#include <string>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Character.hpp"
#include "Animator.hpp"

Door::Door(AudioManager audio_manager, const std::string& idle_path, const std::string& delay_path, const std::string& closedoor_path1,
			const std::string& closedoor_path2, const std::string& closedoor_path3, const std::string& closedoor_path4)
	: Util::GameObject(), audio_manager_(std::move(audio_manager)) {
	SetZIndex(0);
	m_Drawable = std::make_unique<Util::Image>(idle_path);
	animator_.SetAnimationStates(
		{ {"Idle",       std::make_unique<Util::Image>(idle_path)},
		 {"Delay",      std::make_unique<Util::Image>(delay_path)},
		 {"CloseDoor", std::make_unique<Util::Animation>(
				 std::vector<std::string>{(closedoor_path1),
										  (closedoor_path2),
										  (closedoor_path3),
										  (closedoor_path4)}, true, 50,
				 false, 0)} });
}

void Door::Update(const std::shared_ptr<Character>& character_) {
	if (!enabled_) {
		return;
	}
	std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
		std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
	if (current_state_ == State::CloseDoor) {
		if (timer_ <= 0) {
			audio_manager_.Play(AudioManager::SFX::StageClear);
			Disable();
		}
		else {
			timer_ -= float(Util::Time::GetDeltaTime());
		}
		return;
	}
	if (current_state_ == State::Delay) {
		if (timer_ <= 0) {
			animator_.UpdateAnimationState("CloseDoor", set_drawable_function);
			current_state_ = State::CloseDoor;
			timer_ = 0.25;
		}
		else {
			timer_ -= float(Util::Time::GetDeltaTime());
		}
		return;
	}
	if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
		character_->UpdateState(Character::State::LevelClear);
		animator_.UpdateAnimationState("Delay", set_drawable_function);
		current_state_ = State::Delay;
		timer_ = 0.4;
		audio_manager_.Play(AudioManager::SFX::Door);
	}
	else {
		animator_.UpdateAnimationState("Idle", set_drawable_function);
		current_state_ = State::Idle;
	}
}

void Door::Enable() {
	enabled_ = true;
	SetVisible(true);
}

void Door::Disable() {
	enabled_ = false;
	SetVisible(false);
}
