//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_PORTAL_HPP
#define LEVELDEVIL_PORTAL_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "Character.hpp"
#include "IBehaviour.hpp"

class Portal : public Util::GameObject, public IBehaviour {
public:
    enum class State {
        State1,
        State2
    };

    explicit Portal(std::vector<std::string> image_paths, glm::vec2 position, AudioManager audio_manager);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] inline Collider GetCollider() const {
        return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
    }

    void Update(const std::shared_ptr<Character> &character_);

    void Enable() override;

    void Disable() override;

private:
    Animator animator_;
    AudioManager audio_manager_;
    float draw_timer_ = 300;
    glm::vec2 goal_;
    State current_state_ = State::State1;
    bool enable_ = true;
};

#endif //LEVELDEVIL_PORTAL_HPP