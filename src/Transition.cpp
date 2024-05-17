//
// Created by User on 2024/4/20.
//

#include "Transition.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "EasingFunction.hpp"

Transition::Transition() {
    top_ = std::make_shared<Sprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_top.png"),
            20);
    bottom_ = std::make_shared<Sprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_bottom.png"),
            20);
    top_->SetPosition({0, 208});
    bottom_->SetPosition({0, -210});
}

void Transition::Intro(const std::function<void()> &function) {
    if (transition_timer_ < 1) {
        transition_timer_ += Util::Time::GetDeltaTimeMs() * transition_delta_time_multiple_;
    } else {
        transition_timer_ = 1;
        function();
    }
    top_->SetPosition({0, int(208 + EasingFunction::EaseInCubic(transition_timer_) * 544)});
    bottom_->SetPosition({0, int(-210 - EasingFunction::EaseInCubic(transition_timer_) * 540)});
}

void Transition::Outro(const std::function<void()> &function) {
    if (transition_timer_ < 1) {
        transition_timer_ += Util::Time::GetDeltaTimeMs() * transition_delta_time_multiple_;
    } else {
        transition_timer_ = 1;
        transition_end_timer_ += Util::Time::GetDeltaTimeMs();
        if (transition_end_timer_ >= transition_end_delay_) {
            function();
        }
    }
    top_->SetPosition({0, int(752 - EasingFunction::EaseOutCubic(transition_timer_) * 544)});
    bottom_->SetPosition({0, int(-750 + EasingFunction::EaseOutCubic(transition_timer_) * 540)});
}
