//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_GAMEOBJECT_HPP
#define LEVELDEVIL_GAMEOBJECT_HPP

#include "Util/GameObject.hpp"
#include "Collider.hpp"

class GameObject : public Util::GameObject {
public:
    GameObject() = default;

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    inline void SetRotation(float rotation) { m_Transform.rotation = rotation; }

    inline void SetEnabled(bool enabled) { enabled_ = enabled; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] virtual inline Collider GetCollider() const { return {GetPosition(), GetSize()}; }

    [[nodiscard]] inline bool GetEnabled() const { return enabled_; }

    virtual void Enable();

    virtual void Disable();

private:
    bool enabled_ = true;
};


#endif //LEVELDEVIL_GAMEOBJECT_HPP
