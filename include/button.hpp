//
// Created by User on 2024/3/11.
//

#ifndef LEVELDEVIL_BUTTON_HPP
#define LEVELDEVIL_BUTTON_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Animator.hpp"

class Button : public Util::GameObject {
public:
    explicit Button(const std::string& Idle, const std::string& Cursor_OnTop, const std::string& Pressed);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    bool GetIsKeyEvent() const { return keyEvent_; }

    void Update();

private:
    Animator animator_;
    bool keyEvent_;
};

#endif //LEVELDEVIL_BUTTON_HPP