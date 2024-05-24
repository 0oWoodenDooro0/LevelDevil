//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_LEVELBUTTON_HPP
#define LEVELDEVIL_LEVELBUTTON_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"
#include "Character.hpp"

class LevelButton : public GameObject {
public:
    enum class State {
        Up,
        Down
    };

    explicit LevelButton(AudioManager audiomanager);

    [[nodiscard]] inline Collider GetCollider() const override {
        return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(24, 42)};
    }

    inline void SetUp() { current_state_ = State::Up; }

    inline State GetcurrentState() { return current_state_; }

    void Update(const std::shared_ptr<Character> &character);

private:
    Animator animator_;
    AudioManager audiomanager_;
    State current_state_ = State::Up;
};

#endif //LEVELDEVIL_LEVELBUTTON_HPP