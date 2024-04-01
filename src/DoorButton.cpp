//
// Created by User on 2024/3/15.
//

#include "DoorButton.hpp"

#include <utility>

DoorButton::DoorButton(const std::shared_ptr<Core::Drawable> &idle, AudioManager audio_manager)
        : Button(idle, idle, idle, std::move(audio_manager)) {}

void DoorButton::OnIdle() {
    for (const auto &child: m_Children) {
        child->SetVisible(false);
    }
}

void DoorButton::OnHover() {
    for (const auto &child: m_Children) {
        child->SetVisible(true);
    }
}
