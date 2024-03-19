//
// Created by User on 2024/3/12.
//

#ifndef LEVELDEVIL_SPRITE_HPP
#define LEVELDEVIL_SPRITE_HPP


#include <string>
#include "Util/GameObject.hpp"
#include "Collider.hpp"
#include "Behaviour.hpp"

class Sprite : public Util::GameObject, public Behaviour {
public:
    explicit Sprite(const std::shared_ptr<Core::Drawable> &drawable, float z_index = 0);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    inline glm::vec2 GetPosition() { return m_Transform.translation; }

    inline glm::vec2 GetSize() { return m_Drawable->GetSize(); }

    inline Collider GetCollider() { return {GetPosition(), GetSize()}; }

    void Enable() override;

    void Disable() override;
};


#endif //LEVELDEVIL_SPRITE_HPP
