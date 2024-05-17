//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_SPRING_HPP
#define LEVELDEVIL_SPRING_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"

class Spring : public GameObject {
public:
    enum class State {
        Up,
        Down
    };

    explicit Spring(std::vector<std::string> image_paths, AudioManager audio_manager);

    [[nodiscard]] inline Collider GetCollider() const override {
        return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
    }

    void Update(const std::shared_ptr<Character> &character_);

    void Reset();
  
private:
    Animator animator_;
    AudioManager audio_manager_;
    float draw_timer_ = 500;
    float bounce_timer_ = 100;
    State current_state_ = State::Down;
};

#endif //LEVELDEVIL_SPRING_HPP