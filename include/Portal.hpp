//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_PORTAL_HPP
#define LEVELDEVIL_PORTAL_HPP

#include <string>
#include <glm/vec2.hpp>
#include "GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"

class Portal : public GameObject {
public:
    enum class State {
        State1,
        State2
    };

    explicit Portal(std::vector<std::string> image_paths, AudioManager audio_manager, glm::vec2 position = { 0,0 });

    inline void SetGoal(glm::vec2 position) { goal_ = position; }

    [[nodiscard]] inline Collider GetCollider() const override{
        return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
    }

    void Update(const std::shared_ptr<Character> &character_);

    inline bool IsCollide() { return iscollide_; }

    void Enable() override;

private:
    Animator animator_;
    AudioManager audio_manager_;
    float draw_timer_ = 300;
    float speed_ = 100;
    glm::vec2 goal_;
    State current_state_ = State::State1;
    bool iscollide_ = false;
};

#endif //LEVELDEVIL_PORTAL_HPP