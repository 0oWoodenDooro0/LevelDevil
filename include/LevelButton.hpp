//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_LEVELBUTTON_HPP
#define LEVELDEVIL_LEVELBUTTON_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Animator.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"
#include "Character.hpp"
#include "IBehaviour.hpp"

class LevelButton : public Util::GameObject, public IBehaviour {
public:
    enum class State {
        Up,
        Down
    };

    explicit LevelButton(AudioManager audiomanager);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] inline Collider GetCollider() const {
        return { GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(24, 42) };
    }

    void Update(const std::shared_ptr<Character>& character_);

    void Enable() override;

    void Disable() override;

private:
    Animator animator_;
    AudioManager audiomanager_;
    float timer_ = 0.5;
    State current_state_ = State::Down;
};

#endif //LEVELDEVIL_LEVELBUTTON_HPP