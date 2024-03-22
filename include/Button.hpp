//
// Created by User on 2024/3/15.
//

#ifndef LEVELDEVIL_BUTTON_HPP
#define LEVELDEVIL_BUTTON_HPP


#include "Util/GameObject.hpp"
#include "Animator.hpp"
#include "Collider.hpp"
#include "Util/Image.hpp"
#include "IBehaviour.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"

class Button : public Util::GameObject, public IBehaviour {
public:
    enum class State {
        Idle,
        Hover,
        Click
    };

    explicit Button(const std::shared_ptr<Core::Drawable> &idle,
                    const std::shared_ptr<Core::Drawable> &hover,
                    const std::shared_ptr<Core::Drawable> &click,
                    float z_index = 10);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    inline glm::vec2 GetPosition() { return m_Transform.translation; }

    inline glm::vec2 GetSize() { return m_Drawable->GetSize(); }

    inline Collider GetCollider() { return {GetPosition(), GetSize()}; }

    inline State GetState() { return current_state_; }

    void UpdateState(State state);

    void Update();

    void Enable() override;

    void Disable() override;

    virtual void OnClick() {};

    virtual void OnHover() {};

    virtual void OnIdle() {};

private:
    AudioManager audiomanager_;
    Animator animator_;
    State current_state_ = State::Idle;
};


#endif //LEVELDEVIL_BUTTON_HPP
