//
// Created by User on 2024/3/12.
//

#ifndef LEVELDEVIL_BUTTON_HPP
#define LEVELDEVIL_BUTTON_HPP


#include "Util/GameObject.hpp"
#include "Collider.hpp"

template<class... Args>
class Button : public Util::GameObject {
    typedef std::function<void(Args...)> eventFunction;

public:
    enum class State {
        Idle,
        Hover
    };

    explicit Button(const std::shared_ptr<Core::Drawable> &drawable, eventFunction on_click, float z_index = 0)
            : Util::GameObject(), on_click_function_(on_click) {
        SetDrawable(drawable);
        SetZIndex(z_index);
    }

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    inline glm::vec2 GetPosition() { return m_Transform.translation; }

    inline glm::vec2 GetSize() { return m_Drawable->GetSize(); }

    inline Collider GetCollider() { return {GetPosition(), GetSize()}; }

    void UpdateState(State state) {
        if (current_state_ == state) return;
        current_state_ = state;
        switch (current_state_) {
            case State::Idle:
                Idle();
                break;
            case State::Hover:
                Hover();
                break;
        }
    }

    void Hover() {
        for (const auto &child: m_Children) {
            child->SetVisible(true);
        }
    }

    void Idle() {
        for (const auto &child: m_Children) {
            child->SetVisible(false);
        }
    }

    inline void OnClick(const Args &... args) const { (on_click_function_)(args...); }

private:
    State current_state_ = State::Idle;
    eventFunction on_click_function_;
};


#endif //LEVELDEVIL_BUTTON_HPP
