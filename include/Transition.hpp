//
// Created by User on 2024/4/20.
//

#ifndef LEVELDEVIL_TRANSITION_HPP
#define LEVELDEVIL_TRANSITION_HPP


#include <functional>
#include "GameObject.hpp"
#include "Sprite.hpp"

class Transition {
public:
    Transition();

    [[nodiscard]] inline std::shared_ptr<GameObject> GetTop() const { return top_; }

    [[nodiscard]] inline std::shared_ptr<GameObject> GetBottom() const { return bottom_; }

    inline void ResetTimer() { transition_timer_ = 0; }

    void Intro(const std::function<void()> &function);

    void Outro(const std::function<void()> &function);

private:
    std::shared_ptr<Sprite> top_;
    std::shared_ptr<Sprite> bottom_;

    float transition_timer_ = 0;
    float transition_delta_time_multiple_ = 0.003f;
    float transition_end_timer_ = 0;
    float transition_end_delay_ = 300;
};


#endif //LEVELDEVIL_TRANSITION_HPP
