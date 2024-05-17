//
// Created by User on 2024/3/15.
//

#ifndef LEVELDEVIL_BUTTON_HPP
#define LEVELDEVIL_BUTTON_HPP


#include "Animator.hpp"
#include "Collider.hpp"
#include "Util/Image.hpp"
#include "Util/SFX.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"
#include "GameObject.hpp"

class Button : public GameObject {
public:
    enum class State {
        Idle,
        Hover,
        Click
    };

    explicit Button(const std::shared_ptr<Core::Drawable> &idle,
                    const std::shared_ptr<Core::Drawable> &hover,
                    const std::shared_ptr<Core::Drawable> &click,
                    AudioManager audio_manager,
                    float z_index = 10);

    [[nodiscard]] inline State GetState() const { return current_state_; }

    void UpdateState(State state);

    void Update();

    void Enable() override;

    void Disable() override;

    virtual void OnClick();

    virtual void OnHover();

    virtual void OnIdle();

private:
    AudioManager audio_manager_;
    Animator animator_;
    State current_state_ = State::Idle;
};


#endif //LEVELDEVIL_BUTTON_HPP
